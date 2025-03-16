//
// Created by salman on 1/03/25.
//

#include "RFDevices.h"

#include <esp_log.h>
#include <stdbool.h>
#include <string.h>
#include <Mqtt/MqttHandler.h>
#include <Mqtt/MqttPublishMethods.h>

static const char *TAG = "RFDevices";

static bool garageDoorStatus = false;
static bool alarmStatus = false;

RFDispositive getRFDispositive(const char *dispName) {
    if (strcmp(dispName, "Alarm") == 0) return ALARM;
    if (strcmp(dispName, "Garage") == 0) return GARAGE_DOOR;
    return UNKNOWN_RF;
}

GarageDoorCommands getGarageDoorCommands(const char *command) {
    if (strcmp(command, "OpenGarageDoor") == 0) return GD_OPEN;
    if (strcmp(command, "CloseGarageDoor") == 0) return GD_CLOSE;
    return GD_UNKNOWN_COMMAND;
}

AlarmCommands getAlarmCommands(const char *command) {
    if (strcmp(command, "PowerOn") == 0) return ALARM_ON;
    if (strcmp(command, "PowerOff") == 0) return ALARM_OFF;
    return ALARM_UNKNOWN_COMMAND;
}

void processGarageRFCommands(const char *command) {
    switch (getGarageDoorCommands(command)) {
        case GD_OPEN:
            ESP_LOGI(TAG, "🔓 Abriendo puerta del garaje...");
            garageDoorStatus = true;
            break;
        case GD_CLOSE:
            ESP_LOGI(TAG, "🔒 Cerrando puerta del garaje...");
            garageDoorStatus = false;
            break;
        default:
            ESP_LOGW(TAG, "⚠️ Comando de garaje desconocido: %s", command);
            return; // Salimos para no hacer publicaciones innecesarias
    }
    vTaskDelay(pdMS_TO_TICKS(500)); // 🔄 Retraso antes de publicar estado
    publishGarageStatus(garageDoorStatus);
}

void processRFAlarmCommand(const char *command) {
    switch (getAlarmCommands(command)) {
        case ALARM_ON:
            ESP_LOGI(TAG, "🚨 Activando alarma...");
            alarmStatus = true;
            break;
        case ALARM_OFF:
            ESP_LOGI(TAG, "🔕 Desactivando alarma...");
            alarmStatus = false;
            break;
        default:
            ESP_LOGW(TAG, "⚠️ Comando de alarma desconocido: %s", command);
            return;
    }
    publishAlarmStatus(alarmStatus);
}

void processGarageRFTask(void *param) {
    char *command = (char *) param;
    processGarageRFCommands(command);
    vTaskDelete(NULL);
}

void processAlarmRFTask(void *param) {
    char *command = (char *) param;
    processRFAlarmCommand(command);
    vTaskDelete(NULL);
}

void processRFCommand(char *trama) {
    char command[50] = {0};
    char deviceName[50] = {0};

    // 📌 Manejo seguro de strtok()
    char *token = strtok(trama, "/");
    if (token == NULL) {
        ESP_LOGE(TAG, "⚠️ Error: No se encontró dispositivo en la trama RF");
        return;
    }
    strncpy(deviceName, token, sizeof(deviceName) - 1);

    token = strtok(NULL, " ");
    if (token == NULL) {
        ESP_LOGE(TAG, "⚠️ Error: No se encontró comando en la trama RF");
        return;
    }
    strncpy(command, token, sizeof(command) - 1);

    ESP_LOGI(TAG, "📡 Dispositivo: %s | Comando: %s", deviceName, command);

    switch (getRFDispositive(deviceName)) {
        case ALARM:
            xTaskCreate(processAlarmRFTask, "GarageTask", 4096, strdup(command), 5, NULL);
            break;
        case GARAGE_DOOR:
            xTaskCreate(processGarageRFTask, "GarageTask", 4096, strdup(command), 5, NULL);
            break;
        default:
            ESP_LOGW(TAG, "❌ Dispositivo RF desconocido: %s", deviceName);
    }
}
