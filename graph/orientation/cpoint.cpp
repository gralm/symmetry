#include "orientation.h"

using namespace std;



namespace Graph2D {


	const VEC vertexCenteredPoint(.0, .0);
	const VEC edgeCenteredPoint(.25, SIN60*.5);
	const VEC faceCenteredPoint(.5, .25/COS30);

	VEC Point::getpoint() const
	{
		VEC rootpoint_;
		if (index < 0) {
			switch (index)
			{
				case VERTEX_CENTERED:
					rootpoint_ = vertexCenteredPoint;
					break;
				case EDGE_CENTERED:
					rootpoint_ = edgeCenteredPoint;
					break;
				case FACE_CENTERED:
					rootpoint_ = faceCenteredPoint;
					break;
				default:
					cout << "it became default o det ska det inte" << endl;
					rootpoint_ = VEC(-100, -100);
					break;
			}
		} else if (index >= V.size()) {
			//cout << "hit skulle den ju inte komma ju ju ju " << endl;
			return VEC(-100, -100);
		} else {
			rootpoint_ = V[index];
		}

		Orientation ori_;
		VEC rotatedpoint_ = ori_.getOCFromWC(rootpoint_);
		ori_.rotate(Pfx);
		rotatedpoint_ = ori_.getWCFromOC(rotatedpoint_);
		return rotatedpoint_;
		
		// niclas nosch 11-12
		// erik är från combitech
	}

	void Point::print() const
	{
		cout << "index (";
		switch(index)
		{
			case -1:
				cout << "undef.";
				break;
			case VERTEX_CENTERED:
				cout << "VC";
				break;
			case EDGE_CENTERED:
				cout << "EC";
				break;
			case FACE_CENTERED:
				cout << "FC";
				break;
			default:
				cout << index;
				break;
		}
		cout << "), ";
		Pfx.print();
		cout << ", " << getpoint();
	}

	string Point::toString() const 
	{		
		if (index == -1)
			return "undef.";
		stringstream ss;
		switch(index)
		{
			case VERTEX_CENTERED:
				ss << "VC";
				break;
			case EDGE_CENTERED:
				ss << "EC";
				break;
			case FACE_CENTERED:
				ss << "FC";
				break;
			default:
				ss << index << "";
				break;
		}
		ss << Pfx.toString();
		//Pfx.print();
		//ss << ", " << getpoint();
		return ss.str();
		//Point
	}

	Point::Point() {}

	Point::Point(Prefix Pfx_, int index_)
	{
		Pfx = Pfx_;
		index = index_;
	}


	bool Point::equalTo(Point &A) {
		Prefix subPfx = Pfx.difference(A.Pfx);
		bool keepTrying = (index<-1);
		subPfx.simplify();

		cout << endl << "subPfx before: ";
		subPfx.print();
		cout << endl;


		//for (list<TYP>::reverse_iterator rit = R.rbegin(); rit != R.rend(); rit++)
			//toReturn.R.push_back(INV_ROTATION(*rit));

		int previousEcTYP = 0;
		int k = 0;

		keepTrying = true;
		for (list<TYP>::reverse_iterator ritR = subPfx.R.rbegin(); keepTrying && ritR != subPfx.R.rend();)
		{
			k++;
			cout << "now is ritR = ";
			rotationPrint(*ritR);
			cout << endl;

			if (index == VERTEX_CENTERED && (*ritR == VN || *ritR == VP)) {
				cout << "VC" << endl;
				/*subPfx.R.erase(--(ritR.base()));
				ritR = subPfx.R.rbegin();*/
				ritR = list<TYP>::reverse_iterator(subPfx.R.erase(--(ritR.base())));
				cout << "ritR efter erase: ";
				rotationPrint(*ritR);
				cout << endl;
			} else if (index == FACE_CENTERED && (*ritR == FN || *ritR == FP)) {
				cout << "FC" << endl;
				ritR = list<TYP>::reverse_iterator(subPfx.R.erase(--(ritR.base())));
				cout << "ritR efter erase: ";
				rotationPrint(*ritR);
			} else if (index == EDGE_CENTERED) {
				//TYP ritRold = *ritR;
				if ((previousEcTYP == FP && *ritR == VP) || (previousEcTYP == VN && *ritR == FN)) {
					cout << "ECa" << endl;
					subPfx.R.pop_back();
					subPfx.R.pop_back();
					ritR = subPfx.R.rbegin();
					//ritR = list<TYP>::reverse_iterator(subPfx.R.erase(--(ritR.base())));
					//ritR = list<TYP>::reverse_iterator(subPfx.R.erase(--(ritR.base())));
					previousEcTYP = 0;
				} else if ((previousEcTYP == VN && *ritR == FP) || (previousEcTYP == VN && *ritR == FP)){
					cout << "ECd" << endl;
					subPfx.R.pop_back();
					subPfx.R.pop_back();
					subPfx.R.push_back(FN);
					ritR = subPfx.R.rbegin();
					subPfx.print();
					cout << endl;
					subPfx.simplify();
					subPfx.print();
					cout << endl;
					previousEcTYP = 0;
					cout << endl;
				} else if (previousEcTYP == 0 && (*ritR == FP || *ritR == VN)) {
					cout << "ECb" << endl;
					previousEcTYP = *ritR;
					ritR++;
				} else {
					cout << "ECc" << endl;
					keepTrying = false;
				}
			} else {

				cout << "0" << endl;
				keepTrying = false;
			}
			if (k > 10)
				break; 
		}


		cout << endl << "subPfx efter: ";
		subPfx.print();
		cout << endl;

		return (subPfx.getSize() == 0);
	}

}