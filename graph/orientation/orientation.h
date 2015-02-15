#ifndef TESTORIENTATION_H
#define TESTORIENTATION_H

#include <list>
#include <iostream>

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
#define VN		3	// 110
#define FP		4	// 001
#define FN		5	// 101


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

	class point {
	public:
		double x;
		double y;

		point();
		point(double x_, double y_);
		point(const point &A_);
		void print();

		bool defined() const;

		//Prefix getPrefix();

		point operator~ ();

		point operator+ (point A);
		void operator+= (point A);

		point operator- (point A);
		void operator-= (point A);

		double operator* (point A);
		point operator* (double a);
		void operator*= (double a);
	};

	class Prefix {
	public:
		std::list<TYP> R;
		
		Prefix();

		int findInList();
		void simplify();
		void print();
		bool rotate(TYP r);
		bool rotate(Prefix &r);
		int getSize();
		Prefix getInverse();

		TYP operator[](int i);

		Prefix difference(Prefix A);
	};

	class Orientation {
	public:
		point V;
		point F;

		Orientation();

		void rotate(Prefix p_);
		void rotate(TYP);
		point getOCFromWC(point A);		// get orientation coordinates from world coordinates
		point getWCFromOC(point A);		// get world coordinates from orientation coordinates
		point getWCRootFromOC(point A);
		void print();

		static point getWCFromRootOC(point A);
	};

	void rotationPrint(TYP);

}

#endif
