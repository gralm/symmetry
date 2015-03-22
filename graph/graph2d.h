#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP


#include "../publicDefines.h"
#include "../symmetryObject/objs.h"

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


class SymmetryDrawable: public Graph2D {
public:
	// public methods only!
	SymmetryDrawable();
	void drawPoint(VEC _P);
	void drawCircle(VEC _P, bool filled);
	void drawBrade();
	void drawedge(edge &e);
	void drawfaces();
	void drawFace(face &F_);
	void drawFaces();
	void setColorOfVertex(int vert_, double str_);
	void display();

};


//extern Graph2D symmetryObject;
extern SymmetryDrawable symmetryObject;
//SymmetryDrawable


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


		// mode = 0:	Skapa Vertex
		// mode = 1:	Skapa Edges -> Faces
		// mode = 2:	Val av symmetri
		// mode = 3:	Välj energinivåer för relax 1.
		// mode = 4:	Kör relax 1.
		// mode = 5:	Välj energinivåer för relax 2.
		// mode = 6:	Kör relax 2.
		// mode = 7:	Resultat och Spara.
extern int mode;

extern int symmetryType;


int setMode(int newModeVal);
void setMousePosition(int x, int y);
//VEC getRootpoint(VEC coord);
VEC fromABtoXY(int x, int y);
	//XY är transformerade koordinater, edgelängden = 1.0
void fromXYtoAB(VEC XY, int *ABx, int *ABy);
void graphDisplay();

void mouseClick(int x, int y);



#endif




/* todo

orientation/		symmetryObject/symmetryBaseClasses/
	cpoint.cpp			point.cpp
	prefix.cpp			prefix.cpp
	orientation.h 		symmetryBaseClasses.h
	orientetaion.cpp	orientation.cpp

					symmetryObject/
class.Graph2D			symmetricObject.cpp
						symmetricGraphicalObject.cpp
						symmetricSimulationObject.cpp


*/