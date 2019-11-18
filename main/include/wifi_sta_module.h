//----------- ESP HEADERS --------------
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcpip_adapter.h>

// #define WIFI_SSID "New_Frontiers"
// #define WIFI_PASS ""
#define EXAMPLE_ESP_MAXIMUM_RETRY 120

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id,
                   void *event_data);

esp_err_t start_esp32_station_mode(char *ssid, char *password);
void ota_server_task(void *param);
void star_ota_update();