
#include <errno.h>
#include <string.h>

#include "SMFuncs.h"

using namespace std;

void * accuireSHMem(int memId, size_t memSize) {
	int shmid;
	void* shm;
    key_t semKey;
    if ((semKey = ftok(TEMP_SHMEM_FILE, memId)) == (key_t) -1) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "IPC error: ftok" << std::endl;
        std::cerr << errorMsg << std::endl;
        return NULL;
    }
	if ((shmid = shmget(semKey, memSize, IPC_CREAT | 0666)) < 0) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "Shared memory allocation failed" << std::endl;
        std::cerr << errorMsg << std::endl;
		return NULL;
	}
	if ((shm = shmat(shmid, NULL, 0)) == NULL) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "Shared memory attach failed" << std::endl;
        std::cerr << errorMsg << std::endl;
		return NULL;
	}
    return shm;
}

void * accuireSHMemExl(int memId, size_t memSize) {
	int shmid;
	void* shm;
    key_t semKey;
    if ((semKey = ftok(TEMP_SHMEM_FILE, memId)) == (key_t) -1) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "IPC error: ftok" << std::endl;
        std::cerr << errorMsg << std::endl;
        return NULL;
    }
	if ((shmid = shmget(semKey, memSize, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "Shared memory allocation failed" << std::endl;
        std::cerr << errorMsg << std::endl;
		return NULL;
	}
	if ((shm = shmat(shmid, NULL, 0)) == NULL) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "Shared memory attach failed" << std::endl;
        std::cerr << errorMsg << std::endl;
		return NULL;
	}
    return shm;
}

void releaseSHMem(void * shm) {
	shmdt(shm);
}
