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

#ifndef __SESSION_MANAGER_H__
#define __SESSION_MANAGER_H__

#include "CMutex.h"
#include "MarlinCommonTypes.h"
#include "MarlinError.h"

#define MCDM_SESSION_ID_INIT 1

namespace marlincdm {

class CdmSessionManager {
private:
    static CdmSessionManager *sInstance;
    static CMutex sMutex;

    long long mSessionId;
    CMutex mSessionIdMutex;
    CdmSessionManager(const CdmSessionManager &o);
    CdmSessionManager& operator=(const CdmSessionManager &o);

protected:
    CdmSessionManager();
    virtual ~CdmSessionManager();

public:
    static inline CdmSessionManager* getCdmSessionManager() {
        sMutex.lock();
        CdmSessionManager *instance = sInstance;
        if (instance == NULL) {
            instance = new CdmSessionManager();
            if (instance == NULL) {
                LOGE("ERROR : Could not allocate instance of CdmSessionManager.\n");
            }
            sInstance = instance;
        }
        sMutex.unlock();
        return instance;
    }

    mcdm_status_t getCdmSessionId(mcdm_session_id_t &sessionId);

};  //class
};  //namespace

#endif /* __SESSION_MANAGER_H__ */


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
