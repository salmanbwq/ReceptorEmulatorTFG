//
// Created by salman on 31/10/24.
//

#ifndef MAINRF_H
#define MAINRF_H
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include <IR/IREncoder/ir_nec_encoder.h>

typedef struct {
    TaskHandle_t *receiveTaskHandle;
    bool *completed;
    char *message;
    float *frequency;
} RFStatus;


static RFStatus rf_status;


/**
 * Method to initialize the RF module
 */
void initRF();

/**
 * Method to receive via RF
 */
void receiveRF(void *pvParameter);

void setData(char *command, float frequency);

uint8_t *getData();

void setFrecuency(const float frequency);


bool hasFinishReceiving();

#endif //MAINRF_H
