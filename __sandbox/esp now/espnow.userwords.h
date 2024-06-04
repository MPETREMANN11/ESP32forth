// Copyright 2024 Marc PETREMANN
// latest ver: 04 jun 2024
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

# include <esp_now.h>

#define USER_VOCABULARIES V(espnow)
#define USER_WORDS  \
  XV(internals, "user-source", USER_SOURCE, \
      PUSH user_source; PUSH sizeof(user_source) - 1) \
  XV(espnow, "esp_now_init", ESP_NOW_INIT, PUSH esp_now_init();) \
  XV(espnow, "esp_now_deinit", ESP_NOW_DEINIT, PUSH esp_now_deinit();) \
  XV(espnow, "esp_now_get_version", ESP_NOW_GET_VERSION, n0 = esp_now_get_version((uint32_t *) a0);) \
  XV(espnow, "esp_now_send", ESP_NOW_SEND, n0 = esp_now_send((uint8_t *) a2, (uint8_t *) a1, (size_t) n0);) \
  XV(espnow, "esp_now_register_recv_cb", ESP_NOW_REGISTER_RECV_CB, PUSH esp_now_register_recv_cb((esp_now_recv_cb_t) n0);) \
  XV(espnow, "esp_now_unregister_recv_cb", ESP_NOW_UNREGISTER_RECV_CB, PUSH esp_now_unregister_recv_cb();) \
  XV(espnow, "esp_now_register_send_cb", ESP_NOW_REGISTER_SEND_CB, PUSH esp_now_register_send_cb((esp_now_send_cb_t) n0);) \
  XV(espnow, "esp_now_unregister_send_cb", ESP_NOW_UNREGISTER_SEND_CB, PUSH esp_now_unregister_send_cb();) \
  XV(espnow, "esp_now_add_peer", ESP_NOW_ADD_PEER, n0 = esp_now_add_peer((const esp_now_peer_info_t *) a0);) \
  XV(espnow, "esp_now_del_peer", ESP_NOW_DEL_PEER, n0 = esp_now_del_peer((uint8_t *) a0);) \
  XV(espnow, "esp_now_mod_peer", ESP_NOW_MOD_PEER, n0 = esp_now_mod_peer((const esp_now_peer_info_t *) a0);) \
  XV(espnow, "esp_now_is_peer_exist", ESP_NOW_IS_PEER_EXIST, n0 = esp_now_is_peer_exist((uint8_t *) a0);) \
  XV(espnow, "esp_now_get_peer_num", ESP_NOW_GET_PEER_NUM, n0 = esp_now_get_peer_num((esp_now_peer_num_t *) a0);) \
  XV(espnow, "esp_now_set_pmk", ESP_NOW_SET_PMK, n0 = esp_now_set_pmk((uint8_t *) a0);) \
  XV(espnow, "ESP_NOW_MAX_DATA_LEN", ESP_NOW_MAX_DATA_LEN, PUSH ESP_NOW_MAX_DATA_LEN;)

const char user_source[] = R"""(
vocabulary espnow   espnow definitions
transfer espnow-builtins
forth definitions
)""";
