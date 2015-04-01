
#include "comm.h"

using namespace std;

pthread_mutex_t mtxThread = PTHREAD_MUTEX_INITIALIZER;

list<CommMsg> msgList;


int commSendMsg(const CommMsg *msg)
{

    int ret = pthread_mutex_lock(&mtxThread);
    if (ret != 0) {
        cout << "error" << endl;
        pthread_mutex_unlock(&mtxThread);
        return COMM_RET_ERROR;
    }

    char *data_ = new char[msg->dataSiz];
    memcpy(data_, msg->data, msg->dataSiz);

    cout << "commSendMsg, before send: ";
    /*for (const char *c = msg->data; c < msg->data + msg->dataSiz; c++)
        cout << "[" << *c << ", " << ((int) (*c)) << "], ";
    cout << endl;*/
    for (const char *c = msg->data; c < msg->data + msg->dataSiz; c++)
        cout << *c;
    cout << endl;
    CommMsg nyttMsg(msg->fromId, msg->toId, msg->msgTyp, msg->time, msg->dataSiz, data_);
    
    msgList.push_back(nyttMsg);
    //if (printar)      cout << "commSend pushbackar" << endl;
    cout << "nu har ett meddelande addats, antal meddelanden i kö: " << msgList.size() << endl;
    pthread_mutex_unlock(&mtxThread);
    //if (printar)      cout << "commSend mutex over" << endl;
    return COMM_RET_ID_OK;

}


int commGetMsg(int toThreadId, CommMsg *msg)
{
    int ret = pthread_mutex_lock(&mtxThread);
    if (ret != 0) {
        cout << "error" << endl;
        pthread_mutex_unlock(&mtxThread);
        return COMM_RET_ERROR;
    }
        // när man hämtar meddelande så hämtar man med minnet på samma minnesplats, ingen kopiering
    for (list<CommMsg>::iterator itL = msgList.begin(); itL != msgList.end(); itL++)
    {
        if (itL->toId == toThreadId)
        {
            cout << "nu gettas ett meddeland, antal meddelanden i lista: " << msgList.size() << endl;
            msg->fromId = itL->fromId;
            msg->toId = itL->toId;
            msg->msgTyp = itL->msgTyp;
            msg->time = itL->time;
            msg->dataSiz = itL->dataSiz;
            msg->data = itL->data;

            msgList.erase(itL);
            cout << "antal meddelanden i lista efter get: " << msgList.size() << endl;
            pthread_mutex_unlock(&mtxThread);
            return COMM_RET_ID_OK;
        }
    }
    
    pthread_mutex_unlock(&mtxThread);
    return COMM_RET_ID_MISSING;
}

int commPrintMsg()
{
    int ret = pthread_mutex_lock(&mtxThread);
    if (ret != 0) {
        cout << "error" << endl;
        pthread_mutex_unlock(&mtxThread);
        return COMM_RET_ERROR;
    }
    cout << "\t\tTHREAD-MESSAGES IN CUE = " << msgList.size() << endl;
    for (list<CommMsg>::iterator itCM = msgList.begin(); itCM != msgList.end(); itCM++)
    {
        cout << "From: ";
        for (int i=0; i<2; i++)
        {
            int id_ = (i==0? itCM->fromId: itCM->toId);
            switch(id_) {
                case COMM_THREAD_MAIN:
                    cout << "MAIN";
                    break;
                case COMM_THREAD_GLUT:
                    cout << "GLUT";
                    break;
                default:
                    cout << "unknown";
                    break;
            }
            if (i==0)   cout << " \tTo: ";
        }
        cout << "\t Message: " << '"';
        switch(itCM->msgTyp)
        {
            case COMM_MSGTYP_EXIT:
                cout << "Exit" << '"';
                break;
            case COMM_MSGTYP_PAUSE:
                cout << "Pause" << '"';
                break;
            case COMM_MSGTYP_CHOOSE_VERTEX:
                cout << "Choose Vertex" << '"';
                break;
            default:
                cout << "unknown message" << '"';
                break;
        }
        cout << "\t Data size: " << itCM->dataSiz << endl;
    }

    pthread_mutex_unlock(&mtxThread);
    return COMM_RET_ID_OK;
}

int commDestroyMsg()
{

    list<CommMsg>::iterator itL = msgList.begin();
    while (itL != msgList.end())
    {
        itL->destroy();
        itL = msgList.erase(itL);
    }

    return COMM_RET_ID_OK;
}
