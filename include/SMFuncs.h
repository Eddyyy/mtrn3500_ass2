#ifndef SMFUNCS_H
#define SMFUNCS_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

#include <iostream>
#include <stdio.h>

#define TEMP_SHMEM_FILE "/home/eddy/Documents/uni_files/2019/T2/mtrn3500/mtrn3500_ass2/runProject.out"

void * accuireSHMem(key_t keyId, size_t memSize);
void * accuireSHMemExl(key_t keyId, size_t memSize);

void releaseSHMem(void * shm);

#endif
