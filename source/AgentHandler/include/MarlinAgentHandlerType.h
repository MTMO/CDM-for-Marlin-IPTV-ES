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

#ifndef MARLIN_AGENT_HANDLER_TYPE_H_
#define MARLIN_AGENT_HANDLER_TYPE_H_

#include <string>
#include <stdint.h>

#define MH_PRIVATE_DATA_SIZE 28
#define MH_URR_DATA_SIZE 16

using namespace std;

/**
 * @brief Error Type of Marlin Agent Handler
 */
enum MH_status_t {
  MH_ERR_OK = 0, //!< process was success.
  MH_ERR_FAILURE, //!< process failed for error of failure.
  MH_ERR_FAILURE_AGENT, //!< error of Agent.
  MH_ERR_INVALID_REQ_TYPE, //!< RequestType error.
  MH_ERR_INVALID_ACTION_ID, //!< ActionID error.
  MH_ERR_INVALID_ACTION_PARAM, //!< ActionParameter error.
  MH_ERR_INVALID_RESPONSE_MSG, //!< Response message error.
};

/**
 * @brief This parameter show agentHandle.
 */
typedef void* MH_agentHandle_t;

/**
 * @brief This parameter show iptvesHandle.
 */
typedef void* MH_iptvesHandle_t;

/**
 * @brief Unique string to identify Marlin CDM object
 */
typedef string MH_session_id_t;

/**
 * @brief This structure includes buf length and data buffer and fd for platform specific buffer.
 */
struct MH_buffer_t {
    size_t len; //!< data size
    uint8_t* data; //!< data buffer
    int fd; //!< File descriptor for platform specific buffer
};

/**
 * @brief RequestType for createChallengeRequest
 */
enum MH_requestType {
    REQUEST_TYPE_NONE = 0, //!< none
    REQUEST_TYPE_PERMISSION, //!< Get Permission Protocol
    REQUEST_TYPE_TRUSTED_TIME, //!< Get Trusted Time Protocol
};

/**
 * @brief ActionID for createChallengeRequest
 */
enum MH_actionId {
    ACT_ID_NONE = 0, //!< none
    ACT_ID_EXTRACT_SIMPLE_KEY, //!< EXTRACT with Simple Key Delivery
    ACT_ID_EXTRACT_INDIRECT_KEY, //!< EXTRACT with InDirect Key Delivery
    ACT_ID_EXTRACT_DIRECT_KEY, //!< EXTRACT with Direct Key Delivery
    ACT_ID_EXPORT_COPY_DIRECT_KEY, //!< EXPORT for Copy with Direct Key Delivery
    ACT_ID_EXPORT_MOVE_DIRECT_KEY, //!< EXPORT for Move with Direct Key Delivery
    ACT_ID_EXPORT_INDIRECT_KEY, //!< EXPORT with Indirect Key Delivery
    ACT_ID_RECORD_INDIRECT_KEY, //!< RECORD with Indirect Key Delivery
};

/**
 * @brief ActionParameter for createChallengeRequest
 */
enum MH_actionParam {
    ACT_PARAM_NONE = 0, //!< none
    ACT_PARAM_DTCP, //!< Export to DTCP
    ACT_PARAM_CPRM_DVD, //!< Export to CPRM for DVD
    ACT_PARAM_CPRM_SD_VIDEO_093, //!< Export to CPRM for SD Video (Revision 0.93)
    ACT_PARAM_CPRM_SD_AUDIO, //!< Export to CPRM for SD Audio
    ACT_PARAM_MGR_MEMORY_STICK_PRO, //!< Export to MG-R (SVR) for Memory Stick PRO
    ACT_PARAM_MGR_MEMORY_STICK, //!< Export to MG-R (SAR) for Memory Stick and Memory Stick PRO
    ACT_PARAM_VCPS, //!< Export to VCPS
    ACT_PARAM_MGR_EMPR, //!< Export to MG-R (SVR) for EMPR
    ACT_PARAM_MGR_ATRAC, //!< Export to MG-R (SAR) for ATRAC Audio Device
    ACT_PARAM_SAFIA_IVDR_TV, //!< Export to SAFIA for iVDR TV Recording
    ACT_PARAM_SAFIA_IVDR_AUDIO, //!< Export to SAFIA for iVDR Audio Recording
    ACT_PARAM_AACS_BDR_092, //!< Export to AACS Blu-ray Disc Recordable for BD-R/RE (Revision 0.92)
    ACT_PARAM_AACS_RLM_092, //!< Export to AACS Blu-ray Disc Recordable for Red Laser Media (Revision 0.92)
    ACT_PARAM_CPRM_SD_VIDEO_096, //!< Export to CPRM for SD Video (Revision 0.96)
    ACT_PARAM_AACS_BDR_095, //!< Export to AACS Blu-ray Disc Recordable for BD-R/RE (Revision 0.95)
    ACT_PARAM_AACS_RLM_095, //!< Export to AACS Blu-ray Disc Recordable for Red Laser Media (Revision 0.95)
    ACT_PARAM_NSM_CPS, //!< Export to NSM CPS
};

/**
 * @brief KeyID information Type
 */
enum MH_keyIdInfoType {
    KEY_ID_INFO_TYPE_NONE = 0, //!< none
    KEY_ID_INFO_TYPE_PSSH, //!< PSSH information
    KEY_ID_INFO_TYPE_ECM, //!< ECM information
};

/**
 * @brief This structure includes keyIdInfo information.
 */
struct MH_keyIdInfo_t {
    MH_keyIdInfoType type; //!< KeyID information Type
    size_t length; //!< KeyID information length
    uint8_t* data; //!< KeyID information data
};

/**
 * @brief This structure includes challengeParameter information.
 */
struct MH_challengeParameter_t {
    MH_requestType req_type; //!< RequestType
    MH_actionId action_id; //!< ActionID
    MH_actionParam act_param; //!< ActionParameter
    uint8_t private_data[MH_PRIVATE_DATA_SIZE]; //!< PrivateDataTag & PrivateData.
    uint8_t urr_data[MH_URR_DATA_SIZE]; //!< UsageRuleReference
    size_t server_uri_length; //!< DRMServerURI length
    uint8_t* server_uri_data; //!< DRMServerURI data
    MH_keyIdInfo_t kid_info; //!< KeyID information
};

/**
 * @brief This structure includes keyRelease information.
 */
struct MH_keyRelease_t {
    MH_session_id_t session_id; //!< session id
    MH_buffer_t msg_buf; //!< Key release message data buffer
};

#endif /* __MARLIN_AGENT_HANDLER_TYPE_H__ */


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
