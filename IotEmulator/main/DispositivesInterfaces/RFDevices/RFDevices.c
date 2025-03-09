//
// Created by salman on 1/03/25.
//

#include "RFDevices.h"

#include <string.h>


GarageDoorCommands getGarageDoorCommands(const char *command) {
    if (strcmp(command, "OpenGarageDoor") == 0) return GD_OPEN;
    if (strcmp(command, "CloseGarageDoor") == 0) return GD_CLOSE;
    return GD_UNKNOWN_COMMAND;
}

AlarmCommands getAlarmCommands(const char *command) {
    if (strcmp(command, "PowerOn") == 0) return ALARM_ON;
    if (strcmp(command, "PowerOff") == 0) return ALARM_OFF;
    return ALARM_UNKNOWN_COMMAND;
}

RFDispositive getRFDispositive(const char *dispName) {
    if (strcmp(dispName, "Alarm") == 0) return ALARM;
    if (strcmp(dispName, "Garage") == 0) return GARAGE_DOOR;
    return UNKNOWN_RF;
}
