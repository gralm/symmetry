#include <iostream>
#include <pthread.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>


void *waitar(void *threadid);


void *waitar2(void *threadid);