//
// Created by salman on 1/03/25.
//

#include "LEDControl.h"

#include <esp_log.h>
#include <stdbool.h>
#include <string.h>
#include <DispositivesInterfaces/IRDevices/IRDevices.h>

static char *TAG = "LED Control";

static bool ledON = false;

typedef enum {
    WHITE = 0,
    YELLOW = 1,
    BLUE = 2
} LEDColors;

static LEDColors ledColor = WHITE;

static LedCommands getLedCommands(const char *command) {
    if (strcmp(command, "FC01") == 0) return LED_ON_OFF;
    if (strcmp(command, "FC02") == 0) return LED_CHANGE_COLOR;
    return UNKNOWN_LED_COMMAND;
}


void processLedCommand(char command[50]) {
    switch (getLedCommands(command)) {
        case LED_ON_OFF:
            ESP_LOGI(TAG, "LED ON OFF command received");
            ledON = !ledON;
            break;
        case LED_CHANGE_COLOR:
            ESP_LOGI(TAG, "LED CHANGE_COLOR command received");
            ledColor = (ledColor+1) % 3;
            break;
        default:
            ESP_LOGI(TAG, "Unknown command received");
    }
}
