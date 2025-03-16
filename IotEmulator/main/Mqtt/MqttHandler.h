//
// Created by salman on 28/02/25.
//

#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H
#include <esp_now.h>
#include <mqtt_client.h>
#include <stdbool.h>
#include <stdint.h>

void initMqtt(esp_now_recv_cb_t recv);


void log_to_mqtt(const char *message);

void publish_tv_telemetry(bool power_status, int tv_channel, int tv_volume);

void publish_AC_telemetry(bool power_status, int temperature);

void publish_Lamp_telemetry(bool power_status, int colour);

void publishEntranceDoorStatus(bool status);

void publishElevatorStatus(bool status);

void publishGarageStatus(bool status);

void publishAlarmStatus(bool status);


#endif //MQTTHANDLER_H
