idf_component_register(SRCS "mupeClientMqtt.c" "mupeClientMqttWeb.c" "mupeClientMqttNvs.c" 
                    INCLUDE_DIRS "include"
                    REQUIRES "mqtt"   "esp_wifi" "esp_http_server" "mupeWeb" 
                    EMBED_TXTFILES  "client.html"   )

