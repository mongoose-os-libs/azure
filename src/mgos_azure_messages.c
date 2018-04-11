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

#include "mgos_azure.h"
#include "mgos_azure_internal.h"

#include "common/cs_dbg.h"

#include "mgos_mqtt.h"

static void mgos_azure_cloud_message_ev(struct mg_connection *nc, int ev,
                                        void *ev_data, void *user_data) {
  if (ev != MG_EV_MQTT_PUBLISH) return;
  struct mg_mqtt_message *msg = (struct mg_mqtt_message *) ev_data;
  struct mgos_azure_cloud_msg cloud_msg = {
      .body = msg->payload,
  };
  LOG(LL_DEBUG,
      ("Cloud msg: '%.*s'", (int) cloud_msg.body.len, cloud_msg.body.p));
  mgos_event_trigger(MGOS_AZURE_EVENT_CLOUD_MSG, &cloud_msg);
  (void) nc;
  (void) ev_data;
  (void) user_data;
}

bool mgos_azure_messages_init(void) {
  if (!mgos_sys_config_get_azure_enable_cloud_messages()) return true;
  struct mg_str did = mgos_azure_get_device_id();
  char *topic = NULL;
  mg_asprintf(&topic, 0, "devices/%.*s/messages/devicebound/#", (int) did.len,
              did.p);
  mgos_mqtt_global_subscribe(mg_mk_str(topic), mgos_azure_cloud_message_ev,
                             NULL);
  free(topic);
  return true;
}
