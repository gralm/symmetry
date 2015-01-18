#ifndef POINT_H
#define POINT_H

//#include "testprefix.h"
#include <iostream>

#define COS30			0.86602540378444
#define SIN30			0.5
#define COS60			0.5
#define SIN60			0.86602540378444
#define COS120			(-0.5)
#define SIN120			0.86602540378444
#define TAN30			0.57735026918962
#define TAN60			1.73205080756887


namespace Graph2D {

	class point {
	public:
		double x;
		double y;

		point();
		point(double x_, double y_);
		point(const point &A_);
		void print();

		point operator~ ();

		point operator+ (point A);
		void operator+= (point A);

		point operator- (point A);
		void operator-= (point A);

		double operator* (point A);
		point operator* (double a);
		void operator*= (double a);
	};

}

#endif 