/*
 * Copyright (C) 2014 Marlin Trust Management Organization
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MARLIN_CDM_INTERFACE_H_
#define MARLIN_CDM_INTERFACE_H_

#include <stdint.h>
#include <memory>

#include "MarlinError.h"
#include "MarlinCommonTypes.h"
#include "CMutex.h"

/**
 * @brief
 * It describes Marlin IPTV-ES CDM(Content Decryption Module) interface for HTML5(EME). 
 * Marlin CDM can be used to operate the key acquisition of the marlin protected content 
 * and to decrypt the marlin protected content on web browser and web browser platform.
 * EME Verison is 0.1b.
 * @image html marlincdm_overview.jpg
 * Marlin CDM includes below part of implementation.
 * - Marlin CDM Adapter : IIt is glue code to absorb API difference between Marlin CDM Interface and each browsers EME.
 * - Marlin CDM Interface : It is interface layer between Web browser framework and Marlin CDM.
 * - Marlin CDM Engine : It is implementation to delegate HTTP transaction responsibility with License server to Application. It also has session and key management for each application call
 * - Marlin Agent Handler : It is a Porting layer to support Marlin DRM solution in conjunction with specialized SDK. With this layer customization, implementer can use any Marlin DRM Agent.
 *
 * @brief
 * The following diagram shows the sequence while acquiring marlin IPTV-ES license and playing back marlin protected content for EME case.
 * @image html marlincdm_seq_license.jpg
 *
 * The following diagram shows details of the above sequence.
 * @image html marlincdm_seq_license_1.jpg
 * @image html marlincdm_seq_license_2.jpg
 * @image html marlincdm_seq_license_3.jpg
 *
 * @version 1.0
 */

namespace marlincdm {

using namespace std;

/**
 * @brief
 * Marlin IPTV-ES CDM(Content Decryption Module) interface for HTML5(EME).
 *
 */
class MarlinCdmInterface {
    public:

