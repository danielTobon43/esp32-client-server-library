//----------- ESP HEADERS --------------
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "esp_task.h"
#include "esp_tls.h"
#include "esp_wifi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "nvs_flash.h"

#include "xtensa/core-macros.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

#define MAX_HTTP_RECV_BUFFER 512

esp_err_t _http_event_handle(esp_http_client_event_t *evt);
esp_err_t http_post_to_KAA(char *json);
char *generate_json(int gpio_sensor_data);
