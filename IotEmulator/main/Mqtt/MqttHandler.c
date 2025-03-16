//
// Created by salman on 28/02/25.
//

#include "MqttHandler.h"

#include <esp_now.h>
#include <stdio.h>
#include <string.h>
#include <esp32p4/rom/ets_sys.h>
#include <ESPNOW/espNow.h>

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

/**
 * @brief Callback para recibir datos por ESP-NOW
 */
void espnow_receive_cb(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    char log_message[128];
    snprintf(log_message, sizeof(log_message), "ESP-NOW Recibido en el puerto 11: %.*s", len, (char *) data);

    ESP_LOGI(TAG, "%s", log_message);
    log_to_mqtt(log_message);
}


void wifi_init(void) {
    ESP_LOGI(TAG, "Inicializando WiFi...");
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap(); // Crear AP
    esp_netif_create_default_wifi_sta(); // Crear STA

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .channel = 11,
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Conectando a WiFi...");
    ESP_ERROR_CHECK(esp_wifi_connect());

    // 🔹 Fijar canal para ESP-NOW y WiFi
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
}


void log_to_mqtt(const char *message) {
    esp_mqtt_client_publish(client, LOG_TOPIC, message, 0, 1, 0);
    ESP_LOGI(TAG, "Log enviado: %s", message);
}

void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len) {
    ESP_LOGI("ESP_RECV", "Data recibida" MACSTR "%s", MAC2STR(esp_now_info->src_addr), data);
}

void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        ESP_LOGI("ESP_SEND", "ESP_NOW_SEND_SUCCESS");
    } else {
        ESP_LOGE("ESP_SEND", "esp_now_send_data() falló con error: %s", esp_err_to_name(status));
    }
}

/**
 * @brief Inicializa ESP-NOW en modo estación y registra el callback de recepción.
 */
void espnow_init(esp_now_recv_cb_t recv) {
    esp_now_register_send_cb(send_cb);

    if (esp_now_init() != ESP_OK) {
        ESP_LOGE(TAG, "Error inicializando ESP-NOW");
        return;
    }

    ESP_ERROR_CHECK(esp_now_register_recv_cb(recv));
}

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

/**
 * @brief Inicializa WiFi, MQTT y ESP-NOW.
 */
void initMqtt(esp_now_recv_cb_t recv) {
    ESP_LOGI(TAG, "Iniciando WiFi, MQTT y ESP-NOW...");
    log_to_mqtt("Iniciando WiFi, MQTT y ESP-NOW...");

    wifi_init();
    mqtt_app_start();
    espnow_init(recv); // Iniciar ESP-NOW después de WiFi
}

/////////////////////////////////////////////////
/// Funciones de envio de telemetria //////////
//////////////////////////////////////////////


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

void publishGarageStatus(bool status) {
    char payload[150];

    snprintf(payload, sizeof(payload),
             "{\"garageStatus\": \"%s\"}",
             status ? "Open" : "Closed");

    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Datos de TV enviados: %s", payload);
    log_to_mqtt(payload);
}

void publishAlarmStatus(bool status) {
    char payload[150];

    snprintf(payload, sizeof(payload),
             "{\"Alarm\": \"%s\"}",
             status ? "ON" : "OFF");

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
    char colourName[10];

    switch (colour) {
        case 0:
            strcpy(colourName, "Yellow");
            break;
        case 1:
            strcpy(colourName, "Red");
            break;
        case 2:
            strcpy(colourName, "Green");
            break;
        default:
            strcpy(colourName, "White");
    }

    snprintf(payload, sizeof(payload),
             "{\"lampPowerStatus\": \"%s\", \"lampColour\": %s}",
             power_status ? "ON" : "OFF", colourName);

    esp_mqtt_client_publish(client, MQTT_TOPIC, payload, 0, 1, 0);
    ESP_LOGI(TAG, "Datos de TV enviados: %s", payload);
    log_to_mqtt(payload);
}
