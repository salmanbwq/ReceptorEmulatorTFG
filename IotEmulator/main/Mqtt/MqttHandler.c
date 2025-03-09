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
uint8_t global_wifi_channel = 0; // Variable global para almacenar el canal WiFi

void wifi_wait_for_connection() {
    wifi_ap_record_t ap_info;
    for (int i = 0; i < 10; i++) {
        // Esperar hasta 10 intentos
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            global_wifi_channel = ap_info.primary;
            ESP_LOGI("WIFI_INIT", "✅ WiFi conectado en canal %d", global_wifi_channel);
            return;
        }
        ESP_LOGW("WIFI_INIT", "⚠️ Esperando a que WiFi esté completamente inicializado...");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo antes de reintentar
    }
    ESP_LOGE("WIFI_INIT", "❌ No se pudo obtener el canal de WiFi.");
}

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
            .channel = 1,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    // 🔥 Esperar conexión y obtener canal
    wifi_wait_for_connection();
}

uint8_t getChannel() {
    return global_wifi_channel;
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
void publish_telemetry(char *temperature, char *humidity, char *topic) {
    char payload[100];
    snprintf(payload, sizeof(payload), "{\"device\": %s, \"state\": %s}", temperature, humidity);
    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Telemetría enviada: %s", payload);
    log_to_mqtt(payload);
}

// Nueva función para enviar los datos de la TV correctamente
void publish_tv_telemetry(bool power_status, int tv_channel, int tv_volume) {
    char payload[150];

    snprintf(payload, sizeof(payload),
             "{\"power_status\": \"%s\", \"tv_channel\": %d, \"tv_volume\": %d}",
             power_status ? "ON" : "OFF", tv_channel, tv_volume);

    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Datos de TV enviados: %s", payload);
    log_to_mqtt(payload);
}

void publish_AC_telemetry(bool power_status, int temperature) {
    char payload[150];

    snprintf(payload, sizeof(payload),
             "{\"acPowerStatus\": \"%s\", \"acTemperature\": %d}",
             power_status ? "ON" : "OFF", temperature);

    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Datos de TV enviados: %s", payload);
    log_to_mqtt(payload);
}

void publishEntranceDoorStatus(bool status) {
    char payload[150];

    snprintf(payload, sizeof(payload),
             "{\"doorStatus\": \"%s\"}",
             status ? "Open" : "Closed");

    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Datos de TV enviados: %s", payload);
    log_to_mqtt(payload);
}


void publishElevatorStatus(bool status) {
    char payload[150];

    snprintf(payload, sizeof(payload),
             "{\"elevatorStatus\": \"%s\"}",
             status ? "Open" : "Closed");

    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Datos de TV enviados: %s", payload);
    log_to_mqtt(payload);
}

void publish_Lamp_telemetry(bool power_status, int colour) {
    char payload[150];

    snprintf(payload, sizeof(payload),
             "{\"lampPowerStatus\": \"%s\", \"lampColour\": %d}",
             power_status ? "ON" : "OFF", colour);

    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Datos de TV enviados: %s", payload);
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


void initMqtt(void) {
    ESP_LOGI(TAG, "Iniciando WiFi y ThingsBoard MQTT...");
    log_to_mqtt("Iniciando WiFi y ThingsBoard MQTT...");
    wifi_init();
    mqtt_app_start();
}
