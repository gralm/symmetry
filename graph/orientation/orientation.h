#ifndef TESTORIENTATION_H
#define TESTORIENTATION_H

#include <list>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
#include "./../../tm/vector3.h"

#define COS30			0.86602540378444
#define SIN30			0.5
#define COS60			0.5
#define SIN60			0.86602540378444
#define COS120			(-0.5)
#define SIN120			0.86602540378444
#define TAN30			0.57735026918962
#define TAN60			1.73205080756887

#define TYP		unsigned char
#define INV_ROTATION(r)		(r^1)

#define VP		2	// 010
#define VN		3	// 011
#define FP		4	// 100
#define FN		5	// 101

#define VERTEX_CENTERED		(-2)
#define EDGE_CENTERED		(-3)
#define FACE_CENTERED		(-4)



		// omvandla alla:	summa = 1023
	// VP, VN = 1 			0001 	0
	// VP, FP, VP = FN		0002 	1
	// VN, VP = 1 			0004 	2
	// VN, FN, VN = FP 		0008 	3
	// FP, VP, FP = VN		0010 	4
	// FP, FP = FN			0020 	5
	// FP, FN = 1 			0040 	6
	// FN, VN, FN = VP		0080  	7
	// FN, FN = FP			0100 	8
	// FN, FP = 1 			0200 	9

			// lägg in följande:
		// FP VP FN = FP VP FN FN FP = FP VP FP FP = 	VN FP
		// FP VN FN = FP FP FN VN FN = 					FN VP
		// FN VP FP = FN VP FP VP VN = FN FN VN = 		FP VN
		// FN VN FP = FN VN FP FP FN = FN VN FN FN = 	VP FN

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
	int getSize();
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
	VEC getpoint() const;
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

	static const unsigned char 	SP;		// positiv rotation i triangeln
	static const unsigned char 	SN;		// negativ rotation i triangeln
	static const unsigned char 	RP;		// positiv rotation till nästa triangel
	static const unsigned char 	RN;		// negativ rotation tlil föregående triangel

	//class point;
//	struct edge;

	static const VEC vertexCenteredPoint;//(.0, .0);
	static const VEC edgeCenteredPoint;//(.25, SIN60*.5);
	static const VEC faceCenteredPoint;//(.5, .25/COS30);

	//class face;

	std::vector<VEC> V;
	std::vector<edge> E;
	std::vector<face> F;


	//extern prefix preE_ToBe;
	static std::vector<edge> E_ToBe;

		// mode = 0:	Skapa Vertex
		// mode = 1:	Skapa Edges -> Faces
		// mode = 2:	Val av symmetri
		// mode = 3:	Välj energinivåer för relax 1.
		// mode = 4:	Kör relax 1.
		// mode = 5:	Välj energinivåer för relax 2.
		// mode = 6:	Kör relax 2.
		// mode = 7:	Resultat och Spara.



////////////////////////////////////
		// AB är koordinater på skärmen, pixelposition
	VEC fromABtoXY(int x, int y);
		//XY är transformerade koordinater, edgelängden = 1.0
	void fromXYtoAB(VEC XY, int *ABx, int *ABy);
		// returnerar 
		// 0 om det är felaktigt
		// 1 om det är ok men icke sluten
		// 2 om face är ok.
	int checkE_ToBe();		// returnerar true om face är slutet.
	int getEnclosedPoints(VEC *A, std::list<Point> &PntList);
	bool addFaceToBe(int sluten);
	int mouseOverIndex(VEC co_);
	// returns -1 if over none, radius = pixel-radius
	Point mouseOverPoint(VEC co_);
	Prefix getPrefix(VEC coord);
	VEC getRootpoint(VEC coord);
	void setMousePosition(int x, int y);
	void mouseClick(int x, int y);
	int insertVertex(VEC coord_);
	void insertLine(int x, int y);
	void drawPoint(VEC _P);
	void drawCircle(VEC _P, bool filled);
	void drawBrade();
	void drawedge(edge &e);
	void drawfaces();
	void getAllFromRoots(const VEC vRoot_, VEC *vAll_);
	void setColorOfVertex(int vert_, TYP str_);
	int setMode(int newModeVal);
	void display();

	void printAll();
	void drawFace(face &F_);
	void drawFaces();
	void setColorOfVertex(int vert_, double str_);

	static void test();

};




	extern std::vector<VEC> V;
	extern std::vector<edge> E;
	extern std::vector<face> F;

	class Orientation {
	public:
		VEC V;
		VEC F;

		Orientation();

		void rotate(Prefix p_);
		void rotate(TYP);
		VEC getOCFromWC(VEC A);		// get orientation coordinates from world coordinates
		VEC getWCFromOC(VEC A);		// get world coordinates from orientation coordinates
		VEC getWCRootFromOC(VEC A);
		void print();

		static VEC getWCFromRootOC(VEC A);
	};

	void rotationPrint(TYP);

//}

#endif
