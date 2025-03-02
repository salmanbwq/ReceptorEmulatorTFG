//
// Created by salman on 1/03/25.
//

#include "ACControl.h"

#include <esp_log.h>
#include <stdbool.h>
#include <string.h>
#include <DispositivesInterfaces/IRDevices/IRDevices.h>
#include <Mqtt/MqttHandler.h>

static char *TAG = "A/C Control";

static bool acON = false;
static int acTemperature = 18;

static ACCommands getACCommands(const char *command) {
    if (strcmp(command, "FF01") == 0) return AC_ON_OFF;
    if (strcmp(command, "FF02") == 0) return AC_TEMPERATURE_UP;
    if (strcmp(command, "FF03") == 0) return AC_TEMPERATURE_DOWN;
    if (strcmp(command, "FF04") == 0) return AC_OK;
    return UNKNOWN_AC_COMMAND;
}

void processACCommand(char command[50]) {
    switch (getACCommands(command)) {
        case AC_OK:
            ESP_LOGI(TAG, "AC command received");
            break;
        case AC_ON_OFF:
            ESP_LOGI(TAG, "AC ON OFF command received");
            acON = !acON;
            publish_AC_telemetry(acON, acTemperature);
            break;
        case AC_TEMPERATURE_UP:
            ESP_LOGI(TAG, "AC temperature UP command received");
            if (acTemperature < 25) {
                acTemperature++;
            }

            publish_AC_telemetry(acON, acTemperature);
            break;
        case AC_TEMPERATURE_DOWN:
            ESP_LOGI(TAG, "AC temperature DOWN command received");
            if (acTemperature > 18) {
                acTemperature--;
            }

            publish_AC_telemetry(acON, acTemperature);
            break;
        default:
            ESP_LOGW(TAG, "Unknown command received");
    }
}
