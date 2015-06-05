#include "./testSymmetryObject.h"

//class TestSymmetryObject: public SymmetryObject {

using namespace std;

TestSymmetryObject::TestSymmetryObject(): SymmetryObject()
{
	V.push_back(VEC(0.38, .08));	// 0
	V.push_back(VEC(0.39, -.12));	// 1
}

bool TestSymmetryObject::testaGetClosestPointFromWC()
{
	// click wc -> [.6, -.1], 	root: [.39, -.11], Point VN, FN (1)
	// click wc -> [.31, .24],  root: [.39, .05], POINT (0)
	// click wc -> [-.19, .57],  root: [.42, -.1], POINT VP FN (1)
	bool allTestsPassed = true;
	VEC wc[3];
	wc[0] = VEC(.6, -.1);
	wc[1] = VEC(.31, .24);
	wc[2] = VEC(-.19, .57);

	Point testResult[3];
	for (int i=0; i<3; i++)
		testResult[i] = getClosestPointFromWC(wc[i]);

	Prefix Pfx[3];
	Pfx[0].rotate(VN);
	Pfx[0].rotate(FN);
	Pfx[2].rotate(VP);
	Pfx[2].rotate(FN);

	Point testAnswer[3];

	testAnswer[0] = Point(Pfx[0], 1);
	testAnswer[1] = Point(Pfx[1], 0);
	testAnswer[2] = Point(Pfx[2], 1);

	// print results:
	for (int i=0; i<3; i++)
	{
		cout << i << ". testresult:\t";
		testResult[i].print();
		cout << "\tcorrect answer: ";
		testAnswer[i].print();
		bool individualTest = testResult[i].equalTo(testAnswer[i]);
		if (!individualTest)
			allTestsPassed = false;
		cout << (individualTest == true? "\tTest Passed": "\tTest Failed") << endl;
		cout << "difference: " << getWcFromPoint(testAnswer[i]) - wc[i] << endl;
	}


	cout << "Tjena print: ";
	Prefix Tjena(4, VN,FN,FP,VN);
	Tjena.print();
	cout << endl;

	return allTestsPassed;
}

bool TestSymmetryObject::testGetAllFromRoots(){
	VEC jox(1./sqrt(3), 0);
	VEC nio[9];
	VEC dist;
	double h2 = 0.000001;
	this->getAllFromRoots(jox, nio);
	for (int i=0; i<9; i++)
	{
		dist = nio[i] - nio[((i+1)%3) + 3*(i/3)];
		if (dist*dist > h2)
			return false;
	}


	jox = VEC(sqrt(3.)/4, 1./4);
	this->getAllFromRoots(jox, nio);
	dist = nio[0]-nio[4];
	if (dist*dist > h2)
		return false;
	dist = nio[1]-nio[6];
	if (dist*dist > h2)
		return false;

	return true;
}




bool TestSymmetryObject::testGetEnclosedPoints()
{
	list<Point> PointList;
	Orientation ori(SYMMETRY_HEXAGONAL);
	Point A[3];
	VEC arrayWC[3];
	V.push_back(VEC(0.389, 0.072));	// 1

	Prefix pfx;
	pfx.rotate(FN);
	A[0] = Point(pfx, 0);
	cout << endl << getWcFromPoint(A[0]) << endl;
	A[0].print();


	pfx.returnToRoot();
	pfx.rotate(VP);
	pfx.rotate(FP);
	A[1] = Point(pfx, 0);
	cout << endl << getWcFromPoint(A[1]) << endl;
	A[1].print();

	pfx.returnToRoot();
	A[2] = Point(pfx, 0);
	cout << endl << getWcFromPoint(A[2]) << endl;
	A[2].print();



	for (int i=0; i<3; i++)
		arrayWC[i] = this->getWcFromPoint(A[i]);


	getEnclosedPoints(arrayWC, PointList);

	cout << "Enclosed Points: " << endl;
	int i = 0;
	for (list<Point>::iterator itP = PointList.begin(); itP != PointList.end(); itP++)
		cout << i++ << ":\t" << itP->toString() << endl;


	return PointList.size() == 1;
}


void TestSymmetryObject::testAll()
{

	//testFunks.push_back(&this->testGetEnclosedPoints);
	//list<(bool*)()> tjena;
	//tjena.push_back(testGetEnclosedPoints);
	//bool (*hej)(void);
	//hej = &testGetEnclosedPoints;

	//testFunks.push_back(testGetEnclosedPoints);
}



