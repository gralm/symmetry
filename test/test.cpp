#include "testSymmetryObject.h"

using namespace std;

int main() 
{
	TestSymmetryObject asdf;
	bool getClosestPointFromWc = asdf.testaGetClosestPointFromWC();
	cout << (getClosestPointFromWc? "\tAll tests passed": "\tFailed") << endl;

	bool getRootFrom = asdf.testGetAllFromRoots();


	bool getEclosedPoints = asdf.testGetEnclosedPoints();

	return 0;
}
