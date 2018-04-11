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

/*
 * Cloud Messaging support.
 * https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-devguide-messaging
 */

#include "mgos_azure.h"
#include "mgos_azure_internal.h"

#include "common/cs_dbg.h"

#include "mgos_mqtt.h"

static void mgos_azure_cm_ev(struct mg_connection *nc, const char *topic,
                             int topic_len, const char *msg, int msg_len,
                             void *ud) {
  struct mgos_azure_cm_arg cma = {
      .body = {.p = msg, .len = msg_len},
  };
  LOG(LL_DEBUG, ("Cloud msg: '%.*s'", (int) cma.body.len, cma.body.p));
  mgos_event_trigger(MGOS_AZURE_EVENT_CM, &cma);
  (void) nc;
  (void) topic;
  (void) topic_len;
  (void) ud;
}

bool mgos_azure_cm_init(void) {
  if (!mgos_sys_config_get_azure_enable_cm()) return true;
  struct mg_str did = mgos_azure_get_device_id();
  char *topic = NULL;
  mg_asprintf(&topic, 0, "devices/%.*s/messages/devicebound/#", (int) did.len,
              did.p);
  mgos_mqtt_sub(topic, mgos_azure_cm_ev, NULL);
  free(topic);
  return true;
}
