//
// Created by salman on 28/02/25.
//

#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H
#include <stdbool.h>

void publish_telemetry(char *temperature, char *humidity, char *topic);

void publish_tv_telemetry(bool power_status, int tv_channel, int tv_volume);

void publish_AC_telemetry(bool power_status, int temperature);

void publish_Lamp_telemetry(bool power_status, int colour);

void initMqtt(void);

#endif //MQTTHANDLER_H
