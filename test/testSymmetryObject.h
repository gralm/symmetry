#ifndef TEST_SYMMETRY_OBJECT_H

#include "../publicDefines.h"
#include "../symmetryObject/objs.h"
#include <list>


class TestSymmetryObject: public SymmetryObject {
public:
	TestSymmetryObject();
	bool testaGetClosestPointFromWC();
	bool testGetAllFromRoots();
	bool testGetEnclosedPoints();
};

#endif
