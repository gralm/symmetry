#include <iostream>
#include <pthread.h>
#include <time.h>
#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
//#include <GL/glew.h>

#define UPD_GRAPH       50
#define UPD_PHYS        100
#define UPD_PRINT       1000

//void *waitar(void *threadid);
void *waitar2(void *threadid);