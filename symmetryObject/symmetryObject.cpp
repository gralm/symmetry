#include "objs.h"

using namespace std;


vector<edge> SymmetryObject::E_ToBe;



SymmetryObject::SymmetryObject()
{
	facePointActive = false;
	edgePointActive = false;
	vertexPointActive = false;
	pat = SYMMETRY_HEXAGONAL;

	// test
	/*facePointActive = true;
	edgePointActive = true;
	vertexPointActive = true;

	V.push_back(VEC(.3, .14));*/

}


bool SymmetryObject::getCenteredActive(int whichCenteredIndex)
{
	switch(whichCenteredIndex)
	{
	case VERTEX_CENTERED:
		return vertexPointActive;
		break;
	case EDGE_CENTERED:
		return edgePointActive;
		break;
	case FACE_CENTERED:
		return facePointActive;
		break;
	}
	return false;
}


SymmetryObject::SymmetryObject(const SymmetryObject *gammal)
{
	facePointActive = false;
	edgePointActive = false;
	vertexPointActive = false;
}

	// 	detta är en fullständig test som kollar inte 
	//	bara sista tillagda edgen utan alla edges i facet
	// 		returnerar:
	// 0 om det är felaktigt
	// 1 om det är ok men icke sluten
	// 2 om face är ok.
