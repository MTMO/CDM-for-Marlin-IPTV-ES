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

#include <sys/stat.h>
#include <cstring>
#include <map>

#define LOG_TAG "MarlinCdmEngine"
#include "MarlinLog.h"

#include "MarlinCdmEngine.h"
#include "CdmSessionManager.h"

using namespace marlincdm;

// singleton instance and lock
namespace {
    MarlinCdmEngine *sInstance = NULL;
    CMutex sMutex;
    MarlinAgentHandler* mHandler = NULL;
    MH_agentHandle_t mHandle = NULL;
    map<mcdm_session_id_t, MH_iptvesHandle_t> mCdmSessionMap;
}

MarlinCdmEngine::MarlinCdmEngine()
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;
    MH_agentHandle_t handle = NULL;

    mHandler = new MarlinAgentHandler();
    if (mHandler == NULL) {
        LOGE("ERROR : Could not allocate instance of MarlinAgentHandler.\n");
    } else {
        agentStatus = mHandler->initAgent(&handle);
        if (agentStatus != MH_ERR_OK) {
            LOGE("ERROR : calling initAgent (%d).\n", agentStatus);
        }
        mHandle = handle;
    }

    MARLINLOG_EXIT();
}  /* pgr0840 : release in ~MarlinCdmEngine(). */

MarlinCdmEngine::~MarlinCdmEngine()
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;

    if (mHandler != NULL) {
        if(mHandle != NULL) {
            agentStatus = mHandler->finAgent(mHandle);
            if (agentStatus != MH_ERR_OK) {
                LOGE("ERROR : calling finAgent (%d).\n", agentStatus);
            }
        }
        delete mHandler;
        mHandler = NULL;
    }

    MARLINLOG_EXIT();
}

