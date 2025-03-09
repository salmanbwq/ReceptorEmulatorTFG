//
// Created by salman on 28/02/25.
//

#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H
#include <stdbool.h>
#include <stdint.h>


void publish_telemetry(char *temperature, char *humidity, char *topic);

void publish_tv_telemetry(bool power_status, int tv_channel, int tv_volume);

void publish_AC_telemetry(bool power_status, int temperature);

void publish_Lamp_telemetry(bool power_status, int colour);

void publishEntranceDoorStatus(bool status);

void publishElevatorStatus(bool status);

void initMqtt(void);

uint8_t getChannel();

#endif //MQTTHANDLER_H
