#ifndef COMM_H
#define COMM_H

#include <pthread.h>
#include <list>
#include <iostream>

#define COMM_ID_CREATED			11
#define COMM_ID_OVERWRITTEN		12
#define COMM_ID_MISSING			13
#define COMM_ERROR				14
#define COMM_ID_OK				15

extern pthread_mutex_t mtxThread;

int commSendMessage(int siz, int id, const void *mem);
int commGetMessage(int id, int &siz, void *mem);

#endif