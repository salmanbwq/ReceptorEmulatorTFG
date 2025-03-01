//
// Created by salman on 1/03/25.
//

#ifndef RFDEVICES_H
#define RFDEVICES_H

typedef enum {
    GARAGE_DOOR,
    HEATER,
    BLINDS,
    UNKNOWN_RF
} RFDispositive;

typedef enum {
    GD_OPEN,
    GD_CLOSE,
    GD_UNKNOWN_COMMAND
} GarageDoorCommands;

typedef enum {
    HEATER_ON_OFF,
    HEATER_UP,
    HEATER_DOWN,
    HEATER_UNKNOWN_COMMAND
} HeaterCommands;

typedef enum {
    BLINDS_UP,
    BLINDS_DOWN,
    BLINDS_UNKNOWN_COMMAND
} BlindsCommands;

GarageDoorCommands getGarageDoorCommands(const char *command);

HeaterCommands getHeaterCommands(const char *command);

BlindsCommands getBlindsCommands(const char *command);

RFDispositive getRFDispositive(const char *dispName);


#endif //RFDEVICES_H
