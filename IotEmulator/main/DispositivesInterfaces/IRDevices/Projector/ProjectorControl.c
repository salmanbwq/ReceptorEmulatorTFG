//
// Created by salman on 1/03/25.
//

#include "ProjectorControl.h"

#include <esp_log.h>
#include <stdbool.h>
#include <string.h>
#include <DispositivesInterfaces/IRDevices/IRDevices.h>

static char *TAG = "Projector control";

static bool projectorON = false;

static ProjectorCommands getProjectorCommands(const char *command) {
    if (strcmp(command, "F001") == 0) return PROJECTOR_ON_OFF;
    return UNKNOWN_PROJECTOR_COMMAND;
}


void processProjectorCommand(char command[50]) {
    switch (getProjectorCommands(command)) {
        case PROJECTOR_ON_OFF:
            ESP_LOGI(TAG, "PROJECTOR ON OFF command received");
            projectorON = !projectorON;
            break;
        default:
            ESP_LOGI(TAG, "Unknown command received");
    }
}
