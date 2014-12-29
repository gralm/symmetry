#include "glutThread.h"
#ifndef ONLYGLUT
    #include "comm/comm.h"
#endif

using namespace std;

/*
#define COMM_ID_CREATED         11
#define COMM_ID_OVERWRITTEN     12
#define COMM_ID_MISSING         13
#define COMM_ERROR              14
#define COMM_ID_OK              15
*/


void init(void)
{
    /* select clearing (background) color */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    cout << "void init()" << endl;
}

/*void *waitar(void *threadid)
{
    clock_t start = clock();
    char *str = new char[100];
    int siz = 0;
    int i=0;
    do {
        //nanosleep(&vtid, (struct timespec *) NULL);
        Sleep(500);
        switch(commGetMessage(13, siz, str))
        {
            case COMM_ID_OK:
            {
                cout << "Gick bra att getta message siz: " << siz << ", message: " << str << endl;
            }
            break;

            case COMM_ID_MISSING:
                cout << "Gick inte bra att getta message, it is missing " << endl;
                break;
            case COMM_ERROR:
                cout << "Gick inte bra att getta message, it is error " << endl;
                break;
            default:
                cout << "gick inte att getta, it is default" << endl;
                break;
        }
        i++;
    } while(clock() < 7000 + start);
    delete[] str;

    long tid = (long)threadid;
    cout << "Hello World! It's me, thread #" << tid << endl;
    cout << "Klockan was: " << start << "\t Klockan is: " << clock() << "\titers: " << i << endl;
    pthread_exit(NULL);
}*/

void display(void)
{
    /* clear all pixels */
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cout << ">";
    /* draw white polygon (rectangle) with corners at
    * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
    */
    static double t = 0.0;
    t += 0.01;
    //cout << ":";
    double c = cos(t);
    double s = sin(t);

    glColor3f (1.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.5 - .25*c, 0.25, .25*s);
        glVertex3f(0.5 + .25*c, 0.25, .25*s);
        glVertex3f(0.5 + .25*c, 0.75, -.25*s);
        glVertex3f(0.5 - .25*c, 0.75, -.25*s);
    glEnd();
    /* don't wait!
    * start processing buffered OpenGL routines
    */
    glutSwapBuffers();
}



void mouseFunc(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
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

void idleFunc()
{
    static int var = 0;   
    int time = glutGet(GLUT_ELAPSED_TIME);
    static int time_graph = time;
    static int time_phys = time;
    static int time_print = time;

    if (time > time_graph + UPD_GRAPH) {
        //Controller::update();
        glutPostRedisplay();
        time_graph += UPD_GRAPH;
        cout << "-";
        if (var&1){
            cout << "too high graph-update" << endl;
            time_graph = time;
        }
        var |= 1;
    } else
        var &= ~1;


    if (time > time_phys + UPD_PHYS) {
        time_phys += UPD_PHYS;
        //Graphic::Update(UPD_PHYS * .001);
        cout << ":";
        if (var&2){
            cout << "too high phys-update" << endl;
            time_phys = time;
        }
        var |= 2;
    } else
        var &= ~2;


    if ((time > time_print + UPD_PRINT) && UPD_PRINT) {
        time_print += UPD_PRINT;
        cout << ";";
        //Group::Skriv();

        if (var&4){
            cout << "too high print-update" << endl;
            time_print = time;
        }
        var |= 4;
    } else
        var &= ~4;

    //glutPostRedisplay();
}


void closeGlut()
{
    cout << "den lyckades avsluta glutMainLoop() here instead" << endl;
    pthread_exit(NULL);
}

//void *waitar2(void *threadid)

void *waitar2(void *threadid)
{
    int argc = 1;
    char **argv = 0;

    cout.precision(9);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("Hej");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    //glutCloseFunc(closeGlut);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    init();
    glutIdleFunc(idleFunc);
    glutDisplayFunc(display);
    cout << "before main loop" << endl;
    glutMainLoop();
    cout << "den lyckades avsluta glutMainLoop()" << endl;
    //pthread_exit(NULL);
    return 0;
}

