#include "orientation.h"

using namespace std;


namespace Graph2D {


		// Root orientation
	Orientation::Orientation() {
		V = point(0, 0);
		F = point(.5, COS30/3.);
	}

	void Orientation::rotate(Prefix p_)
	{
		for (list<TYP>::iterator it = p_.R.begin(); (it != p_.R.end()); it++) 
			rotate(*it);
	}


	void Orientation::rotate(TYP r_)
	{
		switch (r_) {
			case FP: {
				double Vx_ = F.x + (V.x-F.x)*COS120 - (V.y-F.y)*SIN120;
				V.y = F.y + (V.x-F.x)*SIN120 + (V.y-F.y)*COS120;
				V.x = Vx_;
				break;}
			case FN: {
				double Vx_ = F.x + (V.x-F.x)*COS120 + (V.y-F.y)*SIN120;
				V.y = F.y - (V.x-F.x)*SIN120 + (V.y-F.y)*COS120;
				V.x = Vx_;
				break;}
			case VP: {
				double Fx_ = V.x + (F.x-V.x)*COS60 - (F.y-V.y)*SIN60;
				F.y = V.y + (F.x-V.x)*SIN60 + (F.y-V.y)*COS60;
				F.x = Fx_;
				break;}
			case VN: {
				double Fx_ = V.x + (F.x-V.x)*COS60 + (F.y-V.y)*SIN60;
				F.y = V.y - (F.x-V.x)*SIN60 + (F.y-V.y)*COS60;
				F.x = Fx_;
				break;}
			default:
				cout << "illegal orientation.rotation" << endl;
				break;
		}		
	}

	
	point Orientation::getOCFromWC(point A)
	{
		return point((A-V) * (F-V), (A-V) * ~(F-V))*TAN60;
	}

	point Orientation::getWCFromOC(point A)
	{
		return V + ((F-V)*A.x + ~(F-V)*A.y)*TAN60;
	}

	void Orientation::print()
	{
		cout << "V: ";		V.print();
		cout << "F: ";		F.print();
	}


	point Orientation::getWCFromRootOC(point A)
	{
		//return point(A * point(COS30, SIN30),  A*point(-SIN30, COS30));
		return point(A.x*COS30 + A.y*SIN30,  -A.x*SIN30 + A.y*COS30);
	}

	void rotationPrint(TYP a_)
	{
		switch(a_)
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
}
/*
	Icosaedral symmetri:
	VP X0 = X1

	VP = 	[.5,			sqrt(3.)/2,			0]
			[-sqrt(5./12), 	sqrt(5.)/6,			-2./3]
			[-sqrt(1./3),	1./3				sqrt(5.)/3]

	VP VP VP VP = VN

	VN = VP^-1
*/