int SymmetryObject::checkE_ToBe()
{

	int siz = E_ToBe.size();
	if (siz <= 1)
		return 1;
	/*switch(siz)
	{
		case 1:
			return 1;
			break;
		case 2:

	}
	if (siz < 3 && )
		return 1;*/


		// kolla om den roterar i positiv z-riktning
	VEC fr_ = getVec(E_ToBe[0].fr);
	//VEC fr_ = E_ToBe[0].fr.getpoint();
	VEC to_ = getVec(E_ToBe[0].to);
	VEC edge0_ = to_ - fr_;
	VEC edge1_;
	for (int i=1; i<siz-1; i++)
	{
		fr_ = to_;
		to_ = getVec(E_ToBe[i].to);
		edge1_ = to_ - fr_;

		if ((~edge0_ * edge1_) < 0.0)
		{
			cout << "******** EDGES INVALID ********* " << endl;
			cout << "edge0: " << edge0_;
			//edge0_.print();
			cout << "\t edge1: " << edge1_ << endl;
			
			return 0;
		}
		edge0_ = edge1_;
	}



		// kolla om den korsar sig själv
	for (int i=2; i<siz-1; i++)
	{
		for (int j=0; j<i-1; j++)
		{
			VEC Afr_ = getVec(E_ToBe[j].fr);
			VEC Ato_ = getVec(E_ToBe[j].to);
			VEC Bfr_ = getVec(E_ToBe[i].fr);
			VEC Bto_ = getVec(E_ToBe[i].to);
			
			VEC P_ = ~(Afr_ - Bfr_);
			VEC Q_ = Ato_ - Afr_;
			VEC R_ = Bto_ - Bfr_;

			double divider_ = Q_ * ~R_;
			double a_ = (Q_ * P_) / divider_;
			double b_ = (R_ * P_) / divider_;

			//cout << "a_: " << a_ << "\tb_: " << b_ << endl;
			if (a_>0 && a_<1 && b_>0 && b_<1)
			{
				cout << "EDGES FUNKAR ICKE ICKE ICKE!!!" << endl;
				cout << "korsar varandra i=" << i << " och j=" << j << endl;
				cout << "a_: " << a_ << "\tb_: " << b_ << endl;
				return 0;
			}
		}
	}

		// kontrollera att inga Points är inkapslade av markeringen.
	list<Point> enclosedPoints;
	VEC A[3];
	A[0] = getVec(E_ToBe[0].fr);
	for (int i=2; i<siz; i++)
	{
		A[1] = getVec(E_ToBe[i-1].fr);
		A[2] = getVec(E_ToBe[i].fr);
		getEnclosedPoints(A, enclosedPoints);
	}
	
	cout << " ************* " << endl;
	cout << "Enclosed Points: " << endl;
	for (list<Point>::iterator itP = enclosedPoints.begin(); itP != enclosedPoints.end(); itP++){
		itP->print();
		cout << endl;
	}
	cout << " ************* " << endl;


	if (enclosedPoints.size() > 0)
		return 0;



		// Här kontrolleras om kanterna i ytan är sluten:
	if (E_ToBe[siz - 1].fr.index == E_ToBe[0].fr.index)
	{
			// Nu kan den vara sluten 
		Prefix pfxDiff = E_ToBe[0].fr.Pfx.difference(E_ToBe[siz-1].fr.Pfx);
		cout << "prefix difference: ";
		pfxDiff.print();
		cout << endl;	

			// ytan börjar och slutar i samma punkt.
			// Förutsatt att dne kommit hit ner i funktionen
			// så är kanterna slutna till en yta. Toppen!
		if (pfxDiff.getSize() == 0)
		{
			cout << "startar och slutar i samma punkt :) " << endl;
			return 2;
		}

			// om pfxDiff = [VP] så kan det vara en Vertex-Centered Face.
		if (pfxDiff.getSize() == 1 && pfxDiff[0] == VP)
		{
			cout << "Det is en VP rotation detta :) " << endl;
			
			
				//kontrollera så att inga punkter existerar i det området
			if (vertexPointActive)
				return 1;

			Orientation ori(pat);
			ori.rotate(E_ToBe[0].fr.Pfx);
			VEC A[3];
			A[0] = getVec(E_ToBe[0].fr);
			A[1] = getVec(E_ToBe[siz-1].fr);
			A[2] = ori.getWCFromOC(VEC(0,0));
			list<Point> enclosedPoints;
			getEnclosedPoints(A, enclosedPoints);

			cout << "the following killar ligger in the way: " << endl;
			for (list<Point>::iterator itP = enclosedPoints.begin(); itP != enclosedPoints.end(); itP++){
				itP->print();
				cout << endl;
			}

				// Det är ingen vertexcentered face för den har punkter inom sig.
			if (enclosedPoints.size() > 0)
				return 1;


				// kontrollera 
			if (siz == 2)
				return VERTEX_CENTERED;


			// Kontrollera så att polygonen är konvex om 
			// den är sluten.

				// här kan man vara ute på riktigt hal is om 
				// man exempelvis INTE bygger 10 edgeiga faces
				// i ikosaeder-symmetri, utan istället bygger
				// snorspetsiga fula trianglar. Men skyll dig själv!
			A[0] = getVec(E_ToBe[1].fr);
			A[2] = ori.getOCFromWC(A[0]);
			ori.rotate(VP);
			A[2] = ori.getWCFromOC(A[2]);


			if ((A[2] - A[1]) * ~(A[1] - A[0]) > 0)
			{
				// den kan antas rotera kring vertexen.
				return VERTEX_CENTERED;
			} else 
				return 1;
		}
			
			// kolla om det är en face-centered face
		if (pfxDiff.getSize() == 1 && pfxDiff[0] == FP)
		{
			cout << "   *******    \n Det is en FP rotation detta :) " << endl;
			
			
				//kontrollera så att inga punkter existerar i det området
			if (facePointActive)
				return 1;

			VEC A[3];
			Orientation ori(pat);
			A[2] = ori.getOCFromWC(faceCenteredPoint);

			ori.rotate(E_ToBe[0].fr.Pfx);
			A[0] = getVec(E_ToBe[0].fr);
			A[1] = getVec(E_ToBe[siz-1].fr);
			A[2] = ori.getWCFromOC(A[2]);

			list<Point> enclosedPoints;
			getEnclosedPoints(A, enclosedPoints);

			cout << "the following killar ligger in the way: " << endl;
			for (list<Point>::iterator itP = enclosedPoints.begin(); itP != enclosedPoints.end(); itP++){
				itP->print();
				cout << endl;
			}

				// Det är ingen vertexcentered face för den har punkter inom sig.
			if (enclosedPoints.size() > 0)
				return 1;


				// kontrollera 
			if (siz == 2)
				return FACE_CENTERED;

			cout << "kom hit" << endl;
			//return 1;


			// Kontrollera så att polygonen är konvex om 
			// den är sluten.

				// här kan man vara ute på riktigt hal is om 
				// man exempelvis INTE bygger 10 edgeiga faces
				// i ikosaeder-symmetri, utan istället bygger
				// snorspetsiga fula trianglar. Men skyll dig själv!
			A[0] = getVec(E_ToBe[1].fr);
			A[2] = ori.getOCFromWC(A[0]);
			ori.rotate(FP);
			A[2] = ori.getWCFromOC(A[2]);

			if ((A[2] - A[1]) * ~(A[1] - A[0]) > 0)
			{
				// den kan antas rotera kring vertexen.
				return FACE_CENTERED;
			} else 
				return 1;
		}

			// kolla om det är en edge-centered face
		if (pfxDiff.getSize() == 2 && ((pfxDiff[0]^pfxDiff[1]) == 6))
		{
			cout << "   *******    \n Det is en FN rotation detta :) " << endl;
			
			
				//kontrollera så att inga punkter existerar i det området
			if (edgePointActive)
				return 1;

				// om det är ett rakt streck 
			if (siz == 2)
				return 1;



			VEC A[3];
			Orientation ori(pat);
			ori.rotate(E_ToBe[0].fr.Pfx);

			switch(pfxDiff[0])
			{
				case FP:
				case VN:
					A[0] = ori.getWCFromOC(VEC(COS30, -SIN30)*.5);
					break;
				case FN:
				case VP:
					A[0] = ori.getWCFromOC(VEC(COS30, SIN30)*.5);
					break;
				default:
					cout << "hit ska jag typ icke kommmmma" << endl;
					break;
			}

			list<Point> enclosedPoints;

			for (int k=0; k<siz-1; k++)
			{
				A[1] = getVec(E_ToBe[k].fr);
				A[2] = getVec(E_ToBe[k+1].fr);
				getEnclosedPoints(A, enclosedPoints);
			}


			cout << "the following killar ligger in the way: " << endl;
			for (list<Point>::iterator itP = enclosedPoints.begin(); itP != enclosedPoints.end(); itP++){
				itP->print();
				cout << endl;
			}

				// Det är ingen vertexcentered face för den har punkter inom sig.
			if (enclosedPoints.size() > 0) {
				cout << "Det is killar in the way" << endl;
				return 0;
			}

				// nästa punkt i serien kommer bli A[]
			A[0] = A[0]*2.0 - getVec(E_ToBe[1].fr);


			cout << "kille0: " << A[0] << endl;
			cout << "kille1: " << A[1] << endl;
			cout << "kille2: " << A[2] << endl;
			
			cout << "value: " << ((A[0] - A[2]) * ~(A[2] - A[1])) << endl;

			if ((A[0] - A[2]) * ~(A[2] - A[1]) < 0)
				return 1;

			return EDGE_CENTERED;
		}
	}

		// allt funkar och den är inte sluten. Fortsätt bygga din fejja!!!
	return 1;
}

	// A måste vara en array med tre element. I PntList appendas alla inneslutna Points
