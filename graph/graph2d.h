#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include "./orientation/orientation.h"

#include <cmath>
#include <iostream>
#include <vector>
#include "../comm/comm.h"
#include <string>
#include <sstream>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>


#define COS30			0.86602540378444
#define SIN30			0.5
#define COS60			0.5
#define SIN60			0.86602540378444
#define COS120			(-0.5)
#define SIN120			0.86602540378444
#define TAN30			0.57735026918962
#define TAN60			1.73205080756887



extern Graph2D symmetryObject;

extern bool facePointActive;
extern bool edgePointActive;
extern bool vertexPointActive;


extern int scrWidth;
extern int scrHeight;


extern double xMin;
extern double xMax;
extern double yMin;
extern double yMax;

extern int mouseX;
extern int mouseY;

extern int indexChosen;
extern int indexMouseOver;


extern int mode;

extern int symmetryType;


int setMode(int newModeVal);
void setMousePosition(int x, int y);
VEC getRootpoint(VEC coord);
VEC fromABtoXY(int x, int y);
	//XY är transformerade koordinater, edgelängden = 1.0
void fromXYtoAB(VEC XY, int *ABx, int *ABy);




Prefix getPrefix(VEC coord);
void mouseClick(int x, int y);

#endif




// todo

/*
1.	Byt namn på RP,RN,SP,SN till VCPR VCNR, FCPR, FCNR
	= Vertice/Face-Centered-Positive/Negative-Rotation

2. 	

3.	Dela upp struktarna i olika filer så att graph2d blir
	något mer läsbar.

4.	
*/



