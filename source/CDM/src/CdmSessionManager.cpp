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
#include <sstream>

#define LOG_TAG "CdmSessionManager"
#include "MarlinLog.h"

#include <limits.h>
#include "CdmSessionManager.h"

using namespace marlincdm;

// singleton instance and lock
CdmSessionManager *CdmSessionManager::sInstance = NULL;
CMutex CdmSessionManager::sMutex;

CdmSessionManager::CdmSessionManager() : mSessionId(MCDM_SESSION_ID_INIT)
{
    MARLINLOG_ENTER();
}

CdmSessionManager::~CdmSessionManager()
{
    MARLINLOG_ENTER();
}

mcdm_status_t CdmSessionManager::getCdmSessionId(mcdm_session_id_t &sessionId)
{
    MARLINLOG_ENTER();

    ostringstream stream;

    mSessionIdMutex.lock();
    if (mSessionId < 0) {
        //error;
        LOGV("Error: Session ID: [%lld]\n", mSessionId);
        MARLINLOG_EXIT();
        return ERROR_UNKNOWN;
    }

    if (mSessionId >= LLONG_MAX) {
        mSessionId = MCDM_SESSION_ID_INIT;
    }
    stream << mSessionId;
    sessionId =  stream.str();
    LOGV("Session ID: str[%s] [%lld]\n",sessionId.c_str(), mSessionId);
    mSessionId++;
    mSessionIdMutex.unlock();

    MARLINLOG_EXIT();
    return OK;
}


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
