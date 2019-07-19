//Process 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <iostream>

#include "SMStructs.h"
#include "SMFuncs.h"

int main()
{
	GPS Novatel;
	key_t key;
	GPS* dblPtr;
	void* shm;
	key = 1234;
    if ((shm = accuireSHMem(key, sizeof(GPS))) == NULL) {
        return -1;
    }

	//Novatel.Lat = 128402.293;
	//Novatel.Long = 29384.29394103;
	Novatel.Lat = 9043.25;
	Novatel.Long = 74443.3445346;
// Write to SM
	dblPtr = (GPS *)shm;
	dblPtr->Lat = Novatel.Lat;
	dblPtr->Long = Novatel.Long;
	while(getchar()!= 'Q');
	shmdt(shm);
	return 0;
}
