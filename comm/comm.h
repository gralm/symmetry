#ifndef COMM_H
#define COMM_H

#include <pthread.h>
#include <list>
#include <iostream>

	// för thread-id
#define COMM_THREAD_MAIN		3
#define COMM_THREAD_GLUT		4	

	// Message typ
#define COMM_MSGTYP_EXIT					0
#define COMM_MSGTYP_PAUSE					1
#define COMM_MSGTYP_CHOOSE_VERTEX			2
#define COMM_MSGTYP_ADD_VERTEX				3
#define COMM_MSGTYP_ADD_CENTERED_VERTEX		4
#define COMM_MSGTYP_SET_MODE				5

#define COMM_MSGTYP_UPDATE_VERTEX			6
#define COMM_MSGTYP_UPDATE_EDGE				7
#define COMM_MSGTYP_UPDATE_FACE				8

	// return values messages (msg)
#define COMM_RET_ID_CREATED			11
#define COMM_RET_ID_OVERWRITTEN		12
#define COMM_RET_ID_MISSING			13
#define COMM_RET_ERROR				14
#define COMM_RET_ID_OK				15


extern pthread_mutex_t mtxThread;


struct CommMsg {
	int fromId;		// från vilken thread som meddelande sänds från
	int toId;		// till vilken thread som meddelandet är ämnat för
	int msgTyp;		// vilken typ av meddelande som sänds
	int time;		// millisecond value när den är meddelande sparat, används inte i nuläget
	int dataSiz;	// storlek på det minne som ska kopieras
	const char *data;

	CommMsg() {}

	CommMsg(int fromId_, int toId_, int msgTyp_) 
	{
		fromId = fromId_;	toId = toId_; 	msgTyp = msgTyp_;
		time = 0;	dataSiz = 0; 	data = 0;
	}

	CommMsg(int fromId_, int toId_, int msgTyp_, int time_, int dataSiz_, const char *data_)
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

int commSendMsg(const CommMsg*);
int commGetMsg(int toThreadId, CommMsg*);
int commPrintMsg();
int commDestroyMsg();

#endif