//
// Created by salman on 1/03/25.
//

#include "RFIDDevices.h"

#include <esp_log.h>
#include <Main.h>
#include <string.h>

static char *TAG = "RFID Devices";


static RFIDDispositive getRFIDDispositive(const char *dispName) {
    if (strcmp(dispName, "Door") == 0) return ENTRANCE_DOOR;
    if (strcmp(dispName, "Alarm") == 0) return ALARM;
    if (strcmp(dispName, "Storage") == 0) return STORAGE_LOCK;
    return UNKNOWN_RFID;
}

void processRFIDCommand(char *receivedCmd) {
    char dispositiveName[50];
    char command[50];
    char *token = strtok(receivedCmd, " ");
    if (token == NULL) {
        ESP_LOGE(TAG, "Failed to parse dispositive name");
        return;
    }
    strncpy(dispositiveName, token, sizeof(dispositiveName) - 1);

    token = strtok(NULL, " ");
    if (token == NULL) {
        ESP_LOGE(TAG, "Failed to parse command");
        return;
    }
    strncpy(command, token, sizeof(command) - 1);

    ESP_LOGI(TAG, "Dispositive: %s | Command: %s", dispositiveName, command);
    switch (getRFIDDispositive(dispositiveName)) {
        case ENTRANCE_DOOR:
            ESP_LOGI(TAG, "Entrance door command received");
            if (strcmp("hsmCj495)<<N", command) == 0) {
                ESP_LOGI(TAG, "Door command check success, door opened");
            } else {
                ESP_LOGE(TAG, "Door command check failed");
            }
            break;
        case ALARM:
            ESP_LOGI(TAG, "Alarm command received");
            if (strcmp("55vK=9|'d£b$", command) == 0) {
                ESP_LOGI(TAG, "Alarm command check success, Disable alarm");
            } else {
                ESP_LOGE(TAG, "Alarm command check failed");
            }
            break;
        case STORAGE_LOCK:
            ESP_LOGI(TAG, "Storage lock command received");
            if (strcmp("hsmCj495)<<N", command) == 0) {
                ESP_LOGI(TAG, "Storage lock command check success");
            } else {
                ESP_LOGE(TAG, "Storage lock command check failed");
            }
            break;
        default:
            ESP_LOGI(TAG, "Unknown dispositive received");
    }
}
