
#include "SMFuncs.h"

using namespace std;

void * accuireSHMem(int memId, size_t memSize) {
    key_t semKey;
	int shmid;
	void* shm;
    
    //Retrieve a unique key for the time this program was run
    if ((semKey = ftok(TEMP_SHMEM_FILE, memId)) == (key_t) -1) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "IPC error: ftok" << std::endl;
        std::cerr << errorMsg << std::endl;
        return NULL;
    }

    //Acquire memory at accquired semKey of size memSize
	if ((shmid = shmget(semKey, memSize, IPC_CREAT | 0666)) < 0) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "Shared memory allocation failed" << std::endl;
        std::cerr << errorMsg << std::endl;
		return NULL;
	}

    //Attach to shmid
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
    key_t semKey;
	int shmid;
	void* shm;

    //Retrieve a unique key for the time this program was run
    if ((semKey = ftok(TEMP_SHMEM_FILE, memId)) == (key_t) -1) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "IPC error: ftok" << std::endl;
        std::cerr << errorMsg << std::endl;
        return NULL;
    }

    //Acquire memory at accquired semKey of size memSize
	if ((shmid = shmget(semKey, memSize, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        char buffer[256];
        char * errorMsg = strerror_r(errno, buffer, 256);

        std::cerr << "Shared memory allocation failed" << std::endl;
        std::cerr << errorMsg << std::endl;
		return NULL;
	}

    //Attach to shmid
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
