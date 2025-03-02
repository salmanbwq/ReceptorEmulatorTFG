//
// Created by salman on 1/03/25.
//

#include "TVControl.h"

#include <esp_log.h>
#include <stdbool.h>
#include <string.h>
#include <DispositivesInterfaces/IRDevices/IRDevices.h>
#include <Mqtt/MqttHandler.h>

static int tvChannel = 0;
static int tvVolume = 0;
static bool tvOn = false;
static char *TAG = "TV Control";


static TVCommands getTVCommands(const char *command) {
    if (strcmp(command, "F708") == 0) return TV_ON_OFF;
    if (strcmp(command, "FF00") == 0) return TV_CHANNEL_UP;
    if (strcmp(command, "FE01") == 0) return TV_CHANNEL_DOWN;
    if (strcmp(command, "FD02") == 0) return TV_VOLUME_UP;
    if (strcmp(command, "FC03") == 0) return TV_VOLUME_DOWN;
    if (strcmp(command, "BB44") == 0) return TV_OK;
    return UNKNOWN_TV_COMMAND;
}


void processTVCommand(char command[50]) {
    switch (getTVCommands(command)) {
        case TV_ON_OFF:
            ESP_LOGI(TAG, "TV ON OFF command received");
            tvOn = !tvOn;
            if (!tvOn) {
                tvChannel = 0;
                tvVolume = 0;
            }
            publish_tv_telemetry(tvOn, tvChannel, tvVolume);
            break;
        case TV_VOLUME_UP:
            ESP_LOGI(TAG, "TV VOLUME_UP command received");
            if (tvOn) {
                if (tvVolume < 10) {
                    tvVolume++;
                }
            } else {
                tvVolume = 0;
            }


            publish_tv_telemetry(tvOn, tvChannel, tvVolume);
            break;
        case TV_VOLUME_DOWN:
            ESP_LOGI(TAG, "TV VOLUME_DOWN command received");

            if (tvOn) {
                if (tvVolume < 10) {
                    tvVolume++;
                }
            } else {
                tvVolume = 0;
            }

            publish_tv_telemetry(tvOn, tvChannel, tvVolume);
            break;
        case TV_OK:
            ESP_LOGI(TAG, "TV OK command received");
            break;
        case TV_CHANNEL_UP:
            ESP_LOGI(TAG, "TV CHANNEL_UP command received");
            if (tvOn) {
                if (tvChannel > 10) {
                    tvChannel = 0;
                } else {
                    tvChannel++;
                }
            } else {
                tvChannel = 0;
            }

            publish_tv_telemetry(tvOn, tvChannel, tvVolume);
            break;
        case TV_CHANNEL_DOWN:
            if (tvOn) {
                if (tvChannel == 0) {
                    tvChannel = 10;
                } else {
                    tvChannel--;
                }
            } else
                tvChannel = 0;

            publish_tv_telemetry(tvOn, tvChannel, tvVolume);
            ESP_LOGI(TAG, "TV CHANNEL_DOWN command received");
            break;
        default:
            ESP_LOGW(TAG, "Unknown command received");
    }
}
