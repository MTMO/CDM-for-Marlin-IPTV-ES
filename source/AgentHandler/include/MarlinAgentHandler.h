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

#ifndef __MARLIN_AGENT_HANDLER_H__
#define __MARLIN_AGENT_HANDLER_H__

#include "MarlinAgentHandlerType.h"

#include <vector>


namespace marlincdm {

/**
 * @brief
 * This is adaptation class in Marlin CDM, which links to Marlin DRM Agent.
 *
 */
class MarlinAgentHandler {

public:

  /**
   * @brief Construct MarlinAgentHandler.
   *
   */
  MarlinAgentHandler();
  virtual ~MarlinAgentHandler();

  /**
   * @brief Get reference count.
   *
   * @retval current reference count
   */
  uint32_t getRefCount(void);

  /**
   * @brief Increase reference count.
   *
   * @retval MH_ERR_OK Increasing RefCount is success
   * @retval MH_ERR_FAILURE Cannot increase reference count
   */
  MH_status_t increaseRefCount(void);

  /**
   * @brief Decrease reference count.
   *
   * @retval MH_ERR_OK Decreasing RefCount is success
   * @retval MH_ERR_FAILURE Cannot decrease reference count
   */
  MH_status_t decreaseRefCount(void);

  /**
   * @brief Generate one or more key release messages.
   *
   * @param[out] o_key_release Key release structural informations pointer.(Marlin CDM Session ID & Key release message)
   * @param[out] o_key_release_num Number of Key release structure.
   *
   * @retval MH_ERR_OK Generation is success
   * @retval MH_ERR_FAILURE Cannot Generate the messages
   */
  MH_status_t getKeyReleases(MH_keyRelease_t** o_key_release, uint32_t* o_key_release_num);

  /**
   * @brief Commits the key release message.
   *
   * @param[in] i_key_release Key release structural informations.(Marlin CDM Session ID & Key release message)
   *
   * @retval MH_ERR_OK Committing is success
   * @retval MH_ERR_FAILURE Cannot Commit the messages
   */
  MH_status_t addKeyReleaseCommit(MH_keyRelease_t* i_key_release);

  /**
   * @brief Free the key release message buffer.( allocated by getKeyReleases() )
   *
   * @param[in] i_key_release Key release structural informations pointer.(Marlin CDM Session ID & Key release message)
   * @param[in] i_key_release_num Number of Key release structure.
   *
   * @retval MH_ERR_OK Free the buffer is success
   * @retval MH_ERR_FAILURE Cannot free the buffer
   */
  MH_status_t freeKeyReleasesBuffer(MH_keyRelease_t* i_key_release, uint32_t i_key_release_num);

  /**
   * @brief Load Marlin Agent.
   *
   * @param [out] o_handle Agent handle
   *
   * @retval MH_ERR_OK Init agent is success
   * @retval MH_ERR_FAILURE Cannot initialize Agent
   */
  MH_status_t initAgent(MH_agentHandle_t* o_handle);

  /**
   * @brief Unload Marlin Agent. \n It is necessary for you to call initAgent() function before calling this function.
   *
   * @param [in] i_handle Agent handle
   *
   * @retval MH_ERR_OK Finalize agent is success
   * @retval MH_ERR_FAILURE Cannot finalize Agent
   */
  MH_status_t finAgent(MH_agentHandle_t i_handle);

  /**
   * @brief This function checks the presence of Key.
   *
   * @param [in] i_parameter includes KeyID information(PSSH information or ECM information).\n
   * @param [out] o_is_key_exist "true": Key is exist. "false": Key is not exist.
   * 
   * @retval MH_ERR_OK Check key exist is success
   * @retval MH_ERR_FAILURE Cannot check key exist
   */
  MH_status_t checkKeyExist(MH_keyIdInfo_t* i_parameter, bool* o_is_key_exist);

  /**
   * @brief Initialize Marlin IPTV-ES session.
   *
   * @param [in] i_handle Agent handle
   * @param [in] i_session_id Marlin CDM Session ID
   * @param [out] o_handle Marlin IPTV-ES license handle
   *
   * @retval MH_ERR_OK Intialize IPTV-ES Handle is success
   * @retval MH_ERR_FAILURE Cannot initialize IPTV-ES Handle
   */
  MH_status_t initIPTVESHandle(MH_agentHandle_t i_handle, MH_session_id_t i_session_id, MH_iptvesHandle_t* o_handle);

