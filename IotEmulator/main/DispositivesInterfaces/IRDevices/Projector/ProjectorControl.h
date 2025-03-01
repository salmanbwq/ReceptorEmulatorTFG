//
// Created by salman on 1/03/25.
//

#ifndef PROJECTORCONTROL_H
#define PROJECTORCONTROL_H

typedef enum {
    PROJECTOR_ON_OFF,
    UNKNOWN_PROJECTOR_COMMAND
} ProjectorCommands;

static ProjectorCommands getProjectorCommands(const char *command);

void processProjectorCommand(char command[50]);
#endif //PROJECTORCONTROL_H
