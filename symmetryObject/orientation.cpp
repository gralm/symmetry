#include "objs.h"

using namespace std;

//MAT Orientation::VPHexa = MAT(

	// fixa denna
MAT Orientation::VPT = MAT(1, 0, 0, 0, 1, 0, 0, 0, 1);
MAT Orientation::VNT = MAT(1, 0, 0, 0, 1, 0, 0, 0, 1);

MAT Orientation::VPO = 
	MAT(2./3,			-1./sqrt(3),	-sqrt(2)/3.,
	1./sqrt(3),			0.,				sqrt(2./3),
	-sqrt(2) / 3.,		-sqrt(2./3),	1./3);

MAT Orientation::VPI = 	
	MAT(.5,				sqrt(3.)/2,			0.,
		-sqrt(5./12), 	sqrt(5.)/6,			-2./3,
		-sqrt(1./3),	1./3,				sqrt(5.)/3);

MAT Orientation::VNO = VPO.transpose();

MAT Orientation::VNI = VPI.transpose();


MAT Orientation::FP120 = 
	MAT(	COS120,	SIN120,	0,
			-SIN120,COS120, 0,
			0,		0,		1);

MAT Orientation::FN120 = FP120.transpose();

MAT Orientation::FP60 = 
	MAT(	COS60,	SIN60,	0,
			-SIN60,COS60, 	0,
			0,		0,		1);

MAT Orientation::FN60 = FP60.transpose();
	

	// Root orientation
Orientation::Orientation(SymmetryPattern createWithPattern) {
	pos = VEC(0, 0, 0);

	ori = MAT(	COS30,	SIN30,	0,
				-SIN30,	COS30,	0,
				0,		0,		1);

	pat = createWithPattern;
}

void Orientation::rotate(const Prefix &p_)
{
	for (list<TYP>::const_iterator it = p_.R.begin(); (it != p_.R.end()); it++) 
		rotate(*it);
}


/*void Orientation::rotate(TYP r_)
{
	//rotate(r_, SYMMETRY_HEXAGONAL);
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
}*/

void Orientation::rotate(TYP r_)
{
	
	switch (r_) {
		case FP: {
			pos += VEC(COS30, -SIN30, 0) * ori;
			ori = FP120 * ori;
			//ori = ori * FP120;
			break;}
		case FN: {
			pos += VEC(COS30, SIN30, 0) * ori;
			ori = FN120 * ori;
			//ori = ori * FN120;
			break;}
		case VP: {
			MAT *VRot = 0;
			switch (pat)
			{
				case SYMMETRY_HEXAGONAL:
					VRot = &FP60;
					break;
				case SYMMETRY_TETRAHEDRAL:
					VRot = &VPT;
					break;
				case SYMMETRY_OCTAHEDRAL:
					VRot = &VPO;
					break;
				case SYMMETRY_ICOSAHEDRAL:
					VRot = &VPI;
					break;
				default:
					return;
			}
			ori = (*VRot) * ori;
			//ori = ori * (*VRot);

			break;}
		case VN: {
			MAT *VRot = 0;
			switch (pat)
			{
				case SYMMETRY_HEXAGONAL:
					VRot = &FN60;
					break;
				case SYMMETRY_TETRAHEDRAL:
					VRot = &VNT;
					break;
				case SYMMETRY_OCTAHEDRAL:
					VRot = &VNO;
					break;
				case SYMMETRY_ICOSAHEDRAL:
					VRot = &VNI;
					break;
				default:
					return;
			}
			ori = (*VRot) * ori;
			//ori = ori * (*VRot);
			break;}
		default:
			cout << "illegal orientation.rotation" << endl;
			break;
	}
}


VEC Orientation::getOCFromWC(VEC A) const
{
	//return VEC((A-V) * (F-V), (A-V) * ~(F-V))*TAN60;
	return (A-pos) * ori;
}

VEC Orientation::getWCFromOC(VEC A) const
{
	//return V + ((F-V)*A.x + ~(F-V)*A.y)*TAN60;
	//return (A*ori) + pos;
	return (A*ori.transpose()) + pos;
}

void Orientation::print() const
{
	//cout << "V: " << V << endl;
	//cout << "F: " << F << endl;
	cout << "Position: " << pos << endl;
	cout << "Orientation: " << endl;
	cout << ori << endl;
}

SymmetryPattern Orientation::getPattern() const
{
	return pat;
}

void Orientation::test()
{
	//MAT mh = 
	MAT mt = VPT;
	MAT mo = VPO;
	MAT mi = VPI;

	for (int i=0; i<2; i++)
		mt *= VPT;

	for (int i=0; i<3; i++)
		mo *= VPO;

	for (int i=0; i<4; i++)
		mi *= VPI;

	cout << "MT = I: " << endl;
	cout << mt << endl;
	cout << "MO = I: " << endl;
	cout << mo << endl;
	cout << "MI = I: " << endl;
	cout << mi << endl;

	cout << " ***************** " << endl;
	//Orientation nyOri(SYMMETRY_HEXAGONAL);
	Orientation nyOri(SYMMETRY_OCTAHEDRAL);
	//Orientation nyOri(SYMMETRY_ICOSAHEDRAL);
	double realX0X1val = 0;
	switch(nyOri.getPattern())
	{
		case SYMMETRY_HEXAGONAL:
			realX0X1val = 0.5;
			break;
		case SYMMETRY_TETRAHEDRAL:
			realX0X1val = 5./6;
			break;
		case SYMMETRY_OCTAHEDRAL:
			realX0X1val = 2./3;
			break;
		case SYMMETRY_ICOSAHEDRAL:
			realX0X1val = 0.5;
			break;
		default:
			realX0X1val = -1000;
			break;
	}
	VEC X0 = nyOri.ori[0];
	nyOri.print();	// 0

	nyOri.rotate(VP);
	VEC X1 = nyOri.ori[0];
	nyOri.print();	// 1
	cout << "X0*X1 = " << realX0X1val << " = " << X0*X1 << endl;
	// 0.5 för icosahedral
	// 2/3 för octahedral
	// 0.5 för hexagonal

	nyOri.rotate(FP);
	VEC X2 = nyOri.ori[0];
	nyOri.print();	// 2
	cout << "X1*X2 = -0.5 = " << X1*X2 << endl;

	nyOri.rotate(VP);
	VEC X3 = nyOri.ori[0];
	nyOri.print();	// 3
	cout << "X2*X3 = " << realX0X1val << " = " << X2*X3 << endl;
}


/*VEC Orientation::getWCFromRootOC(VEC A)
{
	//return point(A * point(COS30, SIN30),  A*point(-SIN30, COS30));
	//return VEC(A.x*COS30 + A.y*SIN30,  -A.x*SIN30 + A.y*COS30);
}*/

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


/*
Icosaedral symmetri:
VP X0 = X1

VP = 	[.5,			sqrt(3.)/2,			0]
		[-sqrt(5./12), 	sqrt(5.)/6,			-2./3]
		[-sqrt(1./3),	1./3				sqrt(5.)/3]

VP VP VP VP = VN

VN = VP^-1



	Octahedral Rotationsvktor: VP

0.6666667  - 0.5773503  - 0.4714045  
0.5773503  - 3.205D-17    0.8164966  
- 0.4714045  - 0.8164966    0.3333333 

[2/3 				-1/sqrt(3)		-sqrt(2)/3]
[1/sqrt(3)		0 				sqrt(2/3)]
[-sqrt(2) / 3 	-sqrt(2/3)		1/3]
*/