  /**
   * @brief Finalize marlin IPTV-ES session.
   *
   * @param [in] i_handle Marlin IPTV-ES license handle
   *
   * @retval MH_ERR_OK Finalize IPTV-ES Handle is success
   * @retval MH_ERR_FAILURE Cannot finalize IPTV-ES Handle
   */
  MH_status_t finIPTVESHandle(MH_iptvesHandle_t i_handle);

  /**
   * @brief Create http request of challenge.
   *
   * @param [in] i_handle Marlin IPTV-ES license handle
   * @param [in] i_parameter parameters to process.\n
   * i_parameter includes RequestType, ActionID, ActionParameter, PrivateDataTag&PrivateData, UsageRuleReference,\n
   * DRMServerURI, KeyID information(PSSH information or ECM information).\n
   * @param [out] o_request Http request message data.
   *
   * @retval MH_ERR_OK Create is success
   * @retval MH_ERR_INVALID_REQ_TYPE RequestType error
   * @retval MH_ERR_INVALID_ACTION_ID ActionID error
   * @retval MH_ERR_INVALID_ACTION_PARAM ActionParameter error
   * @retval MH_ERR_FAILURE_AGENT Error of DRM Agent
   * @retval MH_ERR_FAILURE Error by other reasons
   */
  MH_status_t createChallengeRequest(MH_iptvesHandle_t i_handle, MH_challengeParameter_t* i_parameter, MH_buffer_t* o_request);

  /**
   * @brief Process http response.
   *
   * @param [in] i_handle Marlin IPTV-ES license handle
   * @param [in] i_response Http response message dagta.
   * @param [in] i_parameter Initialization data of acquisition process. \n
   * i_parameter is same format as createChallengeRequest(). There is also a case of NULL.
   * @param [out] o_endflag flag whether step remained
   * @param [out] o_request Http request message data. only set when continue acquisitions
   *
   * @retval MH_ERR_OK Processing Response is success
   * @retval MH_ERR_INVALID_RESPONSE_MSG Response message error
   * @retval MH_ERR_FAILURE_AGENT Error of DRM Agent
   * @retval MH_ERR_FAILURE Error by other reasons
   */
  MH_status_t processResponse(MH_iptvesHandle_t i_handle, MH_buffer_t* i_response, MH_challengeParameter_t* i_parameter, bool* o_endflag, MH_buffer_t* o_request);

  /**
   * @brief Free the Http request message buffer.( allocated by createChallengeRequest() or processResponse() )
   *
   * @param [in] i_handle Marlin IPTV-ES license handle
   *
   * @retval MH_ERR_OK Free the buffer is success
   * @retval MH_ERR_FAILURE Cannot free the buffer
   */
  MH_status_t freeRequestBuffer(MH_iptvesHandle_t i_handle);

  /**
   * @brief Canceling session linked request information.
   *
   * @param [in] i_handle Marlin IPTV-ES license handle
   *
   * @retval MH_ERR_OK Canceling request is success
   * @retval MH_ERR_FAILURE Cannot canceling request
   */
  MH_status_t cancelKeyRequest(MH_iptvesHandle_t i_handle);

  /**
   * @brief Decryption of media content.
   *
   * @param [in] i_parameter includes KeyID information(PSSH information or ECM information).\n
   * @param [in] i_src_ptr Input buffer of encrypted data
   * @param [out] o_dst_ptr Output buffer of decrypted data
   *
   * @retval MH_ERR_OK Decryption is success
   * @retval MH_ERR_TOO_SMALL_BUFFER Out buffer is too small
   * @retval MH_ERR_FAILURE Cannot decrypt content
   */
  MH_status_t decrypt(MH_keyIdInfo_t* i_parameter,
                      MH_buffer_t* i_src_ptr,
                      MH_buffer_t* o_dst_ptr);


protected:

private:

};
}; //namespace

#endif /* __MARLIN_AGENT_HANDLER_H__ */


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
