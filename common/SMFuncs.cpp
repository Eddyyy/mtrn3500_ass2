
#include "SMFuncs.h"

using namespace std;

void * accuireSHMem(key_t keyId, size_t memSize) {
	int shmid;
	void* shm;
	if ((shmid = shmget(keyId, memSize, IPC_CREAT | 0666)) < 0) {
        std::cout << "Shared memory allocation failed" << std::endl;
		return NULL;
	}
	if ((shm = shmat(shmid, NULL, 0)) == NULL) {
        std::cout << "Shared memory attach failed" << std::endl;
		return NULL;
	}
    return shm;
}

void * accuireSHMemExl(key_t keyId, size_t memSize) {
	int shmid;
	void* shm;
	if ((shmid = shmget(keyId, memSize, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        std::cout << "Shared memory allocation failed" << std::endl;
		return NULL;
	}
	if ((shm = shmat(shmid, NULL, 0)) == NULL) {
        std::cout << "Shared memory attach failed" << std::endl;
		return NULL;
	}
    return shm;
}

void releaseSHMem(void * shm) {
	shmdt(shm);
}
