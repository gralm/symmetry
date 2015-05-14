/*
#include <iostream>
#include <list>
#include <vector>

#include "../../graph/graph2d.h"

using namespace Graph2D;

int main()
{
	std::cout << "tjena" << std::endl;

	double a = 0.3;
	insertVertex(point(0.28, 0.15));
	insertVertex(point(0.4, 0.2));
	insertVertex(point(0.15, 0.1));

	point ps[3];

	ps[0] = point(0.25, 0.05);
	ps[1] = point(0.65, 0.55);
	ps[2] = point(0.05, 0.75);

	std::list<Point> Plist;
	getEnclosedPoints(ps, Plist);

	std::cout << "Plist size: " << Plist.size() << std::endl;
	for (std::list<Point>::iterator itP = Plist.begin(); itP != Plist.end(); itP++) {
		itP->print();
		std::cout << std::endl;
	}
	
	return 0;
}*/