int SymmetryObject::getEnclosedPoints(VEC *A, list<Point> &PntList)
{
	Prefix pfx[3];
	vector<Prefix> relPfxToControl;
	Orientation ori(pat);
	Point PointToAdd;
	double dh = 0.00000001;


	for (int j=0; j<3; j++)
		pfx[j] = getPrefix(A[j]);


		// radda upp alla unika prefix i pfx och stega genom dem.
		// detta är en ful-lösning som måste analyseras noggrannare
		// Vid rotation [VP FP] måste ju även punkter i området 
		// [VP], [FN] kontrolleras, inte bara [VP FP]. Det görs icke ännu.

	relPfxToControl.push_back(pfx[0]);
	Prefix pfx_tf = pfx[0].difference(pfx[1]);
	if (pfx_tf.getSize() > 0)
		relPfxToControl.push_back(pfx_tf);
	if (pfx[2].difference(pfx[0]).getSize() > 0)
	{
		pfx_tf = pfx[1].difference(pfx[2]);
		if (pfx_tf.getSize() > 0)
			relPfxToControl.push_back(pfx_tf);
	}

	VEC *V_OC = new VEC[V.size()];
	for (unsigned int j=0; j<V.size(); j++)
		V_OC[j] = ori.getOCFromWC(V[j]);


	for (unsigned int k=0; k<relPfxToControl.size(); k++) {
		ori.rotate(relPfxToControl[k]);
		PointToAdd.Pfx.rotate(relPfxToControl[k]);

		for (unsigned int i=0; i<V.size(); i++)
		{
			VEC V_WC = ori.getWCFromOC(V_OC[i]);

			bool enclosed = true;
			for (int j=0; j<3 && enclosed; j++)
			{
				if (~(A[j==2? 0: j+1] - A[j]) * (V_WC - A[j]) < dh)
					enclosed = false;
			}

			if (enclosed)
			{
				PointToAdd.index = i;
				PntList.push_back(PointToAdd);
			}
		}
	}

	delete[] V_OC;

	return 0;
}


