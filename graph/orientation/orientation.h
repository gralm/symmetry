#ifndef TESTORIENTATION_H
#define TESTORIENTATION_H

#include <list>
#include "point.h"
#include "prefix.h"


namespace Graph2D {

	class Orientation {
	public:
		point V;
		point F;

		Orientation();

		void rotate(Prefix p_);
		point getOCFromWC(point A);
		point getWCFromOC(point A);
		void print();
	};

}

#endif
