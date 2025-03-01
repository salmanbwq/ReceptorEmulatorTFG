//
// Created by salman on 1/03/25.
//

#include "IRDevices.h"

#include <esp_log.h>
#include <Main.h>
#include <string.h>

#include "AC/ACControl.h"
#include "LED/LEDControl.h"
#include "Projector/ProjectorControl.h"
#include "TV/TVControl.h"

static char *TAG = "IR Devices";


static IRDispositive getIRDispositive(const char *dispName) {
    if (strcmp(dispName, "FB04") == 0) return TV;
    if (strcmp(dispName, "FB05") == 0) return AC;
    if (strcmp(dispName, "FB06") == 0) return LED;
    if (strcmp(dispName, "FB07") == 0) return PROJECTOR;
    return UNKNOWN_IR;
}


void processIRCommand(command_t receivedCmd) {
    char name[50];
    char command[50];
    strcpy(name, strtok(receivedCmd.data, "/"));
    strcpy(command, strtok(NULL, " "));
    switch (getIRDispositive(name)) {
        case TV:
            ESP_LOGI(TAG, "TV command received");
            processTVCommand(command);
            break;
        case AC:
            ESP_LOGI(TAG, "AC command received");
            processACCommand(command);
            break;
        case LED:
            ESP_LOGI(TAG, "LED command received");
            processLedCommand(command);
            break;
        case PROJECTOR:
            ESP_LOGI(TAG, "PROJECTOR command received");
            processProjectorCommand(command);
            break;
        default:
            ESP_LOGW(TAG, "Unknown dispositive received");
    }
}
