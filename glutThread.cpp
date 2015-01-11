#include "glutThread.h"
#include "comm/comm.h"
#include "graph/graph2d.h"

using namespace std;

/*
    clock_t start = clock();
    Sleep(500);
*/


void init(void)
{
    /* select clearing (background) color */
    glClearColor (0.0, 0.0, 0.0, 0.0);

    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //void glOrtho(left,right,bottom,top,nearVal,farVal);

    Graph2D::xMin = -.5;
    Graph2D::xMax = 1.0;
    Graph2D::yMin = -.5;
    Graph2D::yMax = 1.;

    glOrtho(Graph2D::xMin, Graph2D::xMax, 
            Graph2D::yMin, Graph2D::yMax, -1.0, 1.0);

    cout << "void init()" << endl;
}

void checkThreads()
{
    static CommMsg msg;
    int val = commGetMsg(COMM_THREAD_GLUT, &msg);

    switch(val)
    {
        case COMM_RET_ID_OK:
            cout << "it was ok" << endl;
            break;
        case COMM_RET_ID_MISSING:   // inget meddelande har anlänt.
            return;
        default:
            cout << "det blev no annat: " << val << endl;
            return;
    }

    switch(msg.msgTyp)
    {
        case COMM_MSGTYP_EXIT:
            cout << "nu ska glutThreaden dödas" << endl;
            glutLeaveMainLoop();
            break;
        case COMM_MSGTYP_PAUSE:
            cout << "nu ska glutThreaden pausas" << endl;
            break;
        case COMM_MSGTYP_CHOOSE_VERTEX:{
            int *val = (int*)(msg.data);
            cout << "nu ska en vertex choooosas: " << *val << endl;
            Graph2D::vertexChosen = *val;
            delete[] val;
            break;}
        case COMM_MSGTYP_SET_MODE: {
            int *val = (int*)(msg.data);
            cout << "nu choooosas ett nytt val i glutThread.cpp: " << *val << endl;
            Graph2D::mode = *val;
            delete val;
            break;
        }
        default:
            cout << "nu ska glutThreaden göra något annat" << endl;
            break;
    }
    msg.destroy();

}



void idleFunc()
{
    static int var = 0;   
    int time = glutGet(GLUT_ELAPSED_TIME);
    static int time_graph = time;
    static int time_phys = time;
    static int time_print = time;
    static int time_thread = time;

    if (time > time_graph + UPD_GRAPH) {
        glutPostRedisplay();
        time_graph += UPD_GRAPH;

        if (var&1){
            cout << "too high graph-update" << endl;
            time_graph = time;
        }
        var |= 1;
    } else
        var &= ~1;


    if (time > time_phys + UPD_PHYS) {
        time_phys += UPD_PHYS;

        if (var&2){
            cout << "too high phys-update" << endl;
            time_phys = time;
        }
        var |= 2;
    } else
        var &= ~2;


    if ((time > time_print + UPD_PRINT) && UPD_PRINT) {
        time_print += UPD_PRINT;

        if (var&4){
            cout << "too high print-update" << endl;
            time_print = time;
        }
        var |= 4;
    } else
        var &= ~4;



    if ((time > time_thread + UPD_THREAD) && UPD_THREAD) {
        time_thread += UPD_THREAD;
        checkThreads();

        if (var&8){
            cout << "too high print-update" << endl;
            time_thread = time;
        }
        var |= 8;
    } else
        var &= ~8;
}


void mouseMotionFunc(int x, int y)
{
}

void mousePassiveMotionFunc(int x, int y)
{
    Graph2D::setMousePosition(x, y);

}


void mouseFunc(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_UP){
                //Graph2D::insertVertex(x, y);
                Graph2D::mouseClick(x, y);
            }
            cout << "Left button \t";
            break;

        case GLUT_MIDDLE_BUTTON:
            cout << "Middle button \t";
            break;

        case GLUT_RIGHT_BUTTON:
            cout << "Right button \t";
            break;
    }

    switch (state) {
        case GLUT_UP:
            cout << "Up";
            break;
        case GLUT_DOWN:
            cout << "Down";
            break;
    }

    cout << "\t at [" << x << ", " << y << "]" << endl;
}


void keyboardFunc(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
        case 'q':
            //exit(0);
            glutLeaveMainLoop();
            break;
        case GLUT_KEY_LEFT:
        case GLUT_KEY_DOWN:
        case GLUT_KEY_PAGE_DOWN:
            cout << "down" << endl;
            break;
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_PAGE_UP:
        case GLUT_KEY_UP:
            cout << "up" << endl;
            break;
        break;
            default:
        break;
    }
    cout << "key: " << key << endl;
}

void reshapeFunc(int width, int height)
{
    Graph2D::scrWidth = width;
    Graph2D::scrHeight = height;
    glViewport(0,0,width,height);
}

void *glutThreadFunc(void *threadid)
{
    int argc = 1;
    char **argv = 0;

    cout.precision(9);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    Graph2D::scrWidth = 300;
    Graph2D::scrHeight = 300;

    glutInitWindowPosition(350, 100);
    glutCreateWindow("Hej");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    //glutCloseFunc(closeGlut);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    glutReshapeFunc(reshapeFunc);

    //void mouseMotionFunc(int x, int y);
    //void mousePassiveMotionFunc(int x, int y);
    glutMotionFunc(mouseMotionFunc);
    glutPassiveMotionFunc(mousePassiveMotionFunc);

    init();
    glutIdleFunc(idleFunc);
    //glutDisplayFunc(display);
    glutDisplayFunc(Graph2D::display);
    cout << "before main loop" << endl;
    glutMainLoop();
    cout << "den lyckades avsluta glutMainLoop()" << endl;

    CommMsg msg(COMM_THREAD_GLUT, COMM_THREAD_MAIN, COMM_MSGTYP_EXIT, 0, 0, 0);
    commSendMsg(&msg);
    
    pthread_exit(NULL);

    cout << "threadden är avslutad nu" << endl;
}

