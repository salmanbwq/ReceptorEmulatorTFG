//
// Created by salman on 27/02/25.
//

#ifndef MAIN_H
#define MAIN_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

extern QueueHandle_t xQueue;  // Declaración global

typedef enum {
    CMD_RFID,
    CMD_IR,
    CMD_ESP_NOW
} command_type_t;

typedef struct {
    command_type_t cmd;
    char data[100];
} command_t;
#endif //MAIN_H
