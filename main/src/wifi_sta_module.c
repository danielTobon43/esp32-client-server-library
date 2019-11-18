//----------- ESP HEADERS --------------
#include "include/wifi_sta_module.h"

static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;
static const int WIFI_CONNECTED_BIT = BIT0;
static const char *TAG = "WIFI-STATION";

static char *WIFI_SSID = "";
static char *WIFI_PASS = "";

void string_copy2(char *from, char *to) {

  while ((*to++ = *from++) != '\0')
    ;
}

esp_err_t start_esp32_station_mode(char *ssid, char *password) {

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Print SSID and password
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  printf("\n");
  printf("-> data received: \n ssid: %s \n password: %s \n", ssid, password);
  printf("\n");

  WIFI_SSID = malloc(strlen(ssid) + 1);
  WIFI_PASS = malloc(strlen(password) + 1);

  string_copy2(ssid, WIFI_SSID);
  string_copy2(password, WIFI_PASS);

  printf("~~~~~~~~~~~~~~~~~\n");
  printf("     STA MODE    \n");
  printf("~~~~~~~~~~~~~~~~~\n");
  printf("-> setting event loop\n");

  s_wifi_event_group = xEventGroupCreate();

  tcpip_adapter_init();

  ESP_ERROR_CHECK(esp_event_loop_create_default());

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             &event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                             &event_handler, NULL));

  // printf("WIFI SSID STA: %s\n", WIFI_SSID);
  // printf("WIFI PASSWORD STA: %s\n", WIFI_PASS);
  printf("-> setting wifi config options\n");

  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MAX_MODEM));

  wifi_config_t wifi_config = {};

  // wifi_config_t wifi_config = {
  //     // Define wifi mode as station (connects to an existing network)
  //     .sta = {.ssid = (char *)WIFI_SSID, .password = (char *)WIFI_PASS

  //     },
  // };

  // wifi_config_t wifi_config = {};
  strcpy((char *)wifi_config.sta.ssid, (char *)WIFI_SSID);
  if (strcmp(WIFI_PASS, "none") == 0) {
    printf("-> Configuring wifi without user password...\n");
  } else {
    strcpy((char *)wifi_config.sta.password, (char *)WIFI_PASS);
  }
  // char *pPosition = strchr(WIFI_PASS, '"');
  // if (pPosition == NULL) {
  //   printf("-> Configuring wifi without user password...\n");
  //   // vTaskDelay(5000 / portTICK_PERIOD_MS);
  //   // vTaskDelay(5000 / portTICK_PERIOD_MS);
  // } else {
  //   strcpy((char *)wifi_config.sta.password, (char *)WIFI_PASS);
  // }

  wifi_config.sta.scan_method = WIFI_AUTH_WPA2_PSK;
  wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;

  // printf("WIFI SSID STA CONFIG: %s\n", wifi_config.sta.ssid);
  // printf("WIFI PASSWORD STA CONFIG: %s\n", wifi_config.sta.password);
  printf("-> setting mode: STA\n");
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  printf("-> starting wifi station mode\n");
  ESP_ERROR_CHECK(esp_wifi_start());
  printf("-> connecting to wifi host...\n");
  // vTaskDelay(3000 / portTICK_PERIOD_MS);
  ESP_ERROR_CHECK(esp_wifi_connect());

  tcpip_adapter_ip_info_t ipInfo;
  tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);

  char *ip_p = ip4addr_ntoa(&ipInfo.ip); // 0.0.0.0
                                         // 0 2 4 6

  static const char *TAG = "WIFI-STATION";

  // Checking if wifi is connected...
  while (1) {

    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
    ip_p = ip4addr_ntoa(&ipInfo.ip);

    // check if this numbers are zero -> 0.0.0.0
    if (ip_p[0] != '0' || ip_p[2] != '0' || ip_p[4] != '0' || ip_p[6] != '0') {
      // printf("WIFI CONNECTION OK\n");
      // printf("\n");
      // ESP_LOGI(TAG, "\n");
      ESP_LOGI(TAG, "*** BOOL VAL = TRUE ***\n");
      // ESP_LOGI(TAG, "\n");

      ESP_LOGI(TAG, "~~~~~~~~~~~");
      ESP_LOGI(TAG, "IP:" IPSTR, IP2STR(&ipInfo.ip));
      ESP_LOGI(TAG, "MASK:" IPSTR, IP2STR(&ipInfo.netmask));
      ESP_LOGI(TAG, "GW:" IPSTR, IP2STR(&ipInfo.gw));
      ESP_LOGI(TAG, "~~~~~~~~~~~");
      break;
    } else {
      // printf("NOT YET!\r");
      // printf("\n");
      // ESP_LOGI(TAG, "\n");
      ESP_LOGE(TAG, "*** BOOL VAL = FALSE ***\n");
      // ESP_LOGI(TAG, "\n");

      ESP_LOGE(TAG, "~~~~~~~~~~~");
      ESP_LOGE(TAG, "IP:" IPSTR, IP2STR(&ipInfo.ip));
      ESP_LOGE(TAG, "MASK:" IPSTR, IP2STR(&ipInfo.netmask));
      ESP_LOGE(TAG, "GW:" IPSTR, IP2STR(&ipInfo.gw));
      ESP_LOGE(TAG, "~~~~~~~~~~~");
      vTaskDelay(300 / portTICK_PERIOD_MS);
      // vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }

  printf("-> ssid: %s \n", wifi_config.sta.ssid);
  printf("-> ipv4: " IPSTR "\n", IP2STR(&ipInfo.ip));
  printf("*** OK ***\n");
  printf("\n");

  return ESP_OK;
}

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id,
                   void *event_data) {

  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();

  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
      esp_wifi_connect();
      xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
      s_retry_num++;
      ESP_LOGI(TAG, "retry to connect to the AP");
    }
    ESP_LOGI(TAG, "connect to the AP fail");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->ip_info.ip));
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
  } else if (SYSTEM_EVENT_STA_GOT_IP) {
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
  }
}

