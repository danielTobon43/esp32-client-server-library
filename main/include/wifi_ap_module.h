//----------- ESP HEADERS --------------
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_http_client.h"

#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "esp_task.h"

#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "esp_tls.h"

#include <esp_http_server.h>

#define SSID_AP "ESP32-Access-Point"
#define PASSWORD_AP ""
#define EXAMPLE_ESP_MAXIMUM_RETRY 120

esp_err_t start_esp32_ap_mode(char **ipv4);

void wifi_event_handler(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data);
