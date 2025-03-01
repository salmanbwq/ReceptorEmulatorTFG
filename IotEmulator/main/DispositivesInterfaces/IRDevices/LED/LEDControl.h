//
// Created by salman on 1/03/25.
//

#ifndef LEDCONTROL_H
#define LEDCONTROL_H

typedef enum {
    LED_ON_OFF,
    LED_CHANGE_COLOR,
    UNKNOWN_LED_COMMAND
} LedCommands;

static LedCommands getLedCommands(const char *command);

void processLedCommand(char command[50]);
#endif //LEDCONTROL_H