    /**
     * @brief This function checks the presence of Key.
     *
     * @param[in] init_data Initialization data of media file. \n
     * init_data includes KeyID information(PSSH information or ECM information).\n
     * Byte index       | Description              | Byte size                     | Mandatory
     * ---------------- | ------------------------ | ----------------------------- | -------------
     * 0                | KeyID information type   | 1                             | Yes
     * 1 - 4            | KeyID information length | 4                             | Yes
     * 5 - (5+(a-1))    | KeyID information data   | KeyID information length :(a) | No
     *
     * - KeyID information type
     *  - None
     *  - PSSH information (only use when media file includes PSSH box) 
     *  - ECM information (only use when media file includes ECM info) 
     * - KeyID information length:\n
     *  Size of PSSH or ECM information data.\n
     *  When KeyID information type is "None", set to "00000000h".
     * - KeyID information data:\n
     *  PSSH or ECM information data. \n
     *  When KeyID information type is "None", do not set anything.
     *
     * @param[out] is_key_exist "true": Key is exist. "false": Key is not exist.
     *
     * @retval OK Check key exist is success
     * @retval ERROR_ILLEGAL_ARGUMENT Input parameter is invalid
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t CheckKeyExist(const mcdm_buffer_t& init_data, bool* is_key_exist);

    /**
     * @brief This function is opening new session associated with Marlin CDM object.
     *
     * @param[out] session_id Session ID that can be used by application to identify Marlin CDM objects.
     * @retval OK Openning session is success
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t OpenSession(mcdm_session_id_t& session_id);

    /**
     * @brief This function closes session opened by OpenSession().
     *
     * @param[in] session_id Session ID which is opened by OpenSession()
     * @retval OK Closing session is success
     * @retval ERROR_ILLEGAL_ARGUMENT Input parameter is invalid
     * @retval ERROR_SESSION_NOT_OPENED Session ID does not exist
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t CloseSession(const mcdm_session_id_t& session_id);

    /**
     * @brief This function generates key request and acquires license.
     *
     * GenerateKeyRequest function provides following processes in conjunction with [AddKey()](@ref AddKey) function :
     * After calling this function, caller should call [AddKey()](@ref AddKey) function for Marlin IPTV-ES License acquisition. \n
     *
     * @param[in] session_id Session ID which is opened by OpenSession()
     * @param[in] init_data Initialization data of acquisition process. \n
     * init_data includes RequestType, ActionID, ActionParameter, PrivateDataTag&PrivateData, UsageRuleReference, DRMServerURI,\n
     * KeyID information(PSSH information or ECM information).\n
     * Byte index                      | Description                  | Byte size                        | Mandatory
     * ------------------------------- | ---------------------------- | -------------------------------- | ----------
     * 0                               | RequestType                  | 1                                | Yes
     * 1                               | ActionID                     | 1                                | Yes
     * 2                               | ActionParameter              | 1                                | Yes
     * 3 - 30                          | PrivateDataTag & PrivateData | 28                               | Yes
     * 31 - 46                         | UsageRuleReference           | 16                               | Yes
     * 47 - 50                         | DRMServerURI length          | 4                                | Yes
     * 51 - (51+(a-1))                 | DRMServerURI data            | DRMServerURI length :(a)         | No
     * (51+a)                          | KeyID information type       | 1                                | Yes
     * (52+a) - (55+a)                 | KeyID information length     | 4                                | Yes
     * (56+a) - ((56+a)+(b-1))         | KeyID information data       | KeyID information length :(b)    | No
     *
     * - RequestType
     *  - Get Permission Protocol
     *  - Get Trusted Time Protocol
     * - ActionID\n
     *  Only use When RequestType is "Get Permission Protocol".\n
     *  When ActionID is not in use, set to "None".
     *  - None
     *  - EXTRACT with Simple Key Delivery
     *  - EXTRACT with InDirect Key Delivery
     *  - EXTRACT with Direct Key Delivery
     *  - EXPORT for Copy with Direct Key Delivery
     *  - EXPORT for Move with Direct Key Delivery
     *  - EXPORT with Indirect Key Delivery
     *  - RECORD with Indirect Key Delivery
     * - ActionParameter\n
     *  Only use when ActionID is EXPORTs.\n
     *  When ActionParameter is not in use, set to "None".
     *  - None
     *  - Export to DTCP
     *  - Export to CPRM for DVD
     *  - Export to CPRM for SD Video (Revision 0.93)
     *  - Export to CPRM for SD Audio
     *  - Export to MG-R (SVR) for Memory Stick PRO
     *  - Export to MG-R (SAR) for Memory Stick and Memory Stick PRO
     *  - Export to VCPS
     *  - Export to MG-R (SVR) for EMPR
     *  - Export to MG-R (SAR) for ATRAC Audio Device
     *  - Export to SAFIA for iVDR TV Recording
     *  - Export to SAFIA for iVDR Audio Recording
     *  - Export to AACS Blu-ray Disc Recordable for BD-R/RE (Revision 0.92)
     *  - Export to AACS Blu-ray Disc Recordable for Red Laser Media (Revision 0.92)
     *  - Export to CPRM for SD Video (Revision 0.96)
     *  - Export to AACS Blu-ray Disc Recordable for BD-R/RE (Revision 0.95)
     *  - Export to AACS Blu-ray Disc Recordable for Red Laser Media (Revision 0.95)
     *  - Export to NSM CPS
     * - PrivateDataTag & PrivateData\n
     *  Only use when ActionID is EXPORTs.\n
     *  Set to all "00h" when PrivateData is not in use.
     * - UsageRuleReference\n
     *  Only use when RequestType is "Get Permission Protocol".\n
     *  Set to all "00h" when UsageRuleReference is not in use.
     * - DRMServerURI length\n
     *  Only use when RequestType is "Get Permission Protocol".\n
     *  When DRMServerURI is not in use, set to "00000000h".
     * - DRMServerURI data:\n
     *  Only use when RequestType is "Get Permission Protocol".\n
     *  When DRMServerURI is not in use, do not set anything.
     * - KeyID information type\n
     *  Only use when RequestType is "Get Permission Protocol".\n
     *  When KeyID information type is not in use, set to "None".
     *  - None
     *  - PSSH information (only use when media file includes PSSH box) 
     *  - ECM information (only use when media file includes ECM info) 
     * - KeyID information length:\n
     *  Size of PSSH or ECM information data.\n
     *  Only use when RequestType is "Get Permission Protocol".\n
     *  When KeyID information type is "None", set to "00000000h".
     * - KeyID information data:\n
     *  PSSH or ECM information data.\n
     *  Only use when RequestType is "Get Permission Protocol".\n
     *  When KeyID information type is "None", do not set anything.
     * @param[out] request Request message data.
     *
     * @retval OK Generating request message is success
     * @retval ERROR_ILLEGAL_ARGUMENT Input parameter is invalid
     * @retval ERROR_SESSION_NOT_OPENED Session ID has been closed already or Session ID does not exist.
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t GenerateKeyRequest(const mcdm_session_id_t& session_id,
                                     const mcdm_buffer_t& init_data,
                                     mcdm_buffer_t* request);

    /**
     * @brief This function is adding key to Marlin CDM to be associated with Session ID.\n
     * After calling [GenerateKeyRequest()](@ref GenerateKeyRequest) function, caller should call this function.
     *
     * @param[in] session_id Session ID which is opened by OpenSession()
     * @param[in] key Response data that should be sent to Marlin CDM.
     * @param[in] init_data Initialization data of acquisition process. \n
     * init_data is same format as GenerateKeyRequest(). There is also a case of NULL.
     *
     * @param[out] endflag flag whether step remained
     * @param[out] request Request message data. only set when continue acquisitions
     *
     * @retval OK Adding key is success
     * @retval ERROR_ILLEGAL_ARGUMENT Input parameter is invalid
     * @retval ERROR_SESSION_NOT_OPENED Session ID does not exist
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t AddKey(const mcdm_session_id_t& session_id,
                         const mcdm_buffer_t& key,
                         const mcdm_buffer_t& init_data,
                         bool* endflag,
                         mcdm_buffer_t* request);

    /**
     * @brief This function is canceling session linked request information which is generated by GenerateKeyRequest().
     *
     * @param[in] session_id Session ID which is opened by OpenSession()
     * @retval OK Canceling request is success
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t CancelKeyRequest(const mcdm_session_id_t& session_id);

    /**
     * @brief This function provides decryption of media content.
     *
     * @param[in] init_data Initialization data of media file. \n
     * init_data includes KeyID information(PSSH information or ECM information).\n
     * Byte index       | Description              | Byte size                     | Mandatory
     * ---------------- | ------------------------ | ----------------------------- | -------------
     * 0                | KeyID information type   | 1                             | Yes
     * 1 - 4            | KeyID information length | 4                             | Yes
     * 5 - (5+(a-1))    | KeyID information data   | KeyID information length :(a) | No
     *
     * - KeyID information type
     *  - None
     *  - PSSH information (only use when media file includes PSSH box) 
     *  - ECM information (only use when media file includes ECM info) 
     * - KeyID information length:\n
     *  Size of PSSH or ECM information data.\n
     *  When KeyID information type is "None", set to "00000000h".
     * - KeyID information data:\n
     *  PSSH or ECM information data. \n
     *  When KeyID information type is "None", do not set anything.
     *
     * @param[in] src_ptr Input buffer of encrypted data
     * @param[out] dst_ptr Output buffer of decrypted data
     *
     * @retval OK success
     * @retval ERROR_ILLEGAL_ARGUMENT Input parameter is invalid
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    // Accept encrypted buffer and return decrypted data.
    mcdm_status_t Decrypt(const mcdm_buffer_t& init_data,
                          mcdm_buffer_t* src_ptr,
                          mcdm_buffer_t* dst_ptr);

    /**
     * @brief This function generates one or more key release messages.
     *
     * @param[out] key_release Key release structural informations pointer.(Session ID & Key release message)
     * @param[out] key_release_num Number of Key release structure.
     * @retval OK Generate key release messages is success
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t GetKeyReleases(mcdm_key_release_t** key_release, uint32_t* key_release_num);

    /**
     * @brief This function commits the key release message.
     *
     * @param[in] key_release Key release structural informations.(Session ID & Key release message)
     * @retval OK Commit key release messages is success
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t AddKeyReleaseCommit(const mcdm_key_release_t& key_release);

    /**
     * @brief This function free the key release message buffer.( allocated by GetKeyReleases() )
     *
     * @param[in] key_release Key release structural informations pointer.(Session ID & Key release message)
     * @param[in] key_release_num Number of Key release structure.
     * @retval OK Free the buffer is success
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    mcdm_status_t FreeKeyReleasesBuffer(mcdm_key_release_t* key_release, uint32_t key_release_num);

    /**
     * @brief This function get the MarlinCdmInterface instance. (singleton)
     *
     * - getMarlinCdmInterface() / releaseMarlinCdmInterface() must be called in pairs\n
     *   (or must be called to the same number of times).
     *
     * @retval MarlinCdmInterface instance or NULL
     */
    static MarlinCdmInterface* getMarlinCdmInterface();

    /**
     * @brief This function release the MarlinCdmInterface instance. (singleton)
     *
     * - getMarlinCdmInterface() / releaseMarlinCdmInterface() must be called in pairs\n
     *   (or must be called to the same number of times).
     * - After calling releaseMarlinCdmInterface(), you should not be using the current CDM instance.
     *
     * @retval OK release is success
     * @retval ERROR_UNKNOWN Error by other reasons
     */
    static mcdm_status_t releaseMarlinCdmInterface();

    private:

    /**
     * @brief
     * Construct MarlinCdmInterface.
     */
    MarlinCdmInterface();
    virtual ~MarlinCdmInterface();

    MarlinCdmInterface(const MarlinCdmInterface &o);
    MarlinCdmInterface& operator=(const MarlinCdmInterface &o);

}; // marlincdminterface

}  // namespace marlincdm

#endif  // MARLIN_CDM_INTERFACE_H_


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
