//
// Created by salman on 1/03/25.
//

#include "RFIDDevices.h"

#include <esp_log.h>
#include <Main.h>
#include <string.h>
#include <Mqtt/MqttHandler.h>

static char *TAG = "RFID Devices";

static bool doorStatus = false;
static bool elevatorStatus = false;


static RFIDDispositive getRFIDDispositive(const char *dispName) {
    if (strcmp(dispName, "Door") == 0) return ENTRANCE_DOOR;
    if (strcmp(dispName, "Ascensor") == 0) return Elevator;
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
            if (strcmp("open", command) == 0) {
                ESP_LOGI(TAG, "Door command check success, door opened");
                doorStatus = true;
            } else if (strcmp("close", command) == 0) {
                ESP_LOGE(TAG, "Door command check success, door closed");
                doorStatus = false;
            } else {
                ESP_LOGE(TAG, "Door command check failed");
            }
            publishEntranceDoorStatus(doorStatus);
            break;
        case Elevator:
            ESP_LOGI(TAG, "Storage lock command received");
            if (strcmp("open", command) == 0) {
                ESP_LOGI(TAG, "Elevator command check success, opened");
                elevatorStatus = true;
            } else if (strcmp("close", command) == 0) {
                ESP_LOGI(TAG, "Elevator command check success, closed");
                elevatorStatus = false;
            } else {
                ESP_LOGE(TAG, "Storage lock command check failed");
            }
            publishElevatorStatus(elevatorStatus);
            break;

        default:
            ESP_LOGI(TAG, "Unknown dispositive received");
    }
}
