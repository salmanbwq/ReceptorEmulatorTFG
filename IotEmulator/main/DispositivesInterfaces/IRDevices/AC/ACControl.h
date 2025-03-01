//
// Created by salman on 1/03/25.
//

#ifndef ACCONTROL_H
#define ACCONTROL_H

typedef enum {
    AC_ON_OFF,
    AC_TEMPERATURE_UP,
    AC_TEMPERATURE_DOWN,
    AC_OK,
    UNKNOWN_AC_COMMAND
} ACCommands;

static ACCommands getACCommands(const char *command);

void processACCommand(char command[50]);
#endif //ACCONTROL_H
