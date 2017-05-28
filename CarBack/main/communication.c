#include "communication.h"

#include "esp_event_loop.h"
#include "esp_wifi.h"

#include "debug.h"
#include "user_config.h"

mqtt_settings settings = {.host = "wijken.se",
#if defined(CONFIG_MQTT_SECURITY_ON)
                          .port = 8883,  // encrypted
#else
                          .port = 1883,  // unencrypted
#endif
                          .client_id = "esp32-car",
                          .username = "user",
                          .password = "pass",
                          .clean_session = 0,
                          .keepalive = 120,
                          .lwt_topic = "/lwt",
                          .lwt_msg = "offline",
                          .lwt_qos = 0,
                          .lwt_retain = 0,
                          .connected_cb = connected_cb,
                          .disconnected_cb = disconnected_cb,
                          .reconnect_cb = reconnect_cb,
                          .subscribe_cb = subscribe_cb,
                          .publish_cb = publish_cb,
                          .data_cb = data_cb};

void connected_cb(void* self, void* params) {
  mqtt_client* client = (mqtt_client*)self;
  mqtt_subscribe(client, "/carback", 0);
  mqtt_publish(client, "/carback", "howdy!", 6, 0, 0);
}

void disconnected_cb(void* self, void* params) {}

void reconnect_cb(void* self, void* params) {}

void subscribe_cb(void* self, void* params) {
  INFO("[APP] Subscribe ok, test publish msg\n");
  mqtt_client* client = (mqtt_client*)self;
  mqtt_publish(client, "/carback", "abcde", 5, 0, 0);
}

void publish_cb(void* self, void* params) {}

void data_cb(void* self, void* params) {
  mqtt_client* client = (mqtt_client*)self;
  mqtt_event_data_t* event_data = (mqtt_event_data_t*)params;

  if (event_data->data_offset == 0) {
    char* topic = malloc(event_data->topic_length + 1);
    memcpy(topic, event_data->topic, event_data->topic_length);
    topic[event_data->topic_length] = 0;
    INFO("[APP] Publish topic: %s\n", topic);
    free(topic);
  }

  // char *data = malloc(event_data->data_length + 1);
  // memcpy(data, event_data->data, event_data->data_length);
  // data[event_data->data_length] = 0;
  INFO("[APP] Publish data[%d/%d bytes]\n",
       event_data->data_length + event_data->data_offset,
       event_data->data_total_length);
  // data);

  // free(data);
}

static esp_err_t wifi_event_handler(void* ctx, system_event_t* event) {
  switch (event->event_id) {
    case SYSTEM_EVENT_STA_START:
      ESP_ERROR_CHECK(esp_wifi_connect());
      break;

    case SYSTEM_EVENT_STA_GOT_IP:

      mqtt_start(&settings);
      // Notice that, all callback will called in mqtt_task
      // All function publish, subscribe
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      /* This is a workaround as ESP32 WiFi libs don't currently
      auto-reassociate. */

      mqtt_stop();
      ESP_ERROR_CHECK(esp_wifi_connect());
      break;
    default:
      break;
  }
  return ESP_OK;
}

void wifi_conn_init(void) {
  INFO("[APP] Start, connect to Wifi network: %s ..\n", WIFI_SSID);

  tcpip_adapter_init();

  ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));

  wifi_init_config_t icfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&icfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

  wifi_config_t wifi_config = {
      .sta = {.ssid = WIFI_SSID, .password = WIFI_PASS},
  };

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
}