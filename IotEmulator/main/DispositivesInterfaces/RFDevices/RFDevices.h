//
// Created by salman on 1/03/25.
//

#ifndef RFDEVICES_H
#define RFDEVICES_H

typedef enum {
    GARAGE_DOOR,
    ALARM,
    UNKNOWN_RF
} RFDispositive;

typedef enum {
    GD_OPEN,
    GD_CLOSE,
    GD_UNKNOWN_COMMAND
} GarageDoorCommands;

typedef enum {
    ALARM_ON,
    ALARM_OFF,
    ALARM_UNKNOWN_COMMAND
} AlarmCommands;


GarageDoorCommands getGarageDoorCommands(const char *command);

AlarmCommands getAlarmCommands(const char *command);

RFDispositive getRFDispositive(const char *dispName);

void processRFCommand(char *trama);


#endif //RFDEVICES_H
