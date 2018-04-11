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

#include "common/cs_base64.h"
#include "common/cs_dbg.h"

#include "mongoose.h"

#include "mgos_mqtt.h"
#include "mgos_sys_config.h"

struct mgos_azure_sas_ctx {
  char *host_name;
  char *device_id;
  char *access_key;
  int token_ttl;
};

static const char *s_device_id = NULL;

static void mgos_azure_mqtt_connect(struct mg_connection *c,
                                    const char *client_id,
                                    struct mg_send_mqtt_handshake_opts *opts,
                                    void *arg) {
  struct mgos_azure_sas_ctx *ctx = (struct mgos_azure_sas_ctx *) arg;
  uint64_t exp = (int64_t) mg_time() + ctx->token_ttl;
  struct mg_str tok = MG_NULL_STR;
  char *uri = NULL;
  mg_asprintf(&uri, 0, "%s/%s", ctx->host_name, ctx->device_id);
  opts->user_name = uri;
  tok =
      mgos_azure_gen_sas_token(mg_mk_str(uri), mg_mk_str(ctx->access_key), exp);
  opts->password = tok.p;
  LOG(LL_DEBUG, ("SAS token: %.*s", (int) tok.len, tok.p));
  mg_send_mqtt_handshake_opt(c, ctx->device_id, *opts);
  free(uri);
  free((void *) tok.p);
  (void) client_id;
}

struct mg_str mgos_azure_get_device_id(void) {
  return mg_mk_str(s_device_id);
}

bool mgos_azure_init(void) {
  bool ret = false;
  char *uri = NULL;
  struct mg_str cs;
  struct mgos_config_mqtt mcfg;
  const char *auth_method = NULL;
  if (!mgos_sys_config_get_azure_enable()) {
    ret = true;
    goto out;
  }
  mcfg = *mgos_sys_config_get_mqtt();
  mcfg.enable = true;
  if (mcfg.ssl_ca_cert == NULL) mcfg.ssl_ca_cert = "ca.pem";
  cs = mg_mk_str(mgos_sys_config_get_azure_cs());
  if (cs.len > 0) {
    struct mgos_azure_sas_ctx *ctx =
        (struct mgos_azure_sas_ctx *) calloc(1, sizeof(*ctx));
    ctx->token_ttl = mgos_sys_config_get_azure_token_ttl();
    if (mg_http_parse_header2(&cs, "HostName", &ctx->host_name, 0) <= 0 ||
        mg_http_parse_header2(&cs, "DeviceId", &ctx->device_id, 0) <= 0 ||
        mg_http_parse_header2(&cs, "SharedAccessKey", &ctx->access_key, 0) <=
            0) {
      LOG(LL_ERROR, ("Invalid connection string"));
      ret = false;
      goto out;
    }
    mcfg.server = ctx->host_name;
    mcfg.client_id = mcfg.user = mcfg.pass = NULL;
    mcfg.ssl_cert = mcfg.ssl_key = NULL;
    mgos_mqtt_set_connect_fn(mgos_azure_mqtt_connect, ctx);
    mg_asprintf(&uri, 0, "%s/%s", ctx->host_name, ctx->device_id);
    auth_method = "SAS";
  } else if (mgos_sys_config_get_azure_host_name() != NULL &&
             mgos_sys_config_get_azure_device_id() != NULL &&
             mgos_sys_config_get_azure_cert() != NULL &&
             mgos_sys_config_get_azure_key() != NULL) {
    mcfg.server = (char *) mgos_sys_config_get_azure_host_name();
    mcfg.client_id = (char *) mgos_sys_config_get_azure_device_id();
    mg_asprintf(&uri, 0, "%s/%s", mgos_sys_config_get_azure_host_name(),
                mgos_sys_config_get_azure_device_id());
    mcfg.user = uri;
    mcfg.pass = NULL;
    mcfg.ssl_cert = (char *) mgos_sys_config_get_azure_cert();
    mcfg.ssl_key = (char *) mgos_sys_config_get_azure_key();
    mgos_mqtt_set_connect_fn(NULL, NULL);
    auth_method = mcfg.ssl_cert;
  } else {
    LOG(LL_ERROR,
        ("azure.cs or azure.{host_name,device_id,cert,key} are required"));
    ret = false;
    goto out;
  }
  LOG(LL_INFO, ("Azure IoT Hub client for %s (%s)", uri, auth_method));

  if (!mgos_mqtt_set_config(&mcfg)) goto out;

  s_device_id = mcfg.client_id;

  ret = mgos_azure_messages_init();

out:
  free(uri);
  return ret;
}