Point SymmetryObject::getClosestPoint(VEC co_)
{
	Point P_;
	Prefix startPrefix = getPrefix(co_);
	VEC co2_ = getRootpoint(co_);
	P_.index = -1;// mouseOverIndex(co2_);
	double smallestDistanceSquare;


	bool coLeftSide = (co2_.y > TAN30*co2_.x);

	// om det är på right side jämför med alla coords på VP och FN, annars jämför med VN och FP
	TYP rot1 = coLeftSide? VP: VN;
	TYP rot2 = coLeftSide? FN: FP;

	Orientation ori1(pat);
	VEC co21_ = ori1.getOCFromWC(co2_);
	ori1.rotate(rot1);
	co21_ = ori1.getWCFromOC(co21_);

 	Orientation ori2(pat);
	VEC co22_ = ori2.getOCFromWC(co2_);
	ori2.rotate(rot2);
	co22_ = ori2.getWCFromOC(co22_);

	double newDistance;

	P_ = getClosestCenteredPoint(co_, &smallestDistanceSquare);

	for (unsigned int v=0; v<V.size(); v++)
	{
		newDistance = (V[v] - co2_) * (V[v] - co2_);
		if (newDistance < smallestDistanceSquare)
		{
			smallestDistanceSquare = newDistance;
			P_.index = v;
			P_.Pfx = startPrefix;
		}
	}

		// här är alla V vridna med rot1
	for (unsigned int v=0; v<V.size(); v++)
	{
		newDistance = (V[v] - co21_) * (V[v] - co21_);
		if (newDistance < smallestDistanceSquare)
		{
			smallestDistanceSquare = newDistance;
			P_.index = v;
			P_.Pfx = startPrefix;
			P_.Pfx.rotate(rot1);
		}
	}

		// här är alla V vridna med rot2;
	for (unsigned int v=0; v<V.size(); v++)
	{
		newDistance = (V[v] - co22_) * (V[v] - co22_);
		if (newDistance < smallestDistanceSquare)
		{
			smallestDistanceSquare = newDistance;
			P_.index = v;
			P_.Pfx = startPrefix;
			P_.Pfx.rotate(rot2);
		}
	}

	P_.Pfx.simplify();

	return P_;
}


void SymmetryObject::printAll()
{
	cout << "\t" << "Vectors: " << endl;
	for (unsigned int i=0; i<V.size(); i++)
	{
		cout << i << ":\t" << V[i] << endl;
	}

	cout << endl << "\t" << "Edges:" << endl;
	for (unsigned int i=0; i<E.size(); i++)
	{
		cout << i << ":";
		E[i].print();
		cout << endl;
	}

	cout << "\t" << "Faces: " << endl;
	for (unsigned int i=0; i<F.size(); i++)
	{
		cout << i << ": ";
		F[i].print();
		cout << endl;
	}
}

