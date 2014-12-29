#ifndef COMM_H
#define COMM_H

#include <pthread.h>
#include <list>
#include <iostream>

	// messages (msg)
#define COMM_ID_CREATED			11
#define COMM_ID_OVERWRITTEN		12
#define COMM_ID_MISSING			13
#define COMM_ERROR				14
#define COMM_ID_OK				15
#define COMM_EXIT				16

	// avsändare och mottagare (frto)
#define COMM_GLUT_TO_GUI		1
#define COMM_GUI_TO_GLUT		2

extern pthread_mutex_t mtxThread;

int commSendMessage(int siz, int id, const void *mem);
int commGetMessage(int id, int &siz, void *mem);


#endif