//
// Created by salman on 11/03/25.
//

#include "MqttPublishMethods.h"

#include <esp_log.h>
#include <mqtt_client.h>
#include <stdio.h>
#define MQTT_TOPIC "v1/devices/me/telemetry"
#include "MqttHandler.h"

static const char *TAG = "MQTT_PUBLISH_METHODS";


