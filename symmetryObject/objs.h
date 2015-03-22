#ifndef TESTORIENTATION_H
#define TESTORIENTATION_H

#include <list>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include "./../tm/vector3.h"
#include "../publicDefines.h"
//#include "../graph/graph2d.h"


	// de här måste förflyttas sen
/*#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>*/

/*
#define COS30			0.86602540378444
#define SIN30			0.5
#define COS60			0.5
#define SIN60			0.86602540378444
#define COS120			(-0.5)
#define SIN120			0.86602540378444
#define TAN30			0.57735026918962
#define TAN60			1.73205080756887
*/


//namespace Graph2D {

struct Edge;

class Face;

class face;

struct edge;



class Prefix {
public:
	std::list<TYP> R;
	
	Prefix();

	int findInList();
	void simplify();
	void print() const;
	bool rotate(TYP r);
	bool rotate(const Prefix &r);
	int getSize() const;
	Prefix getInverse() const;

	TYP operator[](int i);
	Prefix operator*(const Prefix &A) const;

	Prefix difference(Prefix A);
	std::string toString() const;
};




//struct Point;
struct Point {
	Prefix Pfx;
	int index;

	Point();
	Point(Prefix Pfx_, int index_);
	//VEC getpoint() const;
	void print() const;
	std::string toString() const ;
	bool equalTo(Point &A);
};


bool definedVec(VEC x);

/*
	extern const VEC vertexCenteredPoint;//(.0, .0);
	extern const VEC edgeCenteredPoint;//(.25, SIN60*.5);
	extern const VEC faceCenteredPoint;//(.5, .25/COS30);
*/



//struct edge;
struct Edge {
	Prefix Pfx;
	int index;

	Edge() {}
	Edge(Prefix Pfx_, int index_) {Pfx = Pfx_; index = index_;}
	void print() const;
	bool isDefined() const;
	std::string toString() const;
};

class Face {
	Prefix P;
	int fr;
	int mode;
};

class face {
public:
	int fr;
	int edges;
	int type;

	face();
	face(int fr_, int edges_, int type_);

	void print();

};

struct edge {
	Point fr;
	Point to;

	Edge next;
	Edge prev;
	Edge oppo;

	void print() const;
	bool isOppositeOf(const edge &e, Prefix *pfx);	// returns true if it is opposite. If pfx != null, pfx
};

class Graph2D {
public:
	static const VEC vertexCenteredPoint;//(.0, .0);
	static const VEC edgeCenteredPoint;//(.25, SIN60*.5);
	static const VEC faceCenteredPoint;//(.5, .25/COS30);


	bool facePointActive;
	bool edgePointActive;
	bool vertexPointActive;

	std::vector<VEC> V;
	std::vector<edge> E;
	std::vector<face> F;

	Graph2D();

	static std::vector<edge> E_ToBe;



////////////////////////////////////
		// AB är koordinater på skärmen, pixelposition

		// returnerar 
		// 0 om det är felaktigt
		// 1 om det är ok men icke sluten
		// 2 om face är ok.
	int checkE_ToBe();		// returnerar true om face är slutet.
	int getEnclosedPoints(VEC *A, std::list<Point> &PntList);
	bool addFaceToBe(int sluten);
	//int vecOverIndex(VEC co_, );



	// returns -1 if over none, radius = pixel-radius
	Point getClosestPoint(VEC co_);
	//Point getClosestPoint2(VEC co_);
	int insertVertex(VEC coord_);	//vertex, vector, point, position, coordinate-> använd vettiga beteckningar 
	//void insertLine(int x, int y);

	void getAllFromRoots(const VEC vRoot_, VEC *vAll_);
	//void setColorOfVertex(int vert_, TYP str_);

	void display();

	void printAll();
	/*void drawPoint(VEC _P);
	void drawCircle(VEC _P, bool filled);
	void drawBrade();
	void drawedge(edge &e);
	void drawfaces();
	void drawFace(face &F_);
	void drawFaces();
	void setColorOfVertex(int vert_, double str_);*/

	VEC getVec(Point P_);
		// ta bort detta och skapa en hel ärvd klass
		// som testar allt senare
	static void test();
};



class Orientation {
public:
	VEC V;
	VEC F;

	Orientation();

	void rotate(const Prefix &p_);
	void rotate(TYP);
	VEC getOCFromWC(VEC A);		// get orientation coordinates from world coordinates
	VEC getWCFromOC(VEC A);		// get world coordinates from orientation coordinates
	VEC getWCRootFromOC(VEC A);
	void print();

	static VEC getWCFromRootOC(VEC A);
};

void rotationPrint(TYP);
Prefix getPrefix(VEC coord);
VEC getRootpoint(VEC coord);

#endif
