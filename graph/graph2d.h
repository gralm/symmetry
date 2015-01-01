#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <cmath>
#include <iostream>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#define TYP			double

#define COS30			0.86602540378444
#define SIN30			0.5
#define COS60			0.5
#define SIN60			0.86602540378444
#define COS120			(-0.5)
#define SIN120			COS30


namespace Graph2D {

	extern const unsigned char 	SP;		// positiv rotation i triangeln
	extern const unsigned char 	SN;		// negativ rotation i triangeln
	extern const unsigned char 	RP;		// positiv rotation till nästa triangel
	extern const unsigned char 	RN;		// negativ rotation tlil föregående triangel

	extern bool facePointActive;
	extern bool vertexPointActive;

	extern int scrWidth;
	extern int scrHeight;


	extern TYP xMin;
	extern TYP xMax;
	extern TYP yMin;
	extern TYP yMax;

	extern int mouseX;
	extern int mouseY;

	struct prefix;
	struct point;
	struct Point;
	struct edge;
	struct Edge;
	struct direction;

		//XY är transformerade koordinater, edgelängden = 1.0
	void fromXYtoAB(point XY, int *ABx, int *ABy);
	point fromABtoXY(int x, int y);
	void display();
	point getRootPoint(point);
	void printDirection(int x, int y);
}
#endif