bool SymmetryObject::addFaceToBe(int sluten)
{
	//cout << "i addfacetobe so is sluten = " << sluten << endl;
			//vector<edge>::iterator ite = E_ToBe.end();
	int sizE = E.size();

	//		Fixa:
	// Rootera tillbaka ytan så att början 
	//och slutet alltid rör  en rootPoint.
	Prefix draBortPfxInv = E_ToBe[0].fr.Pfx;//.getSize();
	cout << "The following prefix ska dras away" << endl;
	draBortPfxInv.print();
	draBortPfxInv = draBortPfxInv.getInverse();

	for (unsigned int i=0; i<E_ToBe.size()-1; i++)
	{
		edge edgeToPushBack;

		Prefix enAnnanPrefixIgen = draBortPfxInv;
		enAnnanPrefixIgen.rotate(E_ToBe[i].fr.Pfx);
		enAnnanPrefixIgen.simplify();
		edgeToPushBack.fr.Pfx = enAnnanPrefixIgen;
		edgeToPushBack.fr.index = E_ToBe[i].fr.index;

		enAnnanPrefixIgen = draBortPfxInv;
		enAnnanPrefixIgen.rotate(E_ToBe[i].to.Pfx);
		enAnnanPrefixIgen.simplify();
		edgeToPushBack.to.Pfx = enAnnanPrefixIgen;
		edgeToPushBack.to.index = E_ToBe[i].to.index;

		Prefix PfxNext;
		Prefix PfxPrev;

		switch (sluten) {
			case NOT_CENTERED: {
				edgeToPushBack.next = Edge(PfxNext, (i==E_ToBe.size()-2? sizE: sizE + i + 1));
				edgeToPushBack.prev = Edge(PfxPrev, (i==0? sizE + E_ToBe.size() - 2: sizE + i - 1));
				break;
			}
			case VERTEX_CENTERED: {

				if (i==0)
					PfxPrev.rotate(VN);
				if (i==E_ToBe.size()-2)
					PfxNext.rotate(VP);
				edgeToPushBack.next = Edge(PfxNext, (i==E_ToBe.size()-2? sizE: sizE + i + 1));
				edgeToPushBack.prev = Edge(PfxPrev, i==0? sizE + E_ToBe.size() - 2: sizE + i - 1);
				//edgeToPushBack.next = (i==E_ToBe.size()-2? Edge(Pfx, sizE): Edge(Prefix(), sizE + i + 1));
				break;
			}
			case EDGE_CENTERED: {
				Prefix cpPrefixHelvete;

				cpPrefixHelvete = E_ToBe[E_ToBe.size()-1].fr.Pfx;			
				cpPrefixHelvete.rotate(draBortPfxInv);

				cout << "when it is done, it becomasar: ";
				cpPrefixHelvete.print();
				cout << endl;

				if (i==0) {
					PfxPrev = cpPrefixHelvete;
				}


				if (i==E_ToBe.size()-2) {
					PfxNext = cpPrefixHelvete;
				}

				edgeToPushBack.next = Edge(PfxNext, (i==E_ToBe.size()-2? sizE: sizE + i + 1));
				edgeToPushBack.prev = Edge(PfxPrev, (i==0? sizE + E_ToBe.size() - 2: sizE + i - 1));
				break;
			}
			case FACE_CENTERED: {
				Prefix PfxNext;
				Prefix PfxPrev;

				if (i==0)
					PfxPrev.rotate(FN);
				if (i==E_ToBe.size()-2)
					PfxNext.rotate(FP);
				edgeToPushBack.next = Edge(PfxNext, (i==E_ToBe.size()-2? sizE: sizE + i + 1));
				edgeToPushBack.prev = Edge(PfxPrev, (i==0? sizE + E_ToBe.size() - 2: sizE + i - 1));
				break;
			}
			default:
				cout << "symmetryObject.cpp\tHIT SKA DEN INTE KOMMA!!!!!!!!!!!" << endl;
				break;
		}

			// kolla genom om man kan hitta någon opposite
		cout << endl;
		//int kortastePrefix = 10000000;
		edgeToPushBack.oppo.index = -1;
		//edgeToPushBack.





			// kolla genom om man kan hitta någon opposite, IGEN
		if (edgeToPushBack.oppo.index == -1) {
			for (unsigned int j=0; j<E.size(); j++)
			{
				Prefix oppositeOfPrefix;
				//if (E[j].isOppositeOf(E_ToBe[i], &oppositeOfPrefix)) {
				if (E[j].isOppositeOf(edgeToPushBack, &oppositeOfPrefix)) {
					edgeToPushBack.oppo = Edge(oppositeOfPrefix.getInverse(), j);
					E[j].oppo = Edge(oppositeOfPrefix, i + sizE);

					break;
				}
			}
		}
		//cout << "a" << endl;

		Prefix oppositeOfPrefix;
		if (edgeToPushBack.oppo.index == -1)
		{
			//if (E_ToBe[i].isOppositeOf(E_ToBe[i], &oppositeOfPrefix)) {
			if (E_ToBe[i].isOppositeOf(edgeToPushBack, &oppositeOfPrefix)) {
				edgeToPushBack.oppo = Edge(oppositeOfPrefix.getInverse(), i + sizE);
			}
		}

		E.push_back(edgeToPushBack);
	}

	F.push_back(face(sizE, E.size() - sizE, sluten));
	cout << "          *****************************************            " << endl;
	cout << "tjena nu is jag here" << endl;
		// uppdatera GUI:et
	char strToSend[200];
		// id, first edge, num of edges, type, görasigplatt-styrka
	snprintf(strToSend, 200, "%d,%d,%d,%d", F.size()-1, sizE, E.size() - sizE, sluten);
	cout << "detta skickas when face skapas: " << strToSend << endl;

	E_ToBe.clear();
	printAll();
	return true;
}

