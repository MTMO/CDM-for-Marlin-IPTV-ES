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

#ifndef MARLIN_COMMON_TYPE_H_
#define MARLIN_COMMON_TYPE_H_

#include <string>
#include <cstring>
#include <stdint.h>

/* Size of Initialization data */
#define MCDM_SIZE_REQ_TYPE                   1
#define MCDM_SIZE_ACT_ID                     1
#define MCDM_SIZE_ACT_PARAM                  1
#define MCDM_SIZE_PRIVATE_DATA               28
#define MCDM_SIZE_URR_DATA                   16
#define MCDM_SIZE_SERVER_URI_LEN             4
#define MCDM_SIZE_KID_INFO_TYPE              1
#define MCDM_SIZE_KID_INFO_LEN               4

/* Byte index of Initialization data for CheckKeyExist()/Decrypt() */
#define MCDM_INDEX_KID_INFO_TYPE             0
#define MCDM_INDEX_KID_INFO_LEN              (MCDM_INDEX_KID_INFO_TYPE + MCDM_SIZE_KID_INFO_TYPE)
#define MCDM_INDEX_KID_INFO_DATA             (MCDM_INDEX_KID_INFO_LEN  + MCDM_SIZE_KID_INFO_LEN)

/* Byte index of Initialization data for GenerateKeyRequest()/AddKey() */
#define MCDM_INDEX_REQ_TYPE                   0
#define MCDM_INDEX_ACT_ID                     (MCDM_INDEX_REQ_TYPE                   + MCDM_SIZE_REQ_TYPE)
#define MCDM_INDEX_ACT_PARAM                  (MCDM_INDEX_ACT_ID                     + MCDM_SIZE_ACT_ID)
#define MCDM_INDEX_PRIVATE_DATA               (MCDM_INDEX_ACT_PARAM                  + MCDM_SIZE_ACT_PARAM)
#define MCDM_INDEX_URR_DATA                   (MCDM_INDEX_PRIVATE_DATA               + MCDM_SIZE_PRIVATE_DATA)
#define MCDM_INDEX_SERVER_URI_LEN             (MCDM_INDEX_URR_DATA                   + MCDM_SIZE_URR_DATA)
#define MCDM_INDEX_SERVER_URI_DATA            (MCDM_INDEX_SERVER_URI_LEN             + MCDM_SIZE_SERVER_URI_LEN)
#define MCDM_INDEX_KID_INFO_TYPE_EXT(uri_len) (MCDM_INDEX_SERVER_URI_DATA            + uri_len)
#define MCDM_INDEX_KID_INFO_LEN_EXT(uri_len)  (MCDM_INDEX_KID_INFO_TYPE_EXT(uri_len) + MCDM_SIZE_KID_INFO_TYPE)
#define MCDM_INDEX_KID_INFO_DATA_EXT(uri_len) (MCDM_INDEX_KID_INFO_LEN_EXT(uri_len)  + MCDM_SIZE_KID_INFO_LEN)

/* keyIdInfoType of a Initialization data */
#define MCDM_KID_INFO_TYPE_NONE              0x00
#define MCDM_KID_INFO_TYPE_PSSH              0x01
#define MCDM_KID_INFO_TYPE_ECM               0x02

/* RequestType of a Initialization data */
#define MCDM_REQ_TYPE_NONE                   0x00
#define MCDM_REQ_TYPE_PERMISSION             0x01
#define MCDM_REQ_TYPE_TRUSTED_TIME           0x02

/* ActionId of a Initialization data */
#define MCDM_ACT_ID_NONE                     0x00
#define MCDM_ACT_ID_EXT_SIMPLE_KEY           0x01
#define MCDM_ACT_ID_EXT_INDIRECT_KEY         0x02
#define MCDM_ACT_ID_EXT_DIRECT_KEY           0x03
#define MCDM_ACT_ID_EXP_COPY_DIRECT_KEY      0x04
#define MCDM_ACT_ID_EXP_MOVE_DIRECT_KEY      0x05
#define MCDM_ACT_ID_EXP_INDIRECT_KEY         0x06
#define MCDM_ACT_ID_REC_INDIRECT_KEY         0x07

/* ActionParameter of a Initialization data */
#define MCDM_ACT_PARAM_NONE                  0x00
#define MCDM_ACT_PARAM_DTCP                  0x01
#define MCDM_ACT_PARAM_CPRM_DVD              0x02
#define MCDM_ACT_PARAM_CPRM_SD_VIDEO_093     0x03
#define MCDM_ACT_PARAM_CPRM_SD_AUDIO         0x04
#define MCDM_ACT_PARAM_MGR_MEMORY_STICK_PRO  0x05
#define MCDM_ACT_PARAM_MGR_MEMORY_STICK      0x06
#define MCDM_ACT_PARAM_VCPS                  0x07
#define MCDM_ACT_PARAM_MGR_EMPR              0x08
#define MCDM_ACT_PARAM_MGR_ATRAC             0x09
#define MCDM_ACT_PARAM_SAFIA_IVDR_TV         0x0a
#define MCDM_ACT_PARAM_SAFIA_IVDR_AUDIO      0x0b
#define MCDM_ACT_PARAM_AACS_BDR_092          0x0c
#define MCDM_ACT_PARAM_AACS_RLM_092          0x0d
#define MCDM_ACT_PARAM_CPRM_SD_VIDEO_096     0x0e
#define MCDM_ACT_PARAM_AACS_BDR_095          0x0f
#define MCDM_ACT_PARAM_AACS_RLM_095          0x10
#define MCDM_ACT_PARAM_NSM_CPS               0x11

#define MCDM_GET_LEN(a)  ((((*(a) & 0xFF) << 24) + ((*(a+1) & 0xFF) << 16) + ((*(a+2) & 0xFF) << 8) + (*(a+3) & 0xFF)) & 0xFFFFFFFF)

using namespace std;

namespace marlincdm {

/**
 * @brief Unique string to identify Marlin CDM object
 */
typedef string mcdm_session_id_t;

/**
 * @brief This structure includes data length and data buffer and fd
 */
struct mcdm_buffer_t {
    size_t len; //!< data size
    uint8_t *data; //!< data buffer
    int fd; //!< File descriptor
};

/**
 * @brief This structure includes keyRelease information.
 */
struct mcdm_key_release_t {
    mcdm_session_id_t session_id; //!< session id
    mcdm_buffer_t msg_buf; //!< Key release message data buffer
};

} // namespace marlincdm

#endif  // MARLIN_COMMON_TYPE_H_


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
