/*
esp-idf library for the esp32 board. This library is an example of the station and access point
module and posting data module to a web server. 
How to use:
- Comment lines that are you not going to use and save it.
- idf.py build
- idf.py flash -p /dev/ttyUSB0 -b 115200
- idf.py -p /dev/ttyUSB0 -b 115200 monitor
*/

//***************** Headers *****************************
#include "include/post_data_cloud_module.h"
#include "include/wifi_ap_module.h"
#include "include/wifi_sta_module.h"
#include "sdkconfig.h"

//***************** Main program ************************
void app_main(void) {

  printf("\n");
  printf("*** MAIN PROGRAM ***\n");
  printf("\n");

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Modules
  char *ssid 		= 	"SSID";
  char *password 	= 	"PASSWORD";
  char *ipv4;

  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------
  // Wifi station mode module
  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------
  ESP_ERROR_CHECK(start_esp32_station_mode(ssid,password));

  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------
  // Wifi access point mode module
  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------
  ESP_ERROR_CHECK(start_esp32_ap_mode(&ipv4)); 

  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------
  // Post data to cloud module
  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------

   char *json_example = "{ \"sensor_id\":  \"test-idf\", \
//                        \"properties\": { \"random_number\": 8 } \
//                       }";
  ESP_ERROR_CHECK(http_post_to_KAA(json_example));

}
//***************** End main program ************************
