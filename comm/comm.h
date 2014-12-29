#ifndef COMM_H
#define COMM_H

#include <pthread.h>
#include <list>
#include <iostream>

	// för structarna
#define COMM_THREAD_GLUT		4	
#define COMM_THREAD_MAIN		3

#define COMM_MSGTYP_EXIT		0
#define COMM_MSGTYP_PAUSE		1

	// return values messages (msg)
#define COMM_ID_CREATED			11
#define COMM_ID_OVERWRITTEN		12
#define COMM_ID_MISSING			13
#define COMM_ERROR_				14
#define COMM_ID_OK				15
#define COMM_EXIT				16

	// avsändare och mottagare (frto)
#define COMM_GLUT_TO_GUI		1
#define COMM_GUI_TO_GLUT		2

extern pthread_mutex_t mtxThread;


struct CommMsg {
	int fromId;		// från vilken thread som meddelande sänds från
	int toId;		// till vilken thread som meddelandet är ämnat för
	int msgTyp;		// vilken typ av meddelande som sänds
	int time;		// millisecond value när den är meddelande sparat
	int dataSiz;
	char *data;

	CommMsg() {}
	CommMsg(int fromId_, int toId_, int msgTyp_, int time_, int dataSiz_, char *data_)
	{
		fromId = fromId_;	toId = toId_; 	msgTyp = msgTyp_;
		time = time_;	dataSiz = dataSiz_; 	data = data_;
	}

	void set(int fromId_, int toId_, int msgTyp_, int time_, int dataSiz_, char *data_)
	{
		fromId = fromId_;	toId = toId_; 	msgTyp = msgTyp_;
		time = time_;	dataSiz = dataSiz_; 	data = data_;
	}

	void destroy()	{if (dataSiz && data) 	delete[] data;}
};

int commSendMessage(int siz, int id, const void *mem);
int commGetMessage(int id, int &siz, void *mem);
int commDestroyMessages();

int commSendMsg(const CommMsg*);
int commGetMsg(int toThreadId, CommMsg*);
int commPrintMsg();

#endif