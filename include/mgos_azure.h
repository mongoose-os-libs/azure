/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CS_MOS_LIBS_AZURE_INCLUDE_MGOS_AZURE_H_
#define CS_MOS_LIBS_AZURE_INCLUDE_MGOS_AZURE_H_

#include <stdint.h>

#include "common/mg_str.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Generate a SAS token as described here:
 * https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-devguide-security#security-tokens
 */
struct mg_str mgos_azure_gen_sas_token(const struct mg_str uri,
                                       const struct mg_str key, uint64_t se);

#ifdef __cplusplus
}
#endif

#endif /* CS_MOS_LIBS_AZURE_INCLUDE_MGOS_AZURE_H_ */
