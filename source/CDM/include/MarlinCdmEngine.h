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

#ifndef MARLIN_CDM_ENGINE_H_
#define MARLIN_CDM_ENGINE_H_

#include "MarlinError.h"
#include "MarlinCommonTypes.h"
#include "MarlinAgentHandler.h"
#include "CMutex.h"

namespace marlincdm {

using namespace std;

class MarlinCdmEngine {
 public:

  mcdm_status_t CheckKeyExist(const mcdm_buffer_t& init_data, bool* is_key_exist);

  mcdm_status_t OpenSession(mcdm_session_id_t& session_id);

  mcdm_status_t CloseSession(const mcdm_session_id_t& session_id);

  mcdm_status_t GenerateKeyRequest(const mcdm_session_id_t& session_id,
                                   const mcdm_buffer_t& init_data,
                                   mcdm_buffer_t* request);

  mcdm_status_t AddKey(const mcdm_session_id_t& session_id,
                       const mcdm_buffer_t& key,
                       const mcdm_buffer_t& init_data,
                       bool* endflag,
                       mcdm_buffer_t* request);

  mcdm_status_t CancelKeyRequest(const mcdm_session_id_t& session_id);

  mcdm_status_t Decrypt(const mcdm_buffer_t& init_data,
                        mcdm_buffer_t* src_ptr,
                        mcdm_buffer_t* dst_ptr);

  mcdm_status_t GetKeyReleases(mcdm_key_release_t** key_release,
                               uint32_t* key_release_num);

  mcdm_status_t AddKeyReleaseCommit(const mcdm_key_release_t& key_release);

  mcdm_status_t FreeKeyReleasesBuffer(mcdm_key_release_t* key_release,
                                      uint32_t key_release_num);

  static MarlinCdmEngine* getMarlinCdmEngine();

  static mcdm_status_t releaseMarlinCdmEngine(bool &end_flag);

 private:

  MarlinCdmEngine();
  virtual ~MarlinCdmEngine();

  MarlinCdmEngine(const MarlinCdmEngine &o);
  MarlinCdmEngine& operator=(const MarlinCdmEngine &o);

  MH_iptvesHandle_t getIPTVEShandle(const mcdm_session_id_t& session_id);
  mcdm_status_t parseInitDataForKeyIdInfo(const mcdm_buffer_t& init_data, MH_keyIdInfo_t& kid_info);
  mcdm_status_t parseInitDataForChallengeParameter(const mcdm_buffer_t& init_data, MH_challengeParameter_t& chal_param);

};

} // namespace marlincdm

#endif  // MARLIN_CDM_ENGINE_H_


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
