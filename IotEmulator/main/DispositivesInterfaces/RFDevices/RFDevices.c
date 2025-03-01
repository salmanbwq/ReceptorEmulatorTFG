//
// Created by salman on 1/03/25.
//

#include "RFDevices.h"

#include <string.h>


GarageDoorCommands getGarageDoorCommands(const char *command) {
    if (strcmp(command, "") == 0) return GD_OPEN;
    if (strcmp(command, "") == 0) return GD_CLOSE;
    return GD_UNKNOWN_COMMAND;
}

HeaterCommands getHeaterCommands(const char *command) {
    if (strcmp(command, "") == 0) return HEATER_ON_OFF;
    if (strcmp(command, "") == 0) return HEATER_UP;
    if (strcmp(command, "") == 0) return HEATER_DOWN;
    return HEATER_UNKNOWN_COMMAND;
}

BlindsCommands getBlindsCommands(const char *command) {
    if (strcmp(command, "") == 0) return BLINDS_UP;
    if (strcmp(command, "") == 0) return BLINDS_DOWN;
    return BLINDS_UNKNOWN_COMMAND;
}

RFDispositive getRFDispositive(const char *dispName) {
    if (strcmp(dispName, "") == 0) return HEATER;
    if (strcmp(dispName, "") == 0) return BLINDS;
    if (strcmp(dispName, "") == 0) return GARAGE_DOOR;
    return UNKNOWN_RF;
}
