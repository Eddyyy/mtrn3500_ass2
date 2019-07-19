//Process 2
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
	PM Novatel;
	int shmid;
	key_t key;
	PM* pmPtr;
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
// Read SM
	pmPtr = (PM*)shm;
    std::cout << "PM Stat " << (int)pmPtr->Heartbeats.Flags.PM << "\t";
    std::cout << "GPS Stat " << (int)pmPtr->Heartbeats.Flags.GPS << "\t";
    std::cout << "Laser Stat " << (int)pmPtr->Heartbeats.Flags.Laser << std::endl;

	shmdt(shm);
	return 0;
}
