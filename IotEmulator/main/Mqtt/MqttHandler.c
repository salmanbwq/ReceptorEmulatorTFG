//
// Created by salman on 28/02/25.
//

#include "MqttHandler.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"

#define WIFI_SSID "WifiEsp"
#define WIFI_PASS "WifiTfg2025"
#define THINGSBOARD_SERVER "mqtt://demo.thingsboard.io"
#define ACCESS_TOKEN "42dcrrkvfb8s56kpqwfx"  // Reemplaza con tu Access Token
#define MQTT_TOPIC "v1/devices/me/telemetry"
#define LOG_TOPIC "v1/devices/me/logs"
static const char *TAG = "THINGSBOARD";
static esp_mqtt_client_handle_t client;

void wifi_init(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

void log_to_mqtt(const char *message) {
    esp_mqtt_client_publish(client, LOG_TOPIC, message, 0, 1, 0);
    ESP_LOGI(TAG, "Log enviado: %s", message);
}

// Callback para eventos MQTT
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado al broker MQTT");
            log_to_mqtt("Conectado al broker MQTT");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "Desconectado del broker MQTT");
            log_to_mqtt("Desconectado del broker MQTT");
            break;
        default:
            break;
    }
}

// Función para publicar telemetría
void publish_telemetry(char *temperature, char *humidity) {
    char payload[100];
    snprintf(payload, sizeof(payload), "{\"device\": %s, \"state\": %s}", temperature, humidity);
    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Telemetría enviada: %s", payload);
    log_to_mqtt(payload);

}

void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = THINGSBOARD_SERVER,
        .credentials.username = ACCESS_TOKEN,
        .session.keepalive = 120,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void telemetry_task(void *pvParameters) {
    while (1) {
        float temperature = 25.0 + (rand() % 10); // Simulación de temperatura
        float humidity = 50.0 + (rand() % 10); // Simulación de humedad
        publish_telemetry("habitacion.luz", "on");
        vTaskDelay(pdMS_TO_TICKS(10000)); // Cada 10 segundos
    }
}

void initMqtt(void) {
    ESP_LOGI(TAG, "Iniciando WiFi y ThingsBoard MQTT...");
    log_to_mqtt("Iniciando WiFi y ThingsBoard MQTT...");
    wifi_init();
    mqtt_app_start();
}
