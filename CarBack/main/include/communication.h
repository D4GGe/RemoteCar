#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "esp_err.h"
#include "esp_event.h"
#include "mqtt.h"

void connected_cb(void* self, void* params);
void disconnected_cb(void* self, void* params);
void reconnect_cb(void* self, void* params);
void subscribe_cb(void* self, void* params);
void publish_cb(void* self, void* params);
void data_cb(void* self, void* params);
static esp_err_t wifi_event_handler(void* ctx, system_event_t* event);
void wifi_conn_init(void);

#endif