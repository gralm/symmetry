
#include "orientation.h"
#include <iostream>

using namespace std;


	//std::list<TYP> R;

namespace Graph2D {


		// Root orientation
	Prefix::Prefix()
	{

	}

	void Prefix::simplify()
	{
		list<TYP>::iterator it1, it2;
		unsigned char rots[3];
		int d;
		do {	// förenkla till det inte går att förenkla längre (d=-1)
			//print();
			if (R.size() < 2)
				return;
			d = -1;
			it2 = R.begin();
			for (list<TYP>::iterator it0 = R.begin(); it2 != R.end() || it0 == R.end(); it0++)
			{
				rots[0] = *it0;
				it1 = it0;
				it1++;
				
				rots[1] = *it1;
				it2 = it1;
				it2++;

				rots[2] = (it2 == R.end())? 0: *it2;


				if (rots[0] == VP) {
					if (rots[1] == VN) {
						d = 0;
						R.erase(it0, it2);
						break;
					}else if (rots[1] == FP && rots[2] == VP) {
						d = 1;
						it0 = R.erase(it0, ++it2);
						R.insert(it0, FN);
						break;
					}
				} else if (rots[0] == VN) {
					if (rots[1] == VP) {
						d = 2;
						R.erase(it0, it2);
						break;
					}else if (rots[1] == FN && rots[2] == VN) {
						d = 3;
						it0 = R.erase(it0, ++it2);
						R.insert(it0, FP);
						break;
					}
				} else if (rots[0] == FP) {
					if (rots[1] == VP && rots[2] == FP) {
						d = 4;
						it0 = R.erase(it0, ++it2);
						R.insert(it0, VN);
						break;
					} else if (rots[1] == FP) {
						d = 5;
						it0 = R.erase(it0, it2);
						R.insert(it0, FN);
						break;
					} else if (rots[1] == FN) {
						d = 6;
						R.erase(it0, it2);
						break;
					}
				} else if (rots[0] == FN) {
					if (rots[1] == VN && rots[2] == FN) {
						d = 7;
						it0 = R.erase(it0, ++it2);
						R.insert(it0, VP);
						break;
					} else if (rots[1] == FN) {
						d = 8;
						it0 = R.erase(it0, it2);
						R.insert(it0, FP);
						break;
					} else if (rots[1] == FP) {
						d = 9;
						R.erase(it0, it2);
						break;
					}
				}
			}
		} while(d >= 0);

		return;
	}

	void Prefix::print() const
	{
			/*
		cout << "Pfx [";
		for (list<TYP>::const_iterator it = R.begin(); it != R.end(); it++)
		{
			if (it != R.begin())
				cout << ", ";

			switch(*it)
			{
				case VP:
					cout << "VP";
					break;
				case VN:
					cout << "VN";
					break;
				case FP:
					cout << "FP";
					break;
				case FN:
					cout << "FN";
					break;
			}
		}
		cout << "]";// << endl;
		*/
		cout << toString();
	}


	bool Prefix::rotate(TYP r)
	{
		if (r & (VP|FP))
		R.push_back(r);
	}

	bool Prefix::rotate(const Prefix &r)
	{
		list<TYP>::const_iterator itr;
		itr = r.R.begin();
		//itr++;

		for (itr = r.R.begin(); itr != r.R.end(); itr++)
			R.push_back(*itr);
		simplify();
	}

	TYP Prefix::operator[](int i)
	{
		if (i<0 || i>=R.size())
			return 0;

		for (list<TYP>::iterator itR = R.begin(); itR != R.end(); itR++)
		{
			if (i-- == 0)
				return *itR;
		}

		cout << "Hit ska den ju inte heller komma, det here is sjuuuukt lixom!!!" << endl;
		return 0;
	}

	Prefix Prefix::operator*(const Prefix &A) const
	{
		Prefix toReturn;
		toReturn.R = R;

		toReturn.rotate(A);
		
		//for (list<TYP>::iterator it = A.R.begin(); it != A.R.end(); it++)
		//	toReturn.R.push_back( (*it));

		toReturn.simplify();

		return toReturn;
	}


	Prefix Prefix::difference(Prefix A)
	{
		Prefix toReturn;

			// dra bort sina egna:
		for (list<TYP>::reverse_iterator rit = R.rbegin(); rit != R.rend(); rit++)
			toReturn.R.push_back(INV_ROTATION(*rit));
		
			// sen lägg till A:s rotationer
		for (list<TYP>::iterator it = A.R.begin(); it != A.R.end(); it++)
			toReturn.R.push_back( (*it));
		
			// snygga till rotationerna
		toReturn.simplify();
		return toReturn;
	}


	int Prefix::getSize()
	{
		return R.size();
	}


	Prefix Prefix::getInverse() const
	{
		Prefix invPfx;
		for (list<TYP>::const_reverse_iterator rit = R.rbegin(); rit != R.rend(); rit++)
			invPfx.R.push_back(INV_ROTATION(*rit));
		return invPfx;
	}

	string Prefix::toString() const
	{
		stringstream returnString;
		returnString << "[";
		for (list<TYP>::const_iterator it = R.begin(); it != R.end(); it++)
		{
			if (it != R.begin())
				returnString << " ";

			switch(*it)
			{
				case VP:
					returnString << "VP";
					break;
				case VN:
					returnString << "VN";
					break;
				case FP:
					returnString << "FP";
					break;
				case FN:
					returnString << "FN";
					break;
			}
		}
		returnString << "]";// << endl;
		return returnString.str();
	}

			// omvandla alla:	summa = 1023
		// VP, VN = 1 			0001 	0
		// VP, FP, VP = FN		0002 	1
		// VN, VP = 1 			0004 	2
		// VN, FN, VN = FP 		0008 	3
		// FP, VP, FP = VN		0010 	4
		// FP, FP = FN			0020 	5
		// FP, FN = 1 			0040 	6
		// FN, VN, FN = VP		0080  	7
		// FN, FN = FP			0100 	8
		// FN, FP = 1 			0200 	9

			// lägg in följande:
		// FP VP FN = FP VP FN FN FP = FP VP FP FP = 	VN FP
		// FP VN FN = FP FP FN VN FN = 					FN VP
		// FN VP FP = FN VP FP VP VN = FN FN VN = 		FP VN
		// FN VN FP = FN VN FP FP FN = FN VN FN FN = 	VP FN
}

