#ifndef OBJS_H
#define OBJS_H

#include <list>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include "./../tm/vector3.h"
#include "../publicDefines.h"


//namespace Graph2D {


class Prefix {
public:
	std::list<TYP> R;
	
	Prefix();

	int findInList();
	void simplify();
	void print() const;
	void rotate(TYP r);
	void rotate(const Prefix &r);
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

	Edge();
	Edge(Prefix Pfx_, int index_);// {Pfx = Pfx_; index = index_;}
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
	Centered type;

	face();
	face(int fr_, int edges_, Centered type_);

	void print();

};

struct edge {
public:
	Point fr;
	Point to;

	Edge next;
	Edge prev;
	Edge oppo;

	edge();
	void print() const;
	bool isOppositeOf(const edge &e, Prefix *pfx);	// returns true if it is opposite. If pfx != null, pfx
};

class SymmetryObject {
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

	SymmetryObject();

	static std::vector<edge> E_ToBe;



////////////////////////////////////
		// AB är koordinater på skärmen, pixelposition

		// returnerar 
		// 0 om det är felaktigt
		// 1 om det är ok men icke sluten
		// 2 om face är ok.
	int checkE_ToBe();		// returnerar true om face är slutet.
	int getEnclosedPoints(VEC *A, std::list<Point> &PntList);
	bool addFaceToBe(Centered sluten);
	//int vecOverIndex(VEC co_, );



	// returns -1 if over none, radius = pixel-radius
	Point getClosestPoint(VEC co_);
	int insertVertex(VEC coord_);	//vertex, vector, point, position, coordinate-> använd vettiga beteckningar 
	void getAllFromRoots(const VEC vRoot_, VEC *vAll_);
	void display();
	void printAll();
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
