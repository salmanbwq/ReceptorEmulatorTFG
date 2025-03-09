//
// Created by salman on 1/03/25.
//

#ifndef RFIDDEVICES_H
#define RFIDDEVICES_H
#include <Main.h>

typedef enum {
    ENTRANCE_DOOR,
    Elevator,
    UNKNOWN_RFID
} RFIDDispositive;

static RFIDDispositive getRFIDDispositive(const char *command);

void processRFIDCommand(char *receivedCmd);
#endif //RFIDDEVICES_H
