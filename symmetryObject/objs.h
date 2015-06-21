#ifndef OBJS_H
#define OBJS_H

#include <list>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include "../tm/tm.h"
#include "./../tm/vector3.h"
#include "../publicDefines.h"
#include <cstdarg>


//namespace Graph2D {


class Prefix {
public:
	std::list<TYP> R;
	
	Prefix();
	Prefix(int n_args, ...);	// c-style funktion, tufft som fan med allt som härstammar från c

	int findInList();
	void simplify();
	void simplify(int centered);
	void print() const;
	void rotate(TYP r);
	void rotate(const Prefix &r);
	int size() const;
	Prefix getInverse() const;
	bool equalTo(Prefix &A);
	bool isOppositeEdge();
	void returnToRoot();

	TYP operator[](int i);
	Prefix operator*(const Prefix &A) const;
	Prefix operator/(const Prefix &A) const;

	Prefix difference(Prefix A);
	std::string toString() const;
};




struct Point {
	Prefix Pfx;
	int index;

	Point();
	Point(int index_);
	Point(Prefix Pfx_, int index_);
	bool isDefined() const;
	void print() const;
	void simplify();
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
	int type;

	face();
	face(int fr_, int edges_, int type_);

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
private:

protected:
	bool facePointActive;
	bool edgePointActive;
	bool vertexPointActive;

public:
	static const VEC vertexCenteredPoint;//(.0, .0);
	static const VEC edgeCenteredPoint;//(.25, SIN60*.5);
	static const VEC faceCenteredPoint;//(.5, .25/COS30);


	std::vector<VEC> V;
	std::vector<edge> E;
	std::vector<face> F;


	SymmetryPattern pat;

	SymmetryObject();
	SymmetryObject(const SymmetryObject *gammal);

	static std::vector<edge> E_ToBe;
	std::vector<Point> newEP;


////////////////////////////////////
		// AB är koordinater på skärmen, pixelposition

		// returnerar 
		// 0 om det är felaktigt
		// 1 om det är ok men icke sluten
		// 2 om face är ok.
	int checkE_ToBe();		// returnerar true om face är slutet.
	int checkNewEP();		// returnerar true om face är slutet.
	int getEnclosedPoints(VEC *A, std::list<Point> &PntList);
	bool addFaceToBe(int sluten);
	bool addNewEP(int sluten);
	//int vecOverIndex(VEC co_, );



	// returns -1 if over none, radius = pixel-radius
	Point getClosestPointFromWC(VEC co_);
	void insertCenteredVertex(int index_);
	int insertVertex(VEC coord_);	//vertex, vector, point, position, coordinate-> använd vettiga beteckningar 
	void getAllFromRoots(VEC vRoot_, VEC *vAll_);
	bool getCenteredActive(int whichCenteredIndex);
	void display();
	void printAll();
	bool fullTest();
	VEC getWcFromPoint(Point P_);
		// ta bort detta och skapa en hel ärvd klass
		// som testar allt senare
	static void test();
};



class Orientation {
private:

	static MAT FP120, FN120;
	static MAT FP60, FN60;
	
	static MAT VPT, VNT;	// Rotational Matrix for vertice-rotation on Tetrahedral symmetry
	static MAT VPO, VNO;	// -||- 	Octahedral symmetry
	static MAT VPI, VNI;	// -||- 	Icosahedral symmetry

	VEC pos;
	MAT ori; // flytta dessa till private

	SymmetryPattern pat;

	void initalize(SymmetryPattern createWithPattern);

public:

	Orientation();
	Orientation(SymmetryPattern createWithPattern);

	void rotate(const Prefix &p_);
	void rotate(TYP);
	VEC getOCFromWC(VEC A) const;		// get orientation coordinates from world coordinates
	VEC getWCFromOC(VEC A) const;		// get world coordinates from orientation coordinates
	static VEC getRootOCFromWC(VEC A);
	static VEC getRootWCFromOC(VEC A);
	SymmetryPattern getPattern() const;
	//VEC getWCRootFromOC(VEC A) const;
	void print() const;
	static void test();

	//static VEC getWCFromRootOC(VEC A);
};

void rotationPrint(TYP);
Prefix getPrefixFromWC(VEC coord);
VEC getRootpoint(VEC coord);
Point getClosestCenteredPointFromWC(VEC coord, double *distanceSquared);
Point getClosestCenteredPointFromWC(VEC coord, double *distanceSquared, bool vertexPointActive, bool edgePointActive, bool facePointActive);

#endif
