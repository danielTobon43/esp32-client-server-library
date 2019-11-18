#include "include/post_data_cloud_module.h"

static const char *TAG = "HTTP_CLIENT";

esp_err_t http_post_to_KAA(char *json) {

  // ESP_LOGI(TAG, "CONECTING TO KAA WEB SITE AND POSTING DATA...");
  ESP_LOGI(TAG, "Begin http protocol\n");
  printf("\n");
  printf("~~~~~~~~~~~~~~~~~\n");
  printf("  POST TO CLOUD  \n");
  printf("~~~~~~~~~~~~~~~~~\n");
  printf("\n");
  printf("--- DEBUG SHIT ---\n");
  esp_http_client_config_t config = {

      .url = "http://46.22.138.171/api/PostData",
      .event_handler = _http_event_handle,
      //.username = "test@user",
      //.password = "testuser",
      //.auth_type = HTTP_AUTH_TYPE_BASIC,
  };

  // GET
  esp_http_client_handle_t client = esp_http_client_init(&config);
  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
  } else {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
  }

  // POST
  esp_http_client_set_method(client, HTTP_METHOD_POST);

  // vTaskDelay(500 / portTICK_PERIOD_MS);
  esp_http_client_set_post_field(client, json, strlen(json));

  // vTaskDelay(100 / portTICK_PERIOD_MS);
  err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
  }

  printf("\n");
  printf("--- DEBUG SHIT ---\n");
  printf("\n");

  printf("-> url: %s \n", config.url);
  printf("-> Sending data to cloud\n");
  printf("*** OK ***\n");
  // vTaskDelay(200 / portTICK_PERIOD_MS);
  printf("-> cleaning buffer for next data\n");
  free(json);
  printf("*** OK ***\n");
  printf("\n");
  esp_http_client_close(client);
  esp_http_client_cleanup(client);

  return ESP_OK;
}
/*
static cJSON *doit(char *text) {
  char *out;
  cJSON *json;

  json = cJSON_Parse(text);

  if (!json) {
    printf("Error before: [%s]\n", cJSON_GetErrorPtr());
  } else {
    out = cJSON_Print(json);
    cJSON_Delete(json);
    printf("%s\n", out);
    free(out);
  }
  return json;
}
*/
esp_err_t _http_event_handle(esp_http_client_event_t *evt) {
  switch (evt->event_id) {
  case HTTP_EVENT_ERROR:
    ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
    break;
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADER_SENT:
    ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
    printf("%.*s", evt->data_len, (char *)evt->data);
    break;
  case HTTP_EVENT_ON_DATA:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
    if (!esp_http_client_is_chunked_response(evt->client)) {
      printf("%.*s", evt->data_len, (char *)evt->data);
    }

    break;
  case HTTP_EVENT_ON_FINISH:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
    break;
  }
  return ESP_OK;
}

/*
  cJSON *json_obj;
  char *str = "{ \"msg-type\": [ \"0xdeadbeef\", \"irc log\" ], \
        \"msg-from\": { \"class\": \"soldier\", \"name\": \"Wixilav\" }, \
        \"msg-to\": { \"class\": \"supreme-commander\", \"name\":
  \"[Redacted]\"
  }, \
        \"msg-log\": [ \
            \"soldier: Boss there is a slight problem with the piece
  offering
  to
  humans\", \
            \"supreme-commander: Explain yourself soldier!\", \
            \"soldier: Well they don't seem to move anymore...\", \
            \"supreme-commander: Oh snap, I came here to see them twerk!\" \
            ] \
        }";


// printf("str:\n---\n%s\n---\n\n", str);
*/

char *generate_json(int gpio_sensor_data) {
  cJSON *json_root;
  cJSON *PROP_sensor;
  char *json_unformatted;
  json_root = cJSON_CreateObject();

  // cJSON_AddItemToObject(root, "d", d = cJSON_CreateObject());
  cJSON_AddStringToObject(json_root, "sensor_id", "test-idf");
  cJSON_AddItemToObject(json_root, "properties",
                        PROP_sensor = cJSON_CreateObject());

  cJSON_AddNumberToObject(PROP_sensor, "random_number", gpio_sensor_data);

  // json_unformatted = cJSON_PrintUnformatted(root);
  printf("\n");

  if (!json_root) {
    printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    return "";
  } else {
    json_unformatted = cJSON_Print(json_root);
    cJSON_Delete(json_root);
    // printf("%s\n", json_unformatted);
    return json_unformatted;
    // free(json_unformatted);
  }

  // vTaskDelay(5000 / portTICK_PERIOD_MS);

  // printf("json format:\n---\n%s\n---\n\n", json_unformatted);
  // free(json_unformatted);

  // cJSON_AddNumberToObject(d, "temperature", 30.100);
  // cJSON_AddStringToObject(info, "properties", "dummy");
  // cJSON_AddNumberToObject(info, "heap", system_get_free_heap_size());
  // cJSON_AddStringToObject(info, "sdk", system_get_sdk_version());
  // cJSON_AddNumberToObject(info, "time", system_get_time());

  // while (1) {
  //   // cJSON_ReplaceItemInObject(info, "heap",
  //   // cJSON_CreateNumber(system_get_free_heap_size()));
  //   // cJSON_ReplaceItemInObject(info, "time",
  //   //                           cJSON_CreateNumber(system_get_time()));
  //   // cJSON_ReplaceItemInObject(info, "sdk",
  //   // cJSON_CreateString(system_get_sdk_version()));

  //   json_unformatted = cJSON_PrintUnformatted(root);
  //   // printf("[len = %d]  ", strlen(json_unformatted));

  //   // for (int var = 0; var < strlen(json_unformatted); ++var) {
  //   //   putc(json_unformatted[var], stdout);
  //   // }

  //   printf("\n");
  //   fflush(stdout);
  //   vTaskDelay(5000 / portTICK_PERIOD_MS);

  //   printf("str1:\n---\n%s\n---\n\n", json_unformatted);
  //   // delay(2000);
  //   free(json_unformatted);
  // }
  // printf("str2:\n---\n%s\n---\n\n", json_unformatted);
}

/*
// JSON example: { "sensor_id" : "gas station","properties" :
// {"power":11,"temperature":55.5} }
// { "sensor_id" : "gas station",
//   "properties" : {
//                    "power":11,
//                     "temperature":55.5
//                   }
// }
// cJSON *json = doit(str);
*/
/* char *pos_data = "{ \"sensor_id\":  \"test-idf\", \
//                        \"properties\": { \"random_number\": 8 } \
//                       }";*/
