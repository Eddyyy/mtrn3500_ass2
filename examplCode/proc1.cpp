//Process 1
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "SMStructs.h"

using namespace std;

int main()
{
	int shmid;
	key_t key;
	PM* dblPtr;
	void* shm;
    if ((key = ftok("/tmp", 'a')) == (key_t) -1) {
        printf("IPC error: ftok");
        return -1;
    }
	if ((shmid = shmget(key, sizeof(PM), IPC_CREAT | 0666)) < 0)
	{
		printf("Shared memory allocation failed\n");
		return -1;
	}
	if ((shm = shmat(shmid, NULL, 0)) == NULL)
	{
		printf("Shared memory attach failed\n");
		return -1;
	}

// Write to SM
	dblPtr = (PM *)shm;
    dblPtr->Heartbeats.Status = 0x00;
	dblPtr->Heartbeats.Flags.PM = 1;
	dblPtr->Heartbeats.Flags.Laser = 0;
	dblPtr->Heartbeats.Flags.GPS = 1;
	while(getchar()!= 'Q');
	shmdt(shm);
	return 0;
}
