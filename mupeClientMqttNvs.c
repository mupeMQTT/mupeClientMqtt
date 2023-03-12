// Copyright Peter Müller mupe
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "nvs_flash.h"
#include "mupeClientMqttNvs.h"
#include "esp_log.h"



#define NAMESPACE_NAME "ClientCfg"
static const char *TAG = "mupeClientMqttNvs";

void mupeClientNvsInit(void) {
	ESP_LOGI(TAG, "mupeClientNvsInit");
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
}

void mqttBrokerSet(char * mqttBroker){
	ESP_LOGI(TAG, "mqttBrokerSet");
	nvs_handle_t my_handle;
	nvs_open(NAMESPACE_NAME, NVS_READWRITE, &my_handle);
	nvs_set_str(my_handle, "MqttBroker", mqttBroker);
	nvs_commit(my_handle);
	nvs_close(my_handle);
}

size_t mqttBrokerGetSize(){

	nvs_handle_t my_handle;
	size_t strSize = 0;
	nvs_open(NAMESPACE_NAME, NVS_READWRITE, &my_handle);
	nvs_get_str(my_handle, "MqttBroker", NULL, &strSize);
	nvs_commit(my_handle);
	nvs_close(my_handle);
	ESP_LOGI(TAG, "mqttBrokerGetSize %u",strSize);
	return strSize;
}

void mqttBrokerGet(char *mqttBroker) {
	ESP_LOGI(TAG, "mqttBrokerGetSize %s",mqttBroker);
	nvs_handle_t my_handle;
	size_t strSize = 0;
	nvs_open(NAMESPACE_NAME, NVS_READWRITE, &my_handle);
	nvs_get_str(my_handle, "MqttBroker", NULL, &strSize);
	nvs_get_str(my_handle,"MqttBroker", mqttBroker, &strSize);
	nvs_close(my_handle);

}
