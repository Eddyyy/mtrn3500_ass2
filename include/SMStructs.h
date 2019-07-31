#ifndef SMSTRUCTS_H
#define SMSTRUCTS_H

#include <stdint.h>

#define PM_KEY 0
#define GPS_KEY 1
#define LASER_KEY 2

#define PM_PROBE 0
#define PM_RESPONSE 1

#define SHUTDOWN_ALL 0xFF
#define STARTUP_RESET 0

typedef uint8_t flag_t;

struct ModuleFlags {
    flag_t PM:1,
           GPS:1,
           Laser:1,
           Xbox:1,
           Vehicle:1,
           Display:1,
           Dummy:1,
           Unused:1;
};

union ExecFlags {
    flag_t Status;
    ModuleFlags Flags;
};

typedef struct PM {
    volatile ExecFlags Started;
    volatile ExecFlags Heartbeats;
    volatile ExecFlags Shutdown;
} PM;

typedef struct GPS {
    volatile double Easting;
    volatile double Northing;
    volatile double Height;
	volatile double Lat;
	volatile double Long;
} GPS;

typedef struct Remote {
    volatile double SetSpeed;
    volatile double SetSteering;
} Remote;

typedef struct Laser {
    volatile double XRange[361];
    volatile double YRange[361];
} Laser;


#endif
