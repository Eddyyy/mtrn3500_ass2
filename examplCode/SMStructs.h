#ifndef SMSTRUCTS_H
#define SMSTRUCTS_H

#include <stdint.h>

#define PM_KEY 128+0
#define GPS_KEY 128+1
#define LASER_KEY 128+2

#define PM_PROBE 0
#define PM_RESPONSE 1

#define SHUTDOWN_ALL 0xFF

typedef uint8_t flag_t;

struct ModuleFlags {
    flag_t PM:1,
           GPS:1,
           Laser:1,
           Xbox:1,
           Vehicle:1,
           Display:1,
           Unused:2;
};

union ExecFlags {
    flag_t Status;
    ModuleFlags Flags;
};

typedef struct PM {
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

typedef struct Vehicle {
    volatile double MeasuredSpeed;
    volatile double MeasuredSteering;
} Vehicle;

typedef struct Laser {
    volatile double XRange[600];
    volatile double YRange[600];
} Laser;


#endif
