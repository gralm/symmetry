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