void SymmetryObject::insertCenteredVertex(int index_)
{
	switch(index_)
	{
	case VERTEX_CENTERED:
		vertexPointActive = true;
		break;
	case EDGE_CENTERED:
		edgePointActive = true;
		break;
	case FACE_CENTERED:
		facePointActive = true;
		break;
	default:
		cout << "symmetryObject.cpp blev nog errorigt here" << endl;
	}
}

int SymmetryObject::insertVertex(VEC coord_)
{
	int index_ = V.size();
	V.push_back(coord_);
			// c:s strängmetoder ligger ljusår före c++:s hantering av stärngar
			// c++ har sitt jädra stringstream::stream::string::hittepå som inte
			// effektiviserar för någon någonsin. 
	return index_;
}

	// hämtar alla speglingar i WC från OC
void SymmetryObject::getAllFromRoots(VEC vRoot_, VEC *vAll_)
{
	//if (vRoot_.y <= -100)
	//if (!vRoot_.defined())

	//if (!definedVec(vRoot_))
	if (!IS_DEFINED(vRoot_))
	{
		for (int i=0; i<9; i++)
			vAll_[i] = vRoot_;
		return;
	}

	for (int i=0; i<3; i++)
	{
		Orientation ori(pat);
		//VEC OCcoord = ori.getOCFromWC(vRoot_);
		VEC OCcoord = vRoot_;//ori.getOCFromWC(vRoot_);
		if (i)
			ori.rotate(i==1? VP: VN);
		
		for (int j=0; j<2; j++)
		{
			vAll_[i*3+j] = ori.getWCFromOC(OCcoord);
			ori.rotate(FP);
		}
		vAll_[i*3+2] = ori.getWCFromOC(OCcoord);
	}
}	


	

void SymmetryObject::test()
{

	Prefix diff;
	edge edgeA, edgeB;

	/*edgeA.fr.index = 0;
	edgeA.fr.Pfx;
	edgeA.to.index = FACE_CENTERED;
	edgeA.to.Pfx.rotate(FN);

	edgeB.fr.index = FACE_CENTERED;
	edgeB.fr.Pfx.rotate(FN);
	edgeB.to.index = 0;
	edgeB.to.Pfx.rotate(FN);*/

	
	edgeA.fr.index = 0;
	//edgeA.fr.Pfx;
	edgeA.to.index = 0;
	edgeA.to.Pfx.rotate(FN);
	edgeA.to.Pfx.rotate(VN);


	cout << "edgeA: ";
	 edgeA.print();
	 cout << endl;
	//cout << "edgeB: " << edgeB.toString() << endl;

	if (edgeA.isOppositeOf(edgeA, &diff))
	{
		cout << "is opposite: ";
		diff.print();
		cout << endl;
	} else {
		cout << "not opposite" << endl;
	}
}


VEC SymmetryObject::getVec(Point P_)
{
	//VEC Point::getpoint() const {
	VEC rootpoint_;
	if (P_.index < 0) {
		switch (P_.index)
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
				cout << "SymmetryObject::getVec(Point) it became default o det ska det inte" << endl;
				rootpoint_ = VEC(-100, -100);
				break;
		}
	} else if (P_.index >= (int)V.size()) {
		//cout << "hit skulle den ju inte komma ju ju ju " << endl;
		return VEC(-100, -100);
	} else {
		rootpoint_ = V[P_.index];
	}

	Orientation ori_(pat);
	VEC rotatedpoint_ = ori_.getOCFromWC(rootpoint_);
	ori_.rotate(P_.Pfx);
	rotatedpoint_ = ori_.getWCFromOC(rotatedpoint_);
	return rotatedpoint_;	
}

bool SymmetryObject::fullTest()
{
	cout << "make full test" << endl;

	if (E.size() == 0)
	{
		cout << "Mode cannot be 2, no edges exist." << endl;
		return false;
	}

	for (int e=0; e<E.size(); e++)
	{
		if (!E[e].oppo.isDefined())
		{
			cout << "edge[" << e << "].oppo is not yet defined" << endl;
			return false;
		}
	}

	return true;
}

