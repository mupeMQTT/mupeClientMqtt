#include "mupeClientMqttWeb.h"
#include "mupeClientMqttNvs.h"
#include "mupeWeb.h"
//#include "mupeMdnsNtp.h"
#include "esp_http_server.h"

const char *client_uri_txt = "Client init";
static const char *TAG = "mupeClientWebInit";
#define STARTS_WITH(string_to_check, prefix) (strncmp(string_to_check, prefix, (strlen(prefix))))

esp_err_t client_get_handler(httpd_req_t *req) {
	extern const unsigned char modbus_index_start[] asm("_binary_client_html_start");
	extern const unsigned char modbus_index_end[] asm("_binary_client_html_end");
	const size_t mqtt_index_size = (modbus_index_end - modbus_index_start);
	httpd_resp_set_type(req, "text/html");
	httpd_resp_send(req, (const char*) modbus_index_start, mqtt_index_size);
	return ESP_OK;
}

esp_err_t root_client_post_handler(httpd_req_t *req) {

	ESP_LOGI(TAG, "root_post_handler req->uri=[%s] content length %d", req->uri,
			req->content_len);
	char *buf = malloc(req->content_len + 1);
	size_t off = 0;
	while (off < req->content_len) {
		/* Read data received in the request */
		int ret = httpd_req_recv(req, buf + off, req->content_len - off);
		if (ret <= 0) {
			if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
				httpd_resp_send_408(req);
			}
			free(buf);
			return ESP_FAIL;
		}

		off += ret;

	}
	buf[off] = '\0';

	char value[30];

	if (find_value("MqttBroker=", buf, value) > 0) {
		mqttBrokerSet(value);
	}

	return client_get_handler(req);
}

esp_err_t client_get_cfg(httpd_req_t *req) {
	ESP_LOGI(TAG, "client_get_cfg %s ", req->uri);

	char value[mqttBrokerGetSize()];
	httpd_resp_set_type(req, "text/html");
	mqttBrokerGet(value);

	httpd_resp_send_chunk(req, value, strlen(value));

	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

esp_err_t root_client_get_handler(httpd_req_t *req) {
	ESP_LOGI(TAG, "HTTP req %s ", req->uri);
	if (STARTS_WITH(req->uri, "/client/cfg") == 0) {
		return client_get_cfg(req);
	}

	return client_get_handler(req);;
}

httpd_uri_t client_uri_get = { .uri = "/client/*", .method = HTTP_GET,
		.handler = root_client_get_handler };

httpd_uri_t client_uri_post = { .uri = "/client/*", .method = HTTP_POST,
		.handler = root_client_post_handler, .user_ctx = NULL };

void mupeClientWebInit(void) {
	ESP_LOGI(TAG, "mupeClientWebInit");
	addHttpd_uri(&client_uri_post, client_uri_txt);
	addHttpd_uri(&client_uri_get, client_uri_txt);
}
