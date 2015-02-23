#ifndef TESTORIENTATION_H
#define TESTORIENTATION_H

#include <list>
#include <vector>
#include <iostream>
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

namespace Graph2D {

	struct Edge;

	class Face;

	class face;

	struct edge;


	bool definedVec(VEC x);

	extern const VEC vertexCenteredPoint;//(.0, .0);
	extern const VEC edgeCenteredPoint;//(.25, SIN60*.5);
	extern const VEC faceCenteredPoint;//(.5, .25/COS30);


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
	};


	//struct Point;
	struct Point {
		Prefix Pfx;
		int index;

		Point();
		Point(Prefix Pfx_, int index_);
		VEC getpoint() const;
		void print() const;
		bool equalTo(Point &A);
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

}

#endif
