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

#define LOG_TAG "MarlinCdmInterface"
#include "MarlinLog.h"

#include "MarlinCdmInterface.h"
#include "MarlinCdmEngine.h"

using namespace marlincdm;

// singleton instance, lock and engine instance
namespace {
    MarlinCdmInterface *sInstance = NULL;
    CMutex sMutex;
    MarlinCdmEngine *sEngine = NULL;
}

MarlinCdmInterface::MarlinCdmInterface()
{
    sEngine = MarlinCdmEngine::getMarlinCdmEngine();
    if (sEngine == NULL) {
        LOGE("ERROR : Could not get instance of MarlinCdmEngine.\n");
    }
}

MarlinCdmInterface::~MarlinCdmInterface()
{
}

mcdm_status_t MarlinCdmInterface::CheckKeyExist(const mcdm_buffer_t& init_data, bool* is_key_exist)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->CheckKeyExist(init_data, is_key_exist);
}

mcdm_status_t MarlinCdmInterface::OpenSession(mcdm_session_id_t& session_id)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->OpenSession(session_id);
}

mcdm_status_t MarlinCdmInterface::CloseSession(const mcdm_session_id_t& session_id)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->CloseSession(session_id);
}

mcdm_status_t MarlinCdmInterface::GenerateKeyRequest(const mcdm_session_id_t& session_id,
                                                     const mcdm_buffer_t& init_data,
                                                     mcdm_buffer_t* request)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->GenerateKeyRequest(session_id,
                                                 init_data,
                                                 request);
}

mcdm_status_t MarlinCdmInterface::AddKey(const mcdm_session_id_t& session_id,
                                         const mcdm_buffer_t& key,
                                         const mcdm_buffer_t& init_data,
                                         bool* endflag,
                                         mcdm_buffer_t* request)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->AddKey(session_id,
                                     key,
                                     init_data,
                                     endflag,
                                     request);
}

mcdm_status_t MarlinCdmInterface::CancelKeyRequest(const mcdm_session_id_t& session_id)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->CancelKeyRequest(session_id);
}

mcdm_status_t MarlinCdmInterface::Decrypt(const mcdm_buffer_t& init_data,
                                          mcdm_buffer_t* src_ptr,
                                          mcdm_buffer_t* dst_ptr)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->Decrypt(init_data,
                                      src_ptr,
                                      dst_ptr);
}

mcdm_status_t MarlinCdmInterface::GetKeyReleases(mcdm_key_release_t** key_release,
                                                 uint32_t* key_release_num)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->GetKeyReleases(key_release,
                                             key_release_num);
}

mcdm_status_t MarlinCdmInterface::AddKeyReleaseCommit(const mcdm_key_release_t& key_release)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->AddKeyReleaseCommit(key_release);
}

mcdm_status_t MarlinCdmInterface::FreeKeyReleasesBuffer(mcdm_key_release_t* key_release,
                                                        uint32_t key_release_num)
{
    if (sEngine == NULL) {
        LOGE("ERROR : instance of MarlinCdmEngine is NULL.\n");
        return ERROR_UNKNOWN;
    }
    return sEngine->FreeKeyReleasesBuffer(key_release, key_release_num);
}

MarlinCdmInterface *MarlinCdmInterface::getMarlinCdmInterface()
{
    MARLINLOG_ENTER();
    sMutex.lock();
    MarlinCdmInterface *instance = sInstance;
    if (instance == NULL) {
        instance = new MarlinCdmInterface();
        if (instance == NULL) {
            LOGE("ERROR : Could not allocate instance of MarlinCdmInterface.\n");
        }
        sInstance = instance;
    }
    sMutex.unlock();

    MARLINLOG_EXIT();
    return instance;  /* pgr0840 : For singleton, and will not be released. */
}

mcdm_status_t MarlinCdmInterface::releaseMarlinCdmInterface()
{
    MARLINLOG_ENTER();

    mcdm_status_t enginRetVal;
    bool endFlag;

    sMutex.lock();
    enginRetVal = sEngine->releaseMarlinCdmEngine(endFlag);
    if (enginRetVal != OK) {
        LOGE("ERROR : releaseMarlinCdmEngine return error.\n");
        sMutex.unlock();
        return enginRetVal;
    }

    if (endFlag == true) {
        MarlinCdmInterface *instance = sInstance;
        sInstance = NULL;
        sMutex.unlock();
        delete instance;
    } else {
        sMutex.unlock();
    }

    MARLINLOG_EXIT();
    return OK;
}


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
