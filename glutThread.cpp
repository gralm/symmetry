#include "glutThread.h"
#include "comm/comm.h"

using namespace std;

/*
#define COMM_ID_CREATED         11
#define COMM_ID_OVERWRITTEN     12
#define COMM_ID_MISSING         13
#define COMM_ERROR              14
#define COMM_ID_OK              15
*/

void *waitar(void *threadid)
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
}

void closeGlut()
{

    cout << "den lyckades avsluta glutMainLoop() here instead" << endl;
    pthread_exit(NULL);
}

void *waitar2(void *threadid)
{
    int argc = 1;
    char **argv = 0;

    cout.precision(9);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hej");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    //glutCloseFunc(closeGlut);
    glutMainLoop();

    cout << "den lyckades avsluta glutMainLoop()" << endl;

    //pthread_exit(NULL);
}


/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MessageBox(NULL, "Goodbye, cruel world!", "Note", MB_OK);
    return 0;
}*/

/*
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    //gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
*glLoadIdentity();
    //glTranslatef(0.0, 0.0, -3.6);
}

void idle()
{
    static int var = -1;                     //Warning
        // Den här skiten måste köras här MITT I main-loopen för att programmet inte har någon
        //  uppfattning om vilken upplösning som används före glutMainLoop har startats. FAN!!!
    if (var==-1)
    {
        var = 0;
    }


    int time = glutGet(GLUT_ELAPSED_TIME);
    static int time_graph = time;
    static int time_phys = time;
    static int time_print = time;

    if (time > time_graph + UPD_GRAPH)
    {,
        Controller::update();
        glutPostRedisplay();
        time_graph += UPD_GRAPH;
        if (var&1){
            printf("too high graph-update\n");
            time_graph = time;
        }
        var |= 1;
    }else
        var &= ~1;


    if (time > time_phys + UPD_PHYS)
    {
        time_phys += UPD_PHYS;
        //Graphic::Update(UPD_PHYS * .001);

        if (var&2){
            printf("too high phys-update\n");
            time_phys = time;
        }
        var |= 2;
    }else
        var &= ~2;


    if ((time > time_print + UPD_PRINT) && UPD_PRINT)
    {
        time_print += UPD_PRINT;
        //Group::Skriv();

        if (var&4){
            printf("too high print-update\n");
            time_print = time;
        }
        var |= 4;
    }else
        var &= ~4;
}



int main(int argc, char** argv)
{

    cout.precision(9);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);


    bool hurgickdet1;

    TYP c = cos(0.7);
    TYP s = sin(0.7);

    ObjectFN *nyobj1 = World::addObjectFN(OBJ_TRUNCATED_ICOSAHEDRON, Vec(0, 0, 0), Vec(1, 1, 1)*0.1, Mat(1,0,0, 0,1,0, 0,0,1));

    int whichFace[7] = {1, 2, 0, 8, 0, 10, 0};
    nyobj1->tabort(whichFace, 2);
    nyobj1->test(1);


    Graphic::setCamera(Controller::getCameraPosition(), Controller::getCameraOrientation());
    Graphic::createTexture(64, 64);
    Graphic::createTexture("pics/meny.bmp");

//  skrivet för att få igång normalerna
    {
        GLfloat position0[] = {10.0, 1.0, 1.0, 0.5};
        GLfloat diffuse0[] = {1.0, 1.0, 0.0, 1.0}; // Id term - Red
        GLfloat specular0[] = {1.0, 1.0, 1.0, 1.0}; // Is term - White
        GLfloat ambient0[] = {0.1, 0.1, 0.1, 1.0}; // Ia term - Gray


        glClearColor(0.4, 0.4, 0.4, 1.0);

        // Enable lighting and the light we have set up
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);

        //Set lighting parameters
        glLightfv(GL_LIGHT0, GL_POSITION, position0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

            // andra ljsuet
        GLfloat position1[] = {-10.0, -2.0, 0.0, 1.0};
        GLfloat diffuse1[] = {0.0, 0.0, 1.0, 1.0}; // Id term - Red
        GLfloat specular1[] = {1.0, 1.0, 1.0, 1.0}; // Is term - White
        GLfloat ambient1[] = {0.1, 0.1, 0.1, 1.0}; // Ia term - Gray

        glLightfv(GL_LIGHT1, GL_POSITION, position1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
        glEnable(GL_LIGHT1);


        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Enable shading
        glShadeModel(GL_SMOOTH);

        glEnable(GL_AUTO_NORMAL);
        glEnable(GL_NORMALIZE);

 
    }

    glEnable(GL_TEXTURE_2D);
    //glDisable(GL_RESCALE_NORMAL)
    glutDisplayFunc(Graphic::display);
    glutIdleFunc(idle);
    //glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutIgnoreKeyRepeat(true);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(Controller::keyboard);
    glutMouseFunc(Controller::mouseFunc);
    glutMotionFunc(Controller::motionFunc);
    glutSpecialFunc(Controller::specialFunc);
    glutSpecialUpFunc(Controller::specialUpFunc);



    glutMainLoop();

    cout << "kommer man hit när man avslutar?" << endl;
    return 0;
}*/

