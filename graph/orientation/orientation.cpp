#include "orientation.h"
#include "point.h"
#include "prefix.h"

using namespace std;


namespace Graph2D {

	Orientation::Orientation() {
		V = point(0, 0);
		F = point(.5, COS30/3.);
	}

	void Orientation::rotate(Prefix p_)
	{
		for (list<TYP>::iterator it = p_.R.begin(); (it != p_.R.end()); it++) 
		{
			switch (*it) {
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
					break;
				}
			}
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
}
