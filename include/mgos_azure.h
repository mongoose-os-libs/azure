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
  /* Connected to the Azure cloud. Arg: NULL */
  MGOS_AZURE_EVENT_CONNECT = MGOS_AZURE_EVENT_BASE,
  /* Incoming Cloud Message. Arg: struct mgos_azure_cloud_msg_arg * */
  MGOS_AZURE_EVENT_CM = MGOS_AZURE_EVENT_BASE,
  /* Incoming Direct Method invocation. Arg: struct mgos_azure_dm_arg * */
  MGOS_AZURE_EVENT_DM,
  /* Disonnected from the cloud. Arg: NULL */
  MGOS_AZURE_EVENT_CLOSE,
};

struct mgos_azure_cm_arg {
  struct mg_str body;
  /* TODO(rojer): Parse properties too */
};

struct mgos_azure_dm_arg {
  int64_t id;
  struct mg_str method;
  struct mg_str payload;
};

/* Returns host name of the Azure hub */
struct mg_str mgos_azure_get_host_name(void);

/* Returns Azure device ID */
struct mg_str mgos_azure_get_device_id(void);

/* Respond to a Direct Method call. */
bool mgos_azure_dm_response(int64_t id, int status, const struct mg_str *resp);

/* Respond to a Direct Method call with a JSON message. */
bool mgos_azure_dm_responsef(int64_t id, int status, const char *json_fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* CS_MOS_LIBS_AZURE_INCLUDE_MGOS_AZURE_H_ */
