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

#include "MarlinAgentHandler.h"


using namespace marlincdm;

MarlinAgentHandler::MarlinAgentHandler()
{
    /* Add marlin agent specific call if needed */
}

MarlinAgentHandler::~MarlinAgentHandler()
{
    /* Add marlin agent specific call if needed */
}

uint32_t MarlinAgentHandler::getRefCount(void)
{

    /* Add marlin agent specific call if needed */

    return 1;
}

MH_status_t MarlinAgentHandler::increaseRefCount(void)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::decreaseRefCount(void)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::getKeyReleases(MH_keyRelease_t** o_key_release,
                                               uint32_t* o_key_release_num)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::addKeyReleaseCommit(MH_keyRelease_t* i_key_release)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::freeKeyReleasesBuffer(MH_keyRelease_t* i_key_release, uint32_t i_key_release_num)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::initAgent(MH_agentHandle_t* o_handle)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::finAgent(MH_agentHandle_t i_handle)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::checkKeyExist(MH_keyIdInfo_t* i_parameter, bool* o_is_key_exist)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::initIPTVESHandle(MH_agentHandle_t i_handle,
                                                 MH_session_id_t i_session_id,
                                                 MH_iptvesHandle_t* o_handle)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::finIPTVESHandle(MH_iptvesHandle_t i_handle)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::createChallengeRequest(MH_iptvesHandle_t i_handle,
                                                       MH_challengeParameter_t* i_parameter,
                                                       MH_buffer_t* o_request)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::processResponse(MH_iptvesHandle_t i_handle,
                                                MH_buffer_t* i_response,
                                                MH_challengeParameter_t* i_parameter,
                                                bool* o_endflag,
                                                MH_buffer_t* o_request)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::freeRequestBuffer(MH_iptvesHandle_t i_handle)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::cancelKeyRequest(MH_iptvesHandle_t i_handle)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}

MH_status_t MarlinAgentHandler::decrypt(MH_keyIdInfo_t* i_parameter,
                                        MH_buffer_t* i_src_ptr,
                                        MH_buffer_t* o_dst_ptr)
{
    MH_status_t retCode = MH_ERR_OK;

    /* Add marlin agent specific call if needed */

    return retCode;
}



/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
