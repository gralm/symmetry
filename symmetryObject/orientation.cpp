#include "objs.h"

using namespace std;


double s5 = sqrt(5.0);
MAT Orientation::VPT = MAT(
		5./6., 			sqrt(1./12.), 		-sqrt(2.)/3.,
		-sqrt(1./12.), 	-1./2., 			-sqrt(2./3.),
		-sqrt(2.)/3., 	sqrt(2./3.), 		-1./3.);
MAT Orientation::VPO =
		MAT(2./3,			1./sqrt(3),		-sqrt(2.)/3.,
		-1./sqrt(3),			0.,			-sqrt(2./3),
		-sqrt(2) / 3.,		sqrt(2./3),		1./3);
MAT Orientation::VPI =
		MAT(	(9.-s5)/12.,				(3+s5)/(4*sqrt(3)),		-1./3.,
				-(3.+s5)/(4.*sqrt(3.)),	(s5-1.)/4.,				-1./sqrt(3.),
				-1./3.,					1./sqrt(3.),				s5/3.);


MAT Orientation::FP120 = 
	MAT(	COS120,	SIN120,	0,
			-SIN120,COS120, 0,
			0,		0,		1);
MAT Orientation::FP60 = 
	MAT(	COS60,	SIN60,	0,
			-SIN60,COS60, 	0,
			0,		0,		1);


MAT Orientation::VNO = VPO.transpose();
MAT Orientation::VNI = VPI.transpose();
MAT Orientation::VNT = VNT.transpose();
MAT Orientation::FN120 = FP120.transpose();
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
	return (A-pos) * ori.transpose();
}

VEC Orientation::getWCFromOC(VEC A) const
{
	return (A*ori) + pos;
}

void Orientation::print() const
{
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

	cout << "FP120*VPT*FP120*VPT = I: " << endl;
	cout << FP120*VPT*FP120*VPT << endl;
	cout << "FP120*VPO*FP120*VPO = I: " << endl;
	cout << FP120*VPO*FP120*VPO << endl;
	cout << "FP120*VPI*FP120*VPI = I: " << endl;
	cout << FP120*VPI*FP120*VPI << endl;

	cout << " ***************** " << endl;
	Orientation nyOri(SYMMETRY_OCTAHEDRAL);

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

