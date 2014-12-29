#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <cmath>
#include <iostream>
#include <vector>

#define TYP			double

#define COS30			0.86602540378444
#define SIN30			0.5
#define COS60			0.5
#define SIN60			0.86602540378444


namespace Graph2D {
	extern const unsigned char 	SP;
	extern const unsigned char 	SM;
	extern const unsigned char 	RP;
	extern const unsigned char 	RM;

	extern int scrWidth;
	extern int scrHeight;

	struct point;
	void fromXYtoAB(point XY, int *ABx, int *ABy);
	point fromABtoXY(int x, int y);

/*
	//std::vector<Graph2D::Point> vect;
		// AB är koordinater på skärmen


		//XY är transformerade koordinater, edgelängden = 1.0
	void fromXYtoAB(point2D XY, int *ABx, int *ABy);
*/
}
#endif
