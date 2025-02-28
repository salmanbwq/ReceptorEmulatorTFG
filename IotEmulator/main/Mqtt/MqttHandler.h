//
// Created by salman on 28/02/25.
//

#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

void publish_telemetry(char *temperature, char *humidity);

void initMqtt(void);

#endif //MQTTHANDLER_H
