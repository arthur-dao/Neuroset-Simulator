#ifndef DEFS_H
#define DEFS_H

typedef enum {ACTIVE, INACTIVE, PAUSED, DISCONNECTED} RunStatus;
typedef enum {MAIN, TREATING, SESSIONS, SETDATETIME} CurrentMenu;
typedef enum {ON, OFF} DeviceStatus;
typedef enum {CONNECT, PAUSE, STOP, DISCONNECT} Status;

//Size of lists
#define MAX_SIZE 21;

#endif // DEFS_H
