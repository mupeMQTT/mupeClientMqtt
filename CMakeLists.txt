idf_component_register(SRCS "mupeClientMqtt.c" "mupeClientMqttWeb.c" "mupeClientMqttNvs.c" "mupeDataBase.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "mqtt" "mupeMdnsNtp"  "esp_wifi" "esp_http_server" "mupeWeb" "mupeSdCard" "fatfs" "vfs"
                    EMBED_TXTFILES  "client.html"   )

