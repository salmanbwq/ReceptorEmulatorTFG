//
// Created by salman on 1/03/25.
//

#ifndef IRDEVICES_H
#define IRDEVICES_H
#include <Main.h>

typedef enum {
    TV,
    AC,
    LED,
    PROJECTOR,
    UNKNOWN_IR
} IRDispositive;

static IRDispositive getIRDispositive(const char *dispName);

void processIRCommand(command_t receivedCmd);

#endif //IRDEVICES_H
