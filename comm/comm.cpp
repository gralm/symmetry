
#include "comm.h"

using namespace std;

pthread_mutex_t mtxThread = PTHREAD_MUTEX_INITIALIZER;

list<CommMsg> msgList;

struct Message {
	int siz;
	int id;
	char *mem;
	Message(int siz_, int id_, const void *mem_) 
	{
		mem = new char[siz_];
		memcpy(mem, mem_, siz_);
		siz = siz_;
		id = id_;
		cout << "messCreate created id: " << id << endl;
	}
	~Message() 
	{
		delete[] mem; 
		//cout << "messDelete deleted id: " << id << endl;
		id = siz = 0;
	}
	void cleanup()
	{
		delete[] mem; 
		//cout << "messDelete deleted id: " << id << endl;
	}
};

list<Message*> memList;

	// den här funktionen tar emot det meddelande som sänds från någon process
int commSendMessage(int siz, int id, const void *mem)
{
	bool printar = false;
    int ret = pthread_mutex_lock(&mtxThread);

    if (ret != 0) {
    	cout << "error i commSendMessage" << endl;
    	pthread_mutex_unlock(&mtxThread);
    	return COMM_ERROR_;
    }

    for (list<Message*>::iterator itL = memList.begin(); itL != memList.end(); itL++)
    {
    	if ((*itL)->id == id)
    	{
    		//if (printar)		cout << "commSend erasar old message: " << (*itL)->id << endl;
    		(*itL)->cleanup();
    		memList.erase(itL);
    		//if (printar)		cout << "commSend makar en new message med id: " << id << endl;
    		//if (printar)		cout << "reading up loud: " << ((const char*) mem) << endl;
    		memList.push_back(new Message(siz, id, mem));
    		//if (printar)		cout << "commSend pushbackar" << endl;
    		pthread_mutex_unlock(&mtxThread);
    		//if (printar)		cout << "commSend mutex over" << endl;
    		return COMM_ID_OVERWRITTEN;
    	}
    }
    //if (printar)		cout << "commSend makar ny id: " << id << endl;
   	//if (printar)		cout << "reading up loud: " << ((const char*) mem) << endl;
    memList.push_back(new Message(siz, id, mem));
    //if (printar)		cout << "commSend pushbackar" << endl;
    pthread_mutex_unlock(&mtxThread);
    //if (printar)		cout << "commSend mutex over" << endl;
    return COMM_ID_CREATED;
}

	//	den här funktionen hämtar det meddelande som id refererar till
int commGetMessage(int id, int &siz, void *mem)
{
	bool printar = false;
    int ret = pthread_mutex_lock(&mtxThread);

    for (list<Message*>::iterator itL = memList.begin(); itL != memList.end(); itL++)
    {
    	if ((*itL)->id == id)
    	{
    		//if (printar)		cout << "commGet Gettar message id: " << id << endl;
    		//if (printar)		cout << "commGet det som ska kopieras = " << ((char*) (*itL)->mem) << endl;
    		memcpy(mem, (*itL)->mem, (*itL)->siz);
    		//if (printar)		cout << "commGet det som returneras= " << ((char*) mem) << endl;
    		siz = (*itL)->siz;
    		(*itL)->cleanup();
    		memList.erase(itL);
			pthread_mutex_unlock(&mtxThread);
			//if (printar)		cout << "commGet Gettat: " << id << endl;
			return COMM_ID_OK;
    	}
    }
    siz = 0;
    pthread_mutex_unlock(&mtxThread);
    return COMM_ID_MISSING;
}

int commDestroyMessages()
{
    int ret = pthread_mutex_lock(&mtxThread);
	memList.clear();
        // fixa detta sen
    pthread_mutex_unlock(&mtxThread);
}

void printMessages()
{
	int ret = pthread_mutex_lock(&mtxThread);
    pthread_mutex_unlock(&mtxThread);
}

////////////////////////////////////////////
////// För struktarna ////////
//////////////////////////////////////////

int commSendMsg(const CommMsg *msg)
{
    int ret = pthread_mutex_lock(&mtxThread);
    if (ret != 0) {
        cout << "error" << endl;
        pthread_mutex_unlock(&mtxThread);
        return COMM_ERROR_;
    }

    char *data_ = new char[msg->dataSiz];
    memcpy(data_, msg->data, msg->dataSiz);
    CommMsg nyttMsg(msg->fromId, msg->toId, msg->msgTyp, msg->time, msg->dataSiz, data_);
    
    msgList.push_back(nyttMsg);
    //if (printar)      cout << "commSend pushbackar" << endl;
    pthread_mutex_unlock(&mtxThread);
    //if (printar)      cout << "commSend mutex over" << endl;
    return COMM_ID_OK;

}


int commGetMsg(int toThreadId, CommMsg *msg)
{
    int ret = pthread_mutex_lock(&mtxThread);
    if (ret != 0) {
        cout << "error" << endl;
        pthread_mutex_unlock(&mtxThread);
        return COMM_ERROR_;
    }
        // när man hämtar meddelande så hämtar man med minnet på samma minnesplats, ingen kopiering
    for (list<CommMsg>::iterator itL = msgList.begin(); itL != msgList.end(); itL++)
    {
        if (itL->toId == toThreadId)
        {
            msg->fromId = itL->fromId;
            msg->toId = itL->toId;
            msg->msgTyp = itL->msgTyp;
            msg->time = itL->time;
            msg->dataSiz = itL->dataSiz;
            msg->data = itL->data;

            msgList.erase(itL);

            pthread_mutex_unlock(&mtxThread);
            return COMM_ID_OK;
        }
    }
    
    pthread_mutex_unlock(&mtxThread);
    return COMM_ID_MISSING;
}

int commPrintMsg()
{
    int ret = pthread_mutex_lock(&mtxThread);
    if (ret != 0) {
        cout << "error" << endl;
        pthread_mutex_unlock(&mtxThread);
        return COMM_ERROR_;
    }
    return COMM_ID_OK;
}