mcdm_status_t MarlinCdmEngine::CheckKeyExist(const mcdm_buffer_t& init_data, bool* is_key_exist)
{
    MARLINLOG_ENTER();

    mcdm_status_t status = OK;
    MH_status_t agentStatus = MH_ERR_OK;
    MH_keyIdInfo_t kid_info;

    memset(&kid_info, 0, sizeof(MH_keyIdInfo_t));

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (is_key_exist == NULL) {
        LOGE("ERROR : Output parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    if (init_data.data == NULL) {
        LOGE("ERROR : Input parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    status = parseInitDataForKeyIdInfo(init_data, kid_info);
    if (status != OK) {
        LOGE("ERROR : calling parseInitDataForKeyIdInfo.\n");
        delete [] kid_info.data;
        MARLINLOG_EXIT();
        return status;
    }

    agentStatus = mHandler->checkKeyExist(&kid_info, is_key_exist);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling checkKeyExist (%d).\n", agentStatus);
        delete [] kid_info.data;
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    delete [] kid_info.data;

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::OpenSession(mcdm_session_id_t& session_id)
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;
    MH_iptvesHandle_t iptves_handle = NULL;
    CdmSessionManager* sm;

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    sm = CdmSessionManager::getCdmSessionManager();
    if (sm == NULL) {
        LOGE("ERROR : CdmSessionManager is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (sm->getCdmSessionId(session_id) != OK) {
        LOGE("ERROR : Could not create session id.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mCdmSessionMap.count(session_id) != 0) {
        LOGE("ERROR : invalid session id.\n");
        session_id = "";
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    agentStatus = mHandler->initIPTVESHandle(mHandle, session_id, &iptves_handle);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling initIPTVESHandle (%d).\n", agentStatus);
        session_id = "";
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    mCdmSessionMap[session_id] = iptves_handle;

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::CloseSession(const mcdm_session_id_t& session_id)
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;
    MH_iptvesHandle_t handle = NULL;

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    handle = getIPTVEShandle(session_id);
    if (handle == NULL) {
        LOGE("ERROR : calling getIPTVEShandle. session_id(%s).\n", session_id.c_str());
        MARLINLOG_EXIT();
        return ERROR_SESSION_NOT_OPENED;
    }

    agentStatus = mHandler->finIPTVESHandle(handle);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling finIPTVESHandle (%d).\n", agentStatus);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    mCdmSessionMap.erase(session_id);

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::GenerateKeyRequest(const mcdm_session_id_t& session_id,
                                                  const mcdm_buffer_t& init_data,
                                                  mcdm_buffer_t* request)
{
    MARLINLOG_ENTER();

    mcdm_status_t status = OK;
    MH_status_t agentStatus = MH_ERR_OK;
    MH_iptvesHandle_t handle = NULL;
    MH_challengeParameter_t mh_chal_param;
    MH_buffer_t mh_request;

    memset(&mh_chal_param, 0, sizeof(MH_challengeParameter_t));
    memset(&mh_request, 0, sizeof(MH_buffer_t));

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (request == NULL) {
        LOGE("ERROR : Output parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    if (init_data.data == NULL) {
        LOGE("ERROR : Input parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    handle = getIPTVEShandle(session_id);
    if (handle == NULL) {
        LOGE("ERROR : calling getIPTVEShandle. session_id(%s).\n", session_id.c_str());
        MARLINLOG_EXIT();
        return ERROR_SESSION_NOT_OPENED;
    }

    status = parseInitDataForChallengeParameter(init_data, mh_chal_param);
    if (status != OK) {
        LOGE("ERROR : calling parseInitDataForChallengeParameter.\n");
        delete [] mh_chal_param.server_uri_data;
        delete [] mh_chal_param.kid_info.data;
        MARLINLOG_EXIT();
        return status;
    }

    agentStatus = mHandler->createChallengeRequest(handle,
                                                   &mh_chal_param,
                                                   &mh_request);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling createChallengeRequest (%d).\n", agentStatus);
        mHandler->freeRequestBuffer(handle);
        delete [] mh_chal_param.server_uri_data;
        delete [] mh_chal_param.kid_info.data;
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    request->len = mh_request.len;
    request->data = mh_request.data;
    request->fd = mh_request.fd;

    delete [] mh_chal_param.server_uri_data;
    delete [] mh_chal_param.kid_info.data;

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::AddKey(const mcdm_session_id_t& session_id,
                                      const mcdm_buffer_t& key,
                                      const mcdm_buffer_t& init_data,
                                      bool* endflag,
                                      mcdm_buffer_t* request)
{
    MARLINLOG_ENTER();

    mcdm_status_t status = OK;
    MH_status_t agentStatus = MH_ERR_OK;
    MH_iptvesHandle_t handle = NULL;
    MH_buffer_t mh_response;
    MH_buffer_t mh_request;
    MH_challengeParameter_t mh_chal_param;
    MH_challengeParameter_t* mh_chal_param_p = NULL;

    memset(&mh_response, 0, sizeof(MH_buffer_t));
    memset(&mh_request, 0, sizeof(MH_buffer_t));
    memset(&mh_chal_param, 0, sizeof(MH_challengeParameter_t));

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if ((request == NULL) || (endflag == NULL)) {
        LOGE("ERROR : Output parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    mh_response.len = key.len;
    mh_response.data = key.data;
    mh_response.fd = key.fd;

    handle = getIPTVEShandle(session_id);
    if (handle == NULL) {
        LOGE("ERROR : calling getIPTVEShandle. session_id(%s).\n", session_id.c_str());
        MARLINLOG_EXIT();
        return ERROR_SESSION_NOT_OPENED;
    }

    agentStatus = mHandler->freeRequestBuffer(handle);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling freeRequestBuffer (%d).\n", agentStatus);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (init_data.data != NULL) {
        status = parseInitDataForChallengeParameter(init_data, mh_chal_param);
        if (status != OK) {
            LOGE("ERROR : calling parseInitDataForChallengeParameter.\n");
            delete [] mh_chal_param.server_uri_data;
            delete [] mh_chal_param.kid_info.data;
            MARLINLOG_EXIT();
            return status;
        }
        mh_chal_param_p = &mh_chal_param;
    }

    agentStatus = mHandler->processResponse(handle,
                                            &mh_response,
                                            mh_chal_param_p,
                                            endflag,
                                            &mh_request);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling processResponse (%d).\n", agentStatus);
        mHandler->freeRequestBuffer(handle);
        delete [] mh_chal_param.server_uri_data;
        delete [] mh_chal_param.kid_info.data;
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    request->len = mh_request.len;
    request->data = mh_request.data;
    request->fd = mh_request.fd;

    delete [] mh_chal_param.server_uri_data;
    delete [] mh_chal_param.kid_info.data;

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::CancelKeyRequest(const mcdm_session_id_t& session_id)
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;
    MH_iptvesHandle_t handle = NULL;

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    handle = getIPTVEShandle(session_id);
    if (handle == NULL) {
        LOGE("ERROR : calling getIPTVEShandle. session_id(%s).\n", session_id.c_str());
        MARLINLOG_EXIT();
        return ERROR_SESSION_NOT_OPENED;
    }

    agentStatus = mHandler->freeRequestBuffer(handle);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling freeRequestBuffer (%d).\n", agentStatus);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    agentStatus = mHandler->cancelKeyRequest(handle);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling cancelKeyRequest (%d).\n", agentStatus);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::Decrypt(const mcdm_buffer_t& init_data,
                                       mcdm_buffer_t* src_ptr,
                                       mcdm_buffer_t* dst_ptr)
{
    MARLINLOG_ENTER();

    mcdm_status_t status = OK;
    MH_status_t agentStatus = MH_ERR_OK;
    MH_buffer_t mh_src_ptr;
    MH_buffer_t mh_dst_ptr;
    MH_keyIdInfo_t kid_info;

    memset(&mh_src_ptr, 0, sizeof(MH_buffer_t));
    memset(&mh_dst_ptr, 0, sizeof(MH_buffer_t));
    memset(&kid_info, 0, sizeof(MH_keyIdInfo_t));

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (dst_ptr == NULL) {
        LOGE("ERROR : Output parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    if ((init_data.data == NULL) || (src_ptr == NULL)) {
        LOGE("ERROR : Input parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    status = parseInitDataForKeyIdInfo(init_data, kid_info);
    if (status != OK) {
        LOGE("ERROR : calling parseInitDataForKeyIdInfo.\n");
        delete [] kid_info.data;
        MARLINLOG_EXIT();
        return status;
    }

    mh_src_ptr.len = src_ptr->len;
    mh_src_ptr.data = src_ptr->data;
    mh_src_ptr.fd = src_ptr->fd;

    agentStatus = mHandler->decrypt(&kid_info,
                                    &mh_src_ptr,
                                    &mh_dst_ptr);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling decrypt (%d).\n", agentStatus);
        delete [] kid_info.data;
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    dst_ptr->len = mh_dst_ptr.len;
    dst_ptr->data = mh_dst_ptr.data;
    dst_ptr->fd = mh_dst_ptr.fd;

    delete [] kid_info.data;

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::GetKeyReleases(mcdm_key_release_t** key_release,
                                              uint32_t* key_release_num)
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;
    MH_keyRelease_t* mh_key_release = NULL;

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if ((key_release == NULL) || (key_release_num == NULL)) {
        LOGE("ERROR : Output parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    agentStatus = mHandler->getKeyReleases(&mh_key_release, key_release_num);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling getKeyReleases (%d).\n", agentStatus);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    *key_release = (mcdm_key_release_t*)mh_key_release;

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::AddKeyReleaseCommit(const mcdm_key_release_t& key_release)
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    agentStatus = mHandler->addKeyReleaseCommit((MH_keyRelease_t*)&key_release);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling addKeyReleaseCommit (%d).\n", agentStatus);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::FreeKeyReleasesBuffer(mcdm_key_release_t* key_release,
                                                     uint32_t key_release_num)
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;
    MH_keyRelease_t* mh_key_release = (MH_keyRelease_t*)key_release;

    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (key_release == NULL) {
        LOGE("ERROR : Input parameter is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_ILLEGAL_ARGUMENT;
    }

    agentStatus = mHandler->freeKeyReleasesBuffer(mh_key_release, key_release_num);
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling freeKeyReleasesBuffer (%d).\n", agentStatus);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    MARLINLOG_EXIT();
    return OK;
}

MH_iptvesHandle_t MarlinCdmEngine::getIPTVEShandle(const mcdm_session_id_t& session_id)
{
    MARLINLOG_ENTER();

    MH_iptvesHandle_t handle = NULL;

    if (mCdmSessionMap.count(session_id) == 0) {
        LOGD("ERROR : invalid session id.\n");
    } else {
        handle = mCdmSessionMap[session_id];
    }

    MARLINLOG_EXIT();
    return handle;
}

mcdm_status_t MarlinCdmEngine::parseInitDataForKeyIdInfo(const mcdm_buffer_t& init_data, MH_keyIdInfo_t& kid_info)
{
    MARLINLOG_ENTER();

    /* KeyID information Type */
    kid_info.type = (MH_keyIdInfoType)init_data.data[MCDM_INDEX_KID_INFO_TYPE];

    /* KeyID information length */
    kid_info.length = (size_t)MCDM_GET_LEN(&init_data.data[MCDM_INDEX_KID_INFO_LEN]);

    if (kid_info.length > 0) {
        /* KeyID information data */
        kid_info.data = new uint8_t[kid_info.length];
        if(kid_info.data == NULL) {
            LOGE("ERROR : Could not allocate memory.\n");
            MARLINLOG_EXIT();
            return ERROR_UNKNOWN;
        }
        memcpy(kid_info.data, &init_data.data[MCDM_INDEX_KID_INFO_DATA], kid_info.length);
    }

    MARLINLOG_EXIT();
    return OK;
}

mcdm_status_t MarlinCdmEngine::parseInitDataForChallengeParameter(const mcdm_buffer_t& init_data, MH_challengeParameter_t& chal_param)
{
    MARLINLOG_ENTER();

    /* RequestType */
    chal_param.req_type = (MH_requestType)init_data.data[MCDM_INDEX_REQ_TYPE];

    /* ActionID */
    chal_param.action_id = (MH_actionId)init_data.data[MCDM_INDEX_ACT_ID];

    /* ActionParameter */
    chal_param.act_param = (MH_actionParam)init_data.data[MCDM_INDEX_ACT_PARAM];

    /* PrivateDataTag & PrivateData */
    memcpy(chal_param.private_data, &init_data.data[MCDM_INDEX_PRIVATE_DATA], MCDM_SIZE_PRIVATE_DATA);

    /* UsageRuleReference */
    memcpy(chal_param.urr_data, &init_data.data[MCDM_INDEX_URR_DATA], MCDM_SIZE_URR_DATA);

    /* DRMServerURI length */
    chal_param.server_uri_length = 
        (size_t)MCDM_GET_LEN(&init_data.data[MCDM_INDEX_SERVER_URI_LEN]);

    if (chal_param.server_uri_length > 0) {
        /* DRMServerURI data */
        chal_param.server_uri_data = new uint8_t[chal_param.server_uri_length];
        if(chal_param.server_uri_data == NULL) {
            LOGE("ERROR : Could not allocate memory.\n");
            MARLINLOG_EXIT();
            return ERROR_UNKNOWN;
        }
        memcpy(chal_param.server_uri_data, 
                &init_data.data[MCDM_INDEX_SERVER_URI_DATA],
                chal_param.server_uri_length);
    }

    /* KeyID information Type */
    chal_param.kid_info.type = 
        (MH_keyIdInfoType)init_data.data[MCDM_INDEX_KID_INFO_TYPE_EXT(chal_param.server_uri_length)];

    /* KeyID information length */
    chal_param.kid_info.length = 
        (size_t)MCDM_GET_LEN(&init_data.data[MCDM_INDEX_KID_INFO_LEN_EXT(chal_param.server_uri_length)]);

    if (chal_param.kid_info.length > 0) {
        /* KeyID information data */
        chal_param.kid_info.data = new uint8_t[chal_param.kid_info.length];
        if(chal_param.kid_info.data == NULL) {
            LOGE("ERROR : Could not allocate memory.\n");
            MARLINLOG_EXIT();
            return ERROR_UNKNOWN;
        }
        memcpy(chal_param.kid_info.data, 
                &init_data.data[MCDM_INDEX_KID_INFO_DATA_EXT(chal_param.server_uri_length)], 
                chal_param.kid_info.length);
    }

    MARLINLOG_EXIT();
    return OK;
}

MarlinCdmEngine* MarlinCdmEngine::getMarlinCdmEngine()
{
    MH_status_t agentStatus = MH_ERR_OK;

    sMutex.lock();
    MarlinCdmEngine *instance = sInstance;
    if (instance == NULL) {
        instance = new MarlinCdmEngine();
        if (instance == NULL) {
            LOGE("ERROR : Could not allocate instance of MarlinCdmEngine.\n");
        }
        sInstance = instance;
    }
    if (mHandle != NULL) {
        agentStatus = mHandler->increaseRefCount();
        if (agentStatus != MH_ERR_OK) {
            LOGE("ERROR : calling increaseRefCount (%d).\n", agentStatus);
        }
    }
    sMutex.unlock();
    return instance;  /* pgr0840 : For singleton, and will not be released. */
}

mcdm_status_t MarlinCdmEngine::releaseMarlinCdmEngine(bool &end_flag)
{
    MARLINLOG_ENTER();

    MH_status_t agentStatus = MH_ERR_OK;

    sMutex.lock();
    if (mHandler == NULL) {
        LOGE("ERROR : MarlinAgentHandler is NULL.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandle == NULL) {
        LOGE("ERROR : This function is called in the wrong sequence.\n");
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    agentStatus = mHandler->decreaseRefCount();
    if (agentStatus != MH_ERR_OK) {
        LOGE("ERROR : calling increaseRefCount (%d).\n", agentStatus);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mHandler->getRefCount() == 0) {
        MarlinCdmEngine *instance = sInstance;
        sInstance = NULL;
        sMutex.unlock();
        delete instance;
        end_flag = true;
    } else {
        sMutex.unlock();
        end_flag = false;
    }

    sMutex.unlock();

    MARLINLOG_EXIT();
    return OK;
}


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
