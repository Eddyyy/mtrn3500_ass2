//Process 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "SMStructs.h"

int main()
{
	GPS Novatel;
	int shmid;
	key_t key;
	double* dblPtr;
	void* shm;
	key = 1234;
	if ((shmid = shmget(key, sizeof(GPS), IPC_CREAT | 0666)) < 0)
	{
		printf("Shared memory allocation failed\n");
		return -1;
	}
	if ((shm = shmat(shmid, NULL, 0)) == NULL)
	{
		printf("Shared memory attach failed\n");
		return -1;
	}

	//Novatel.Lat = 128402.293;
	//Novatel.Long = 29384.29394103;
	Novatel.Lat = 9043.25;
	Novatel.Long = 74443.3445346;
// Write to SM
	dblPtr = (double *)shm;
	*dblPtr = Novatel.Lat;
	*(dblPtr+1) = Novatel.Long;
	while(getchar()!= 'Q');
	shmdt(shm);
	return 0;
}
