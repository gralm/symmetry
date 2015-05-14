#ifndef TEST_SYMMETRY_OBJECT_H

#include "../publicDefines.h"
#include "../symmetryObject/objs.h"


class TestSymmetryObject: public SymmetryObject {
public:
	TestSymmetryObject();
	bool testaGetClosestPointFromWC();
};

#endif