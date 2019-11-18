//----------- ESP HEADERS --------------
#include "include/wifi_ap_module.h"

static const char *TAG = "ap-station";

void wifi_event_handler(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data) {
  if (event_id == WIFI_EVENT_AP_STACONNECTED) {
    wifi_event_ap_staconnected_t *event =
        (wifi_event_ap_staconnected_t *)event_data;
    ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac),
             event->aid);
  } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
    wifi_event_ap_stadisconnected_t *event =
        (wifi_event_ap_stadisconnected_t *)event_data;
    ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac),
             event->aid);
  }
}

// void wifi_event_handler(void *arg, esp_event_base_t event_base,
//                         int32_t event_id, void *event_data) {

//   if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//     esp_wifi_connect();
//   } else if (event_base == WIFI_EVENT &&
//              event_id == WIFI_EVENT_STA_DISCONNECTED) {
//     if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
//       esp_wifi_connect();
//       xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
//       s_retry_num++;
//       ESP_LOGI(TAG, "retry to connect to the AP");
//     }
//     ESP_LOGI(TAG, "connect to the AP fail");
//   } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//     ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
//     ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->ip_info.ip));
//     s_retry_num = 0;
//     xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
//   }
// }

esp_err_t start_esp32_ap_mode(char **ipv4) {

  printf("~~~~~~~~~~~~~~~~~\n");
  printf("     AP MODE     \n");
  printf("~~~~~~~~~~~~~~~~~\n");
  // vTaskDelay(5000 / portTICK_PERIOD_MS);

  tcpip_adapter_init();
  static const char *TAG = "wifi softAP";
  printf("-> setting event loop\n");
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  // s_wifi_event_group = xEventGroupCreate();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  printf("-> setting wifi config options\n");

  wifi_config_t wifi_config = {
      .ap = {.ssid = SSID_AP,
             .ssid_len = strlen(SSID_AP),
             .password = PASSWORD_AP,
             .max_connection = 4,
             .authmode = WIFI_AUTH_WPA_WPA2_PSK},
  };
  if (strlen(PASSWORD_AP) == 0) {
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
  }

  printf("-> setting mode: AP\n");
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
  printf("-> starting wifi access point\n");
  ESP_ERROR_CHECK(esp_wifi_start());

  tcpip_adapter_ip_info_t ipInfo;
  tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ipInfo);

  ESP_LOGI(TAG, "~~~~~~~~~~~");
  ESP_LOGI(TAG, "IP:" IPSTR, IP2STR(&ipInfo.ip));
  ESP_LOGI(TAG, "MASK:" IPSTR, IP2STR(&ipInfo.netmask));
  ESP_LOGI(TAG, "GW:" IPSTR, IP2STR(&ipInfo.gw));
  ESP_LOGI(TAG, "~~~~~~~~~~~");

  *ipv4 = ip4addr_ntoa(&ipInfo.ip);
  char *mask = ip4addr_ntoa(&ipInfo.netmask);
  char *gw = ip4addr_ntoa(&ipInfo.gw);

  printf("-> parameters:\n");
  printf("-> ipv4: %s \n", *ipv4);
  printf("-> mask: %s \n", mask);
  printf("-> gw: %s \n", gw);
  printf("*** OK ***\n");
  printf("\n");
  // vTaskDelay(8000 / portTICK_PERIOD_MS);
  return ESP_OK;
}
