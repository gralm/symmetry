#include "objs.h"

using namespace std;

/*
const VEC SymmetryObject::vertexCenteredPoint = VEC(.0, .0);
const VEC SymmetryObject::edgeCenteredPoint = VEC(.25, SIN60*.5);
const VEC SymmetryObject::faceCenteredPoint = VEC(.5, .25/COS30);
*/

const VEC SymmetryObject::vertexCenteredPoint = VEC(.0, .0);
const VEC SymmetryObject::edgeCenteredPoint = VEC(sqrt(3.)/4, 1./4.);
const VEC SymmetryObject::faceCenteredPoint = VEC(sqrt(1./3.), 0);


Point::Point() {
	index = -1;
}

Point::Point(int index_) {
	index = index_;
}

Point::Point(Prefix Pfx_, int index_)
{
	Pfx = Pfx_;
	index = index_;
}

void Point::print() const
{
	cout << "index (";
	switch(index)
	{
		case NOT_CENTERED:
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
	//cout << ", " << getpoint();
}

bool Point::isDefined() const
{
	return index != NOT_CENTERED;
}

string Point::toString() const 
{		
	if (index == NOT_CENTERED)
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



bool Point::equalTo(Point &A) {
	Prefix subPfx = Pfx.difference(A.Pfx);
	bool keepTrying = (index<-1);
	subPfx.simplify();

	//cout << endl << "subPfx before: ";
	//subPfx.print();
	//cout << endl;


	//for (list<TYP>::reverse_iterator rit = R.rbegin(); rit != R.rend(); rit++)
		//toReturn.R.push_back(INV_ROTATION(*rit));

	int previousEcTYP = 0;
	int k = 0;

	keepTrying = true;
	for (list<TYP>::reverse_iterator ritR = subPfx.R.rbegin(); keepTrying && ritR != subPfx.R.rend();)
	{
		k++;
		//cout << "now is ritR = ";
		//rotationPrint(*ritR);
		//cout << endl;

		if (index == VERTEX_CENTERED && (*ritR == VN || *ritR == VP)) {
			//cout << "VC" << endl;
			/*subPfx.R.erase(--(ritR.base()));
			ritR = subPfx.R.rbegin();*/
			ritR = list<TYP>::reverse_iterator(subPfx.R.erase(--(ritR.base())));
			//cout << "ritR efter erase: ";
			//rotationPrint(*ritR);
			//cout << endl;
		} else if (index == FACE_CENTERED && (*ritR == FN || *ritR == FP)) {
			//cout << "FC" << endl;
			ritR = list<TYP>::reverse_iterator(subPfx.R.erase(--(ritR.base())));
			//cout << "ritR efter erase: ";
			//rotationPrint(*ritR);
		} else if (index == EDGE_CENTERED) {
			//TYP ritRold = *ritR;
			if ((previousEcTYP == FP && *ritR == VP) || (previousEcTYP == VN && *ritR == FN)) {
				//cout << "ECa" << endl;
				subPfx.R.pop_back();
				subPfx.R.pop_back();
				ritR = subPfx.R.rbegin();
				//ritR = list<TYP>::reverse_iterator(subPfx.R.erase(--(ritR.base())));
				//ritR = list<TYP>::reverse_iterator(subPfx.R.erase(--(ritR.base())));
				previousEcTYP = 0;
			} else if ((previousEcTYP == VN && *ritR == FP) || (previousEcTYP == VN && *ritR == FP)){
				//cout << "ECd" << endl;
				subPfx.R.pop_back();
				subPfx.R.pop_back();
				subPfx.R.push_back(FN);
				ritR = subPfx.R.rbegin();
				subPfx.print();
				//cout << endl;
				subPfx.simplify();
				subPfx.print();
				//cout << endl;
				previousEcTYP = 0;
				//cout << endl;
			} else if (previousEcTYP == 0 && (*ritR == FP || *ritR == VN)) {
				//cout << "ECb" << endl;
				previousEcTYP = *ritR;
				ritR++;
			} else {
				//cout << "ECc" << endl;
				keepTrying = false;
			}
		} else {

			//cout << "0" << endl;
			keepTrying = false;
		}
		if (k > 10)
			break; 
	}


	//cout << endl << "subPfx efter: ";
	//subPfx.print();
	//cout << endl;

	return (subPfx.size() == 0);
}

void Point::simplify()
{
	Pfx.simplify();
	switch(index)
	{
	case VERTEX_CENTERED:
		// ta bort alla sistkommande TYP av värde VP och VN
	case EDGE_CENTERED:
		/*
		 * VP FP = 1
		 * FP VN = FN
		 * FN VN = 1
		 * */
	case FACE_CENTERED:
		// ta bort alla sistkommande TYP av värde FP och FN
		cout << "Point::simplify() är inte färdigskriven" << endl;
		break;
	}
}
