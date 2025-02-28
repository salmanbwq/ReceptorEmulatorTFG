//
// Created by salman on 10/11/24.
//

#ifndef IRMANAGER_H
#define IRMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint16_t address;
    uint16_t command;
    char *message;
} IrStatus;

static IrStatus *irStatus;

bool hasIRReceived();

void initializeReceiveIR(void);


void executeReceiveIR();


void initializeIRStatus(uint16_t address, uint16_t command);


#endif //IRMANAGER_H
