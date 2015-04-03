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

void *glutThreadFunc(void *threadid);

void convertAndSendMsgFromString(std::string str);

#endif 
