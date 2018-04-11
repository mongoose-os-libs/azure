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

#include "mgos_event.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MGOS_AZURE_EVENT_BASE MGOS_EVENT_BASE('A', 'Z', 'R')

/* In the comment, the type of `void *ev_data` is specified */
enum mgos_azure_event {
  MGOS_AZURE_EVENT_CLOUD_MSG = /* struct mgos_azure_cloud_msg * */
  MGOS_AZURE_EVENT_BASE,
};

struct mgos_azure_cloud_msg {
  struct mg_str body;
  /* TODO(rojer): Parse properties too */
};

struct mg_str mgos_azure_get_device_id(void);

#ifdef __cplusplus
}
#endif

#endif /* CS_MOS_LIBS_AZURE_INCLUDE_MGOS_AZURE_H_ */