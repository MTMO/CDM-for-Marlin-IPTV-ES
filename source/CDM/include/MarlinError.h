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

#ifndef __MARLIN_ERROR_H__
#define __MARLIN_ERROR_H__

#include <string>

namespace marlincdm {
/**
 * @brief Marlin CDM Error Type
 */
enum mcdm_status_t {
    OK = 0,  //!< process is success
    ERROR_UNKNOWN,  //!< Error by other reasons
    ERROR_ILLEGAL_ARGUMENT,  //!< Invalid parameter
    ERROR_SESSION_NOT_OPENED,  //!< Session is discarded
};

} // marlincdm

#endif /* __MARLIN_ERROR_H__ */


/*
 * 2015 - Copyright Marlin Trust Management Organization
 */
