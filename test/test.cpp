#include "testSymmetryObject.h"

using namespace std;

int main() 
{
	TestSymmetryObject asdf;
	bool getClosestPointFromWc = asdf.testaGetClosestPointFromWC();
	cout << (getClosestPointFromWc? "\tAll tests passed": "\tFailed") << endl;
	return 0;
}