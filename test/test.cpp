#include "testSymmetryObject.h"

using namespace std;


bool qwer()
{
	return false;
}

int main() 
{
	TestSymmetryObject asdf;

	//list<(bool*)()> tjena;
	//tjena.push_back(testGetEnclosedPoints);
	list<bool(*)()> lista;
	bool (*hej)(void);
	hej = &qwer;
	lista.push_back(qwer);

	//testFunks.push_back(testGetEnclosedPoints);

	bool getClosestPointFromWc = asdf.testaGetClosestPointFromWC();
	cout << (getClosestPointFromWc? "\tAll tests passed": "\tFailed") << endl;
	bool getRootFrom = asdf.testGetAllFromRoots();
	bool getEclosedPoints = asdf.testGetEnclosedPoints();

	bool getEnclosedPoints = asdf.testGetEnclosedPoints2();


	return 0;
}
