//
// Created by salman on 7/02/25.
//

#ifndef RECEIVEIR_H
#define RECEIVEIR_H

static char *result;

void executeReceiveIR();

char *getIRStatus();

void deleteIRStatus(char *status);

bool hasFinished();
#endif //RECEIVEIR_H
