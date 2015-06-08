#ifndef TEST_SYMMETRY_OBJECT_H

#include "../publicDefines.h"
#include "../symmetryObject/objs.h"
#include <list>


class TestSymmetryObject: public SymmetryObject {
private:
	std::list<bool(*)()> testFunks;

public:
	TestSymmetryObject();
	bool testaGetClosestPointFromWC();
	bool testGetAllFromRoots();
	bool testGetEnclosedPoints();
	bool testGetEnclosedPoints2();

	void testAll();
};

#endif
