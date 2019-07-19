#ifndef SMFUNCS_H
#define SMFUNCS_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <iostream>
#include <stdio.h>

#define TEMP_SHMEM_FILE "/tmp"

void * accuireSHMem(key_t keyId, size_t memSize);
void * accuireSHMemExl(key_t keyId, size_t memSize);

void releaseSHMem(void * shm);

#endif
