//
// Created by salman on 1/03/25.
//

#ifndef TVCONTROL_H
#define TVCONTROL_H

typedef enum {
    TV_ON_OFF,
    TV_VOLUME_UP,
    TV_VOLUME_DOWN,
    TV_OK,
    TV_CHANNEL_UP,
    TV_CHANNEL_DOWN,
    UNKNOWN_TV_COMMAND
} TVCommands;

static TVCommands getTVCommands(const char *command);

void processTVCommand(char command[50]);
#endif //TVCONTROL_H
