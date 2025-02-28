//
// Created by salman on 7/02/25.
//

#ifndef RECEIVEIR_H
#define RECEIVEIR_H
#include <IR/IRmanager.h>
static IrStatus *ir_status;

static char *result;

void receiveIR();

char *getIRStatus();

void deleteIRStatus(char *status);

bool hasFinished();
#endif //RECEIVEIR_H
