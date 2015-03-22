#ifndef GLUTTHREAD_H
#define GLUTTHREAD_H

#include <iostream>
#include <pthread.h>
#include <time.h>
#include <cmath>

#include "symmetryObject/objs.h"
#include "comm/comm.h"
#include "graph/graph2d.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
//#include <GL/glew.h>

//#define UPD_update	t	   // each t miliseconds UPD_update is updated
#define UPD_GRAPH       25
#define UPD_PHYS        100
#define UPD_PRINT       1000
#define UPD_THREAD      100

//void *waitar(void *threadid);
void *glutThreadFunc(void *threadid);

#endif 