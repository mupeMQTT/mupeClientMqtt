#ifndef MUPECLIENTMQTT
#define MUPECLIENTMQTT

#include "mqtt_client.h"

void mupeClientMqttInit(void);
void mupeClientSend(char *topic, char *msg);
esp_mqtt_client_handle_t  get_esp_mqtt_client_handle_t();

#endif
