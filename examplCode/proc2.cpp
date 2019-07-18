//Process 2

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "SMStructs.h"
#include "SMFuncs.h"

using namespace std;

int main() {
	GPS Novatel;
	key_t key;
	GPS* gpsPtr;
	void* shm;
	key = 1234;
    if ((shm = accuireSHMem(key, sizeof(GPS))) == NULL) {
        return -1;
    }
// Read SM
	gpsPtr = (GPS*)shm;
	Novatel.Lat = gpsPtr->Lat;
	Novatel.Long = gpsPtr->Long;
	printf("Lat = %10.3f, Long = %10.3f \n", Novatel.Lat, Novatel.Long);
	releaseSHMem(shm);
	return 0;
}
