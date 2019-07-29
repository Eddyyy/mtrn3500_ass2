#ifndef SMFUNCS_H
#define SMFUNCS_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

#include <iostream>
#include <stdio.h>

void * accuireSHMem(key_t keyId, size_t memSize);
void * accuireSHMemExl(key_t keyId, size_t memSize);

void releaseSHMem(void * shm);

#endif
