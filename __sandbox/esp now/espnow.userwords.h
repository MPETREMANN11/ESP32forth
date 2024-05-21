// Copyright 2024 Marc PETREMANN
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
  XV(espnow, "esp_now_get_version", ESP_NOW_GET_VERSION, n0 = esp_now_get_version((uint32_t *) a0);)


const char user_source[] = R"""(
vocabulary espnow   espnow definitions

  6 constant ESP_NOW_ETH_ALEN               \ Length of ESPNOW peer MAC address
 16 constant ESP_NOW_KEY_LEN                \ Length of ESPNOW peer local master key
 20 constant ESP_NOW_MAX_TOTAL_PEER_NUM     \ Maximum number of ESPNOW total peers
  6 constant ESP_NOW_MAX_ENCRYPT_PEER_NUM   \ Maximum number of ESPNOW encrypted peers
250 constant ESP_NOW_MAX_DATA_LEN           \ Maximum length of ESPNOW data which is sent very time

transfer espnow-builtins
forth definitions
)""";
