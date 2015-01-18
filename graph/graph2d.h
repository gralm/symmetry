#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include "./orientation/point.h"

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



#define TYP			double

#define COS30			0.86602540378444
#define SIN30			0.5
#define COS60			0.5
#define SIN60			0.86602540378444
#define COS120			(-0.5)
#define SIN120			0.86602540378444
#define TAN30			0.57735026918962
#define TAN60			1.73205080756887


namespace Graph2D {

	extern const unsigned char 	SP;		// positiv rotation i triangeln
	extern const unsigned char 	SN;		// negativ rotation i triangeln
	extern const unsigned char 	RP;		// positiv rotation till nästa triangel
	extern const unsigned char 	RN;		// negativ rotation tlil föregående triangel

	class point;
	struct edge;

	extern const point vertexCenteredPoint;//(.0, .0);
	extern const point edgeCenteredPoint;//(.25, SIN60*.5);
	extern const point faceCenteredPoint;//(.5, .25/COS30);

	extern bool facePointActive;
	extern bool edgePointActive;
	extern bool vertexPointActive;


	extern int scrWidth;
	extern int scrHeight;


	extern TYP xMin;
	extern TYP xMax;
	extern TYP yMin;
	extern TYP yMax;

	extern int mouseX;
	extern int mouseY;

	extern int vertexChosen;
	extern int vertexMouseOver;

	extern std::vector<point> V;
	extern std::vector<edge> E;


	//extern prefix preE_ToBe;
	extern std::vector<edge> E_ToBe;

		// mode = 0:	Skapa Vertex
		// mode = 1:	Skapa Edges -> Faces
		// mode = 2:	Val av symmetri
		// mode = 3:	Välj energinivåer för relax 1.
		// mode = 4:	Kör relax 1.
		// mode = 5:	Välj energinivåer för relax 2.
		// mode = 6:	Kör relax 2.
		// mode = 7:	Resultat och Spara.
	extern int mode;

	//struct prefix;

	struct prefix {
		unsigned char r[10];
		prefix();
		void update();
		void print();

		prefix operator-(prefix &A);

	};

/*
	struct point {
		TYP x;
		TYP y;

		point();
		point(TYP x_, TYP y_);
		point(const point &A_);
		void print();

		point operator+(point &A);
		void operator+=(point &A);
		point operator-(point &A);
		void operator-=(point &A);
		point operator*(TYP a);
		void operator*=(TYP a);
		TYP operator&(point &A);
	};*/

	//struct Point;
	struct Point {
		prefix P;
		int v;

		point getpoint();
		void print();
	};

	//struct edge;
	struct Edge {
		prefix P;
		int e;
	};

	struct edge {
		Point fr;
		Point to;

		Edge next;
		Edge prev;
		Edge oppo;

		void print();
	};

	struct direction {
		TYP Rx, Ry;
		TYP Sx, Sy;

		direction();

		void rotate(int dir_);
		point rotate(int dir_, point p_);
		void print();

	};

	point getRootPoint(point p_);

		// AB är koordinater på skärmen, pixelposition
	point fromABtoXY(int x, int y);

		//XY är transformerade koordinater, edgelängden = 1.0
	void fromXYtoAB(point XY, int *ABx, int *ABy);


		// returnerar true om face är slutet.
	int checkE_ToBe();

		// returns -1 if over none, radius = pixel-radius
	int mouseOverVertex(point co_);

	int mouseOverVertex(int x, int y);
	prefix getRootPrefix(point coord_);
	Point mouseOverVertex2(int x, int y);
	prefix getRootPrefix(point coord_);
	void setMousePosition(int x, int y);
	void mouseClick(int x, int y);
	int insertVertex(point coord_);
	void insertLine(int x, int y);
	void drawPoint(point _P);
	void drawCircle(point _P, bool filled);
	void drawBrade();
	void getAllFromRoots(const point vRoot_, point *vAll_);
	void setColorOfVertex(int vert_, TYP str_);
	void display();
}
#endif


// todo

/*
1.	Byt namn på RP,RN,SP,SN till VCPR VCNR, FCPR, FCNR
	= Vertice/Face-Centered-Positive/Negative-Rotation

2. 	Byt struct point till en 2d-klass eller utveckla funktioner i 
	den för att lättare hantera beräkningar

3.	Dela upp struktarna i olika filer så att graph2d blir
	något mer läsbar.

4.	Gör std::lista av prefix::r istället för en jälla array

*/