// non symmetryObjectFunctions

Prefix getPrefix(VEC coord)
{
	Prefix pfx;
	Orientation ori(SYMMETRY_HEXAGONAL);

	if (coord.y > 0)
	{
		if (!(coord * VEC(COS30, -SIN30) > 0 && coord.x>0)){
			ori.rotate(VP);
			pfx.rotate(VP);
		}
	} else {
		ori.rotate(VN);
		pfx.rotate(VN);
	}

	VEC vertexpoint_[3];
	double dist[3];
	vertexpoint_[0] = ori.getWCFromOC(VEC(0, 0)) - coord;
	vertexpoint_[1] = ori.getWCFromOC(VEC(COS30, -SIN30)) - coord;
	vertexpoint_[2] = ori.getWCFromOC(VEC(COS30, SIN30)) - coord;

	for (int i=0; i<3; i++)
		dist[i] = vertexpoint_[i]*vertexpoint_[i];
	

	if (dist[1] < dist[0])
		pfx.rotate((dist[2]<dist[1])? FN: FP);
	else if (dist[2] < dist[0])
		pfx.rotate(FN);


	for (int i=0; i<3; i++)
	{
		if (ori.getOCFromWC(coord).x > COS30)
			return Prefix();
	}

	return pfx;		
}


VEC getRootpoint(VEC coord)	// använder enbart hexagonal symmetry
{
	Prefix pfx = getPrefix(coord);
	Orientation ori(SYMMETRY_HEXAGONAL);
	ori.rotate(pfx);
	VEC OCcoords(ori.getOCFromWC(coord));
	return OCcoords;
	/*return VEC(
		OCcoords.x*COS30 - OCcoords.y*SIN30, 
		OCcoords.x*SIN30 + OCcoords.y*COS30);*/
}




// returnerar närmsta Centered-point
Point getClosestCenteredPoint(VEC coord, double *distanceSquared)
{

	double distanceSquared_ = UNDEFINED_VEC*UNDEFINED_VEC;
	//double distanceSquared_;
	int index = -1;
	Point P;
	VEC x = VEC(0.5, 0.0);
	VEC y = SymmetryObject::edgeCenteredPoint;

	VEC cv[15];

		//Vector-points
	cv[0] = VEC(0., 0.);
	cv[1] = 2*(y - x);
	cv[2] = 2*y;
	cv[3] = 2*x;
	cv[4] = 2*(x - y);

		// Edge-points
	cv[5] = y;
	cv[6] = x;
	cv[7] = x + y;
	cv[8] = y - x;
	cv[9] = 2*y - x;
	cv[10] = x - y;
	cv[11] = 2*x - y;

		// Face-points
	cv[12] = 2.*(x + y)/3.;
	cv[13] = 2.*(2*y - x) / 3.;
	cv[14] = 2*(2*x - y) / 3.;

	for (int i=0; i<15; i++)
	{
		double newDistance = (cv[i] - coord) * (cv[i] - coord);
		if (newDistance < distanceSquared_) {
			distanceSquared_ = newDistance;
			index = i;
		}
	}

	if (index > 11 )
		P.index = FACE_CENTERED;
	else if (index > 4)
		P.index  = EDGE_CENTERED;
	else if (index >= 0)
		P.index = VERTEX_CENTERED;


	switch (index)
	{
	case 1:
	case 9:
		P.Pfx.rotate(VP);
		P.Pfx.rotate(FN);
		break;
	case 2:
		P.Pfx.rotate(VP);
		P.Pfx.rotate(FP);
		break;
	case 3:
	case 6:
		P.Pfx.rotate(FP);
		break;
	case 4:
	case 10:
		P.Pfx.rotate(VN);
		P.Pfx.rotate(FP);
		break;
	case 8:
	case 13:
		P.Pfx.rotate(VP);
		break;
	case 7:
		P.Pfx.rotate(FN);
		break;
	case 11:
		P.Pfx.rotate(VN);
		P.Pfx.rotate(FN);
		break;
	case 14:
		P.Pfx.rotate(VN);
		break;
	default:
		index = -1;
		break;
	}

	if (distanceSquared != 0)
		*distanceSquared = distanceSquared_;

	return P;
}
