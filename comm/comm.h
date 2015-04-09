#ifndef COMM_H
#define COMM_H

#include <pthread.h>
#include <list>
#include <iostream>
#include "../publicDefines.h"

extern pthread_mutex_t mtxThread;

struct CommMsg {
	int fromId;		// från vilken thread som meddelande sänds från
	int toId;		// till vilken thread som meddelandet är ämnat för
	int msgTyp;		// vilken typ av meddelande som sänds
	int time;		// millisecond value när den är meddelande sparat, används inte i nuläget
	int dataSiz;	// storlek på det minne som ska kopieras
	// list<CommMsg>::iterator msgItem, senare ska denna automatiskt msgList.erase(msgItem) vid destroy(), blir bättre så.
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

	// allokerar minne till meddelande som sparas msgList
int commSendMsg(const CommMsg*);
	// msg->data måste frigöras när den använts. Detta kan göras med msg->destroy()
int commGetMsg(int toThreadId, CommMsg *msg);
int commPrintMsg();
int commDestroyMsg();

#endif
