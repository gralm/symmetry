#include "graph2d.h"
#include "orientation/orientation.h"

using namespace std;

namespace Graph2D {

	int mode = 0;

	double xMin = 0;
	double xMax = 0;
	double yMin = 0;
	double yMax = 0;
	int scrWidth = 0;
	int scrHeight = 0;
	
	bool facePointActive = false;
	bool edgePointActive = false;
	bool vertexPointActive = false;

	int mouseX = -100;
	int mouseY = -100;

	std::vector<point> V;
	std::vector<edge> E;
	std::vector<face> F;


	point pointsToDrawTillfalligt[3];

	Prefix preE_ToBe;
	vector<edge> E_ToBe;

	
	int indexChosen = -1;
	int indexMouseOver = -1;



	void Edge::print()
	{
		cout << "index (";
		switch(index)
		{
			case -1:
				cout << "undef.";
				break;
			default:
				cout << index;
				break;
		}
		cout << "), ";
		Pfx.print();
	}



	void edge::print()
	{
		cout << "\tPoint fr = ";
		fr.print();
		cout << endl << "\tPoint to = ";
		to.print();
		cout << endl;

		cout << "\tEdge next = ";
		next.print();
		cout << endl << "\tEdge prev = ";
		prev.print();
		cout << endl << "\tEdge oppo = ";
		oppo.print();
		cout << endl;
	}

		// returns true if e is opposite edge. If pfx != null, pfx
	bool edge::isOppositeOf(const edge &e, Prefix *pfx)
	{
		//Point fr;
		//Point to;
		/*
			Point
		Prefix Pfx;
		int index;
		*/

			// avsluta om inte ens punkterna stämmer
		if (fr.index != e.to.index || to.index != e.fr.index)
			return false;

		Prefix A_ = fr.Pfx * to.Pfx.getInverse();
		


		//pfx->R.clear();
		return false;
	}


	face::face()
	{
		fr = -1;
		edges = 0;
	}

	face::face(int fr_, int edges_, int type_)
	{
		fr = fr_;
		edges = edges_;
		type = type_;
	}

	void face::print()
	{
		cout << "from: " << fr << "\t len: " << edges << "\t typ: ";
		switch(type)
		{
			case -1:
				cout << "Not-Centered";
				break;
			case VERTEX_CENTERED:
				cout << "Vertex-Centered";
				break;
			case EDGE_CENTERED:
				cout << "Edge-Centered";
				break;
			case FACE_CENTERED:
				cout << "Face-Centered";
				break;
			default:
				cout << "Felaktig";
				break;
		}
	}


		// AB är koordinater på skärmen, pixelposition
	point fromABtoXY(int x, int y)
	{
		return point(
			xMin + x*(xMax - xMin)/scrWidth,
			yMax - y*(yMax - yMin)/scrHeight);
		
	}

		//XY är transformerade koordinater, edgelängden = 1.0
	void fromXYtoAB(point XY, int *ABx, int *ABy)
	{
		*ABx = scrWidth*(XY.x + 0.5)*2.0/3.0;
		*ABy = scrHeight*(XY.y + 1.0)*COS30;
	}



		// 	detta är en fullständig test som kollar inte 
		//	bara sista tillagda edgen utan alla edges i facet
		// 		returnerar:
		// 0 om det är felaktigt
		// 1 om det är ok men icke sluten
		// 2 om face är ok.
	int checkE_ToBe()
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
		point fr_ = E_ToBe[0].fr.getpoint();
		point to_ = E_ToBe[0].to.getpoint();
		point edge0_ = to_ - fr_;
		point edge1_;
		for (int i=1; i<siz-1; i++)
		{
			fr_ = to_;
			to_ = E_ToBe[i].to.getpoint();
			edge1_ = to_ - fr_;

			if ((~edge0_ * edge1_) < 0.0)
			{
				cout << "******** EDGES INVALID ********* " << endl;
				cout << "edge0: ";
				edge0_.print();
				cout << "\t edge1: ";
				edge1_.print();
				cout << endl;
				return 0;
			}
			edge0_ = edge1_;
		}



			// kolla om den korsar sig själv
		for (int i=2; i<siz-1; i++)
		{
			for (int j=0; j<i-1; j++)
			{
				point Afr_ = E_ToBe[j].fr.getpoint();
				point Ato_ = E_ToBe[j].to.getpoint();
				point Bfr_ = E_ToBe[i].fr.getpoint();
				point Bto_ = E_ToBe[i].to.getpoint();
				
				point P_ = ~(Afr_ - Bfr_);
				point Q_ = Ato_ - Afr_;
				point R_ = Bto_ - Bfr_;

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
		point A[3];
		A[0] = E_ToBe[0].fr.getpoint();
		for (int i=2; i<siz; i++)
		{
			A[1] = E_ToBe[i-1].fr.getpoint();
			A[2] = E_ToBe[i].fr.getpoint();
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

				Orientation ori;
				ori.rotate(E_ToBe[0].fr.Pfx);
				point A[3];
				A[0] = E_ToBe[0].fr.getpoint();
				A[1] = E_ToBe[siz-1].fr.getpoint();
				A[2] = ori.getWCFromOC(point(0,0));
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
				A[0] = E_ToBe[1].fr.getpoint();
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

				point A[3];
				Orientation ori;
				A[2] = ori.getOCFromWC(faceCenteredPoint);

				ori.rotate(E_ToBe[0].fr.Pfx);
				A[0] = E_ToBe[0].fr.getpoint();
				A[1] = E_ToBe[siz-1].fr.getpoint();
				A[2] = ori.getWCFromOC(A[2]);
				for (int k=0; k<3; k++)
					pointsToDrawTillfalligt[k] = A[k];

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
				A[0] = E_ToBe[1].fr.getpoint();
				A[2] = ori.getOCFromWC(A[0]);
				ori.rotate(FP);
				A[2] = ori.getWCFromOC(A[2]);

				/*for (int k=0; k<3; k++) {
					cout << "k: " << k << "\t";
					A[k].print();
					cout << endl;
				}*/

				//cout << "(A[2] - A[1]) * ~(A[1] - A[0]) = " << ((A[2] - A[1]) * ~(A[1] - A[0])) << endl;


				if ((A[2] - A[1]) * ~(A[1] - A[0]) > 0)
				{
					// den kan antas rotera kring vertexen.
					return FACE_CENTERED;
				} else 
					return 1;
			}

				// kolla om det är en edge-centered face
			if (pfxDiff.getSize() == 2 && (pfxDiff[0]^pfxDiff[1] == 6))
			{
				cout << "   *******    \n Det is en FN rotation detta :) " << endl;
				
				
					//kontrollera så att inga punkter existerar i det området
				if (edgePointActive)
					return 1;

					// om det är ett rakt streck 
				if (siz == 2)
					return 1;



				point A[3];
				Orientation ori;
				ori.rotate(E_ToBe[0].fr.Pfx);

				switch(pfxDiff[0])
				{
					case FP:
					case VN:
						A[0] = ori.getWCFromOC(point(COS30, -SIN30)*.5);
						break;
					case FN:
					case VP:
						A[0] = ori.getWCFromOC(point(COS30, SIN30)*.5);
						break;
					default:
						cout << "hit ska jag typ icke kommmmma" << endl;
						break;
				}

				list<Point> enclosedPoints;

				for (int k=0; k<siz-1; k++)
				{
					A[1] = E_ToBe[k].fr.getpoint();
					A[2] = E_ToBe[k+1].fr.getpoint();
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
				A[0] = A[0]*2.0 - E_ToBe[1].fr.getpoint();


				cout << "kille0: " << endl;
				A[0].print();
				cout << endl << "kille1: ";
				A[1].print();
				cout << endl << "kille2: ";
				A[2].print();
				cout << endl;
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
	int getEnclosedPoints(point *A, list<Point> &PntList)
	{
		Prefix pfx[3];
		vector<Prefix> relPfxToControl;
		Orientation ori;
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

		point *V_OC = new point[V.size()];
		for (int j=0; j<V.size(); j++)
			V_OC[j] = ori.getOCFromWC(V[j]);


		for (int k=0; k<relPfxToControl.size(); k++) {
			ori.rotate(relPfxToControl[k]);
			PointToAdd.Pfx.rotate(relPfxToControl[k]);

			for (int i=0; i<V.size(); i++)
			{
				point V_WC = ori.getWCFromOC(V_OC[i]);

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


	int mouseOverIndex(point co_)
	{
		double radius2 = 20.0 / (scrHeight + scrHeight);
		radius2 *= radius2;

		if (co_.x*co_.x + co_.y*co_.y < radius2)
			return VERTEX_CENTERED;

		if ((co_.x-.5)*(co_.x-.5) + co_.y*co_.y < radius2 || 
			(co_.x-.25)*(co_.x-.25) + (co_.y-SIN60*.5)*(co_.y-SIN60*.5) < radius2)
			return EDGE_CENTERED;

		if ((co_.x-.5)*(co_.x-.5) + (co_.y-.25/COS30)*(co_.y-.25/COS30) < radius2)
			return FACE_CENTERED;

		for (int v = 0; v<V.size(); v++)
		{
			if ((co_.x-V[v].x)*(co_.x-V[v].x) + (co_.y-V[v].y)*(co_.y-V[v].y) < radius2)
				return v;
		}
		return -1;
	}

	// returns -1 if over none, radius = pixel-radius

	/*Point mouseOverPoint(point co_)
	{
		Point P_;
		P_.Pfx = getPrefix(co_);
		co_ = getRootpoint(co_);
		P_.index = mouseOverIndex(co_);
		//return  mouseOverIndex(getRootpoint(co_));
		return P_;
	}*/

	Point mouseOverPoint(point co_)
	{
			
		Point P_;
		P_.Pfx = getPrefix(co_);
		point co2_ = getRootpoint(co_);
		P_.index = mouseOverIndex(co2_);



		double r2 = co_.x*co_.x + co_.y*co_.y;
			// kolla om det är Edge-centered
		if (P_.index == EDGE_CENTERED)
		{
			Prefix edgePfx;
			cout << endl << " *********** detta is edge" << endl;
				// returnera något annat istället, detta ger fel prefix
			if (r2 < 0.5) {	// r2 = .25 eller .75
				if (co_.x < 0) {
					cout << "A" << endl;
					edgePfx.rotate(VP);
					//A
				} else if (co_.x > .375) {
					cout << "C" << endl;
					edgePfx.rotate(VN);
					// C
				} else if (co_.y > 0) {
					cout << "B" << endl;
					// B
				} else {
					cout << "D" << endl;
					edgePfx.rotate(VN);
					edgePfx.rotate(VN);
					// D
				}
			} else {
				if (co_.x < .5) {
					cout << "E" << endl;
					edgePfx.rotate(VP);
					edgePfx.rotate(FN);
					// E
				} else if (co_.y > 0) {
					cout << "F" << endl;
					edgePfx.rotate(FN);
					// F
				} else {
					cout << "G" << endl;
					edgePfx.rotate(VN);
					edgePfx.rotate(FN);
					// G
				}
			}
			P_.Pfx = edgePfx;
		} else if (P_.index == FACE_CENTERED) {
			Prefix edgePfx;
			cout << endl << " *********** detta is face" << endl;
			if (co_.y < 0) {
				// c
				cout << "c" << endl;
				edgePfx.rotate(VN);
			} else if (co_.y < .5) {
				cout << "b" << endl;
			} else {
				edgePfx.rotate(VP);
				cout << "a" << endl;
			}
			P_.Pfx = edgePfx;
		} else if (P_.index == VERTEX_CENTERED) {
			Prefix edgePfx;
			cout << endl << " *********** detta is vertex" << endl;
			if (r2 < 0.25) {
				cout << "T" << endl;
			} else if (co_.x < 0.0) {
				edgePfx.rotate(VP);
				edgePfx.rotate(FN);
				cout << "P" << endl;
			} else if (co_.x > .75) {
				edgePfx.rotate(FP);
				cout << "R" << endl;
			} else if (co_.y > 0) {
				edgePfx.rotate(FN);

				cout << "Q" << endl;
			} else {
				edgePfx.rotate(VN);
				edgePfx.rotate(FP);
				cout << "S" << endl;
			}
			P_.Pfx = edgePfx;
		}
		//return  mouseOverIndex(getRootpoint(co_));
		return P_;
	}


	Prefix getPrefix(point coord)
	{
		Prefix pfx;
		Orientation ori;

		if (coord.y > 0)
		{
			if (!(coord * point(COS30, -SIN30) > 0 && coord.x>0)){
				ori.rotate(VP);
				pfx.rotate(VP);
			}
		} else {
			ori.rotate(VN);
			pfx.rotate(VN);
		}

		point vertexpoint_[3];
		double dist[3];
		vertexpoint_[0] = ori.getWCFromOC(point(0, 0)) - coord;
		vertexpoint_[1] = ori.getWCFromOC(point(COS30, -SIN30)) - coord;
		vertexpoint_[2] = ori.getWCFromOC(point(COS30, SIN30)) - coord;

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

	point getRootpoint(point coord)
	{
		Prefix pfx = getPrefix(coord);
		Orientation ori;
		ori.rotate(pfx);
		point OCcoords(ori.getOCFromWC(coord));
		return point(
			OCcoords.x*COS30 - OCcoords.y*SIN30, 
			OCcoords.x*SIN30 + OCcoords.y*COS30);

		//point asdf = Orientation::getWCRootFromOC(ori.getOCFromWC(coord));
		//return asdf;
	}

	void setMousePosition(int x, int y)
	{
		mouseX = x;
		mouseY = y;
		indexMouseOver = mouseOverIndex(getRootpoint(fromABtoXY(x, y)));
	}

	void mouseClick(int x, int y)
	{
		cout << "here klickas it: ";
		fromABtoXY(x, y).print();

		if (mode == 0)
    	{
    		point coord_(fromABtoXY(x, y));
    		coord_ = getRootpoint(coord_);

    		indexMouseOver = mouseOverIndex(coord_);
    		cout << "mode = 0, mouseklick: [" << coord_.x << ", " << coord_.y << "] = " << indexMouseOver << endl;
    	
	    	if (indexMouseOver < 0) {
	    		indexChosen = indexMouseOver;

	    		if (indexMouseOver == -1) {
					indexChosen = indexMouseOver = insertVertex(coord_);
		    	} else if (indexMouseOver == VERTEX_CENTERED) {
		    		//indexChosen = VERTEX_CENTERED;
					int msgtyp = vertexPointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
					vertexPointActive = true;
					char vertCent = VERTEX_CENTERED;
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), &vertCent);
					commSendMsg(&wakeV);

				} else if (indexMouseOver == EDGE_CENTERED) {
					//indexChosen = EDGE_CENTERED;
					int msgtyp = edgePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
					edgePointActive = true;
					char edgeCent = EDGE_CENTERED;
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), &edgeCent);
					commSendMsg(&wakeV);

				} else if (indexMouseOver == FACE_CENTERED) {
					//indexChosen = FACE_CENTERED;
					int msgtyp = facePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
					facePointActive = true;
					char faceCent = FACE_CENTERED;
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), &faceCent);
					commSendMsg(&wakeV);
				}
			} else {
					indexChosen = indexMouseOver;
					CommMsg nyttMess(COMM_THREAD_GLUT, COMM_THREAD_MAIN, COMM_MSGTYP_CHOOSE_VERTEX, 0, sizeof(int), (char*)&indexChosen);
					commSendMsg(&nyttMess);
					nyttMess.destroy();
			}
		} else if (mode == 1 && indexMouseOver != -1) {

			Point nyPunkt = mouseOverPoint(fromABtoXY(x, y));
	
    		//point coord_(fromABtoXY(x, y));
    		//Point nyPunkt = getRootpoint2(coord_);

    		//indexMouseOver = mouseOverIndex(V[nyPunkt.v].x, V[nyPunkt.v].y);
    		cout << "mode = 1, mouseklick: [";// << coord_.x << ", " << coord_.y << "] = " << indexMouseOver << endl;


    		cout << "Point: ";
    		nyPunkt.print();
    		cout << endl;

			cout << "nu startar man ritande over vertex" << endl;
			int len = E_ToBe.size();
			edge e_;
			if (len == 0) {
				e_.fr = nyPunkt;
			} else {
				E_ToBe[len-1].to = e_.fr = nyPunkt;
			}
			E_ToBe.push_back(e_);
			len = E_ToBe.size();
			for (int i=0; i<len; i++)
			{
				cout << "Edge[" << i << "]" << endl;
				E_ToBe[i].print();
				cout << endl;
			}

			int sluten = checkE_ToBe();
			cout << "sluten: " << sluten << endl;

			switch(sluten)
			{
				case 2:
					cout << "DET blev en vanlig FACE" << endl;
					break;
				case VERTEX_CENTERED:
					cout << "DET blev en VERTEX FACE" << endl;
					break;
				case EDGE_CENTERED:
					cout << "DET blev en EDGE FACE" << endl;
					break;
				case FACE_CENTERED:
					cout << "DET blev en FACE FACE" << endl;
					break;
			}

			switch(sluten)
			{
				case 0:
					cout << "Det bidde error nostans." << endl;
					E_ToBe.clear();
					break;
				case 1:
					cout << "Den is icke fardig just nu men on god way" << endl;
					break;
				case 2:
				case VERTEX_CENTERED:
				case EDGE_CENTERED:
				case FACE_CENTERED:
					cout << "Den ska vara sluten nu" << endl;

					cout << (addFaceToBe(sluten)? "gick bra att adda": "gick icke laya too");
					cout << endl;
					break;
				default:
					cout << "Hit ska den icke anlanda" << endl;
					break;
			}
		}
	}

	void printAll()
	{
		cout << "\t" << "Edges" << endl;
		for (int i=0; i<E.size(); i++)
		{
			cout << i << ":" << endl;
			E[i].print();
			cout << endl;
		}

		cout << "\t" << "Faces: " << endl;
		for (int i=0; i<F.size(); i++)
		{
			cout << i << ": ";
			F[i].print();
			cout << endl;
		}
	}

	bool addFaceToBe(int sluten)
	{
		cout << "i addfacetobe so is sluten = " << sluten << endl;
				//vector<edge>::iterator ite = E_ToBe.end();
		int sizE = E.size();

		//		Fixa:
		// Rootera tillbaka ytan så att början 
		//och slutet alltid rör  en rootPoint.
		Prefix draBortPfxInv = E_ToBe[0].fr.Pfx;//.getSize();
		cout << "The following prefix ska dras away" << endl;
		draBortPfxInv.print();
		draBortPfxInv = draBortPfxInv.getInverse();

		for (int i=0; i<E_ToBe.size()-1; i++)
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
			if (sluten == 2) {
				edgeToPushBack.next = Edge(PfxNext, (i==E_ToBe.size()-2? sizE: sizE + i + 1));
				edgeToPushBack.prev = Edge(PfxPrev, (i==0? sizE + E_ToBe.size() - 2: sizE + i - 1));
			} else if (sluten == VERTEX_CENTERED) {

				if (i==0)
					PfxPrev.rotate(VN);
				if (i==E_ToBe.size()-2)
					PfxNext.rotate(VP);
				edgeToPushBack.next = Edge(PfxNext, (i==E_ToBe.size()-2? sizE: sizE + i + 1));
				edgeToPushBack.prev = Edge(PfxPrev, i==0? sizE + E_ToBe.size() - 2: sizE + i - 1);
				//edgeToPushBack.next = (i==E_ToBe.size()-2? Edge(Pfx, sizE): Edge(Prefix(), sizE + i + 1));
				
			} else if (sluten == EDGE_CENTERED) {
				PfxNext;
				PfxPrev;
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
			} else if (sluten == FACE_CENTERED) {
				Prefix PfxNext;
				Prefix PfxPrev;

				if (i==0)
					PfxPrev.rotate(FN);
				if (i==E_ToBe.size()-2)
					PfxNext.rotate(FP);
				edgeToPushBack.next = Edge(PfxNext, (i==E_ToBe.size()-2? sizE: sizE + i + 1));
				edgeToPushBack.prev = Edge(PfxPrev, (i==0? sizE + E_ToBe.size() - 2: sizE + i - 1));
			}

				// kolla genom om man kan hitta någon opposite
			cout << endl;
			//int kortastePrefix = 10000000;
			edgeToPushBack.oppo.index = -1;
			//int indexToUse = -1;
			/*
			for (int j=0; j<E.size(); j++)
			{
				if (E[j].fr.index == edgeToPushBack.to.index
					&& E[j].to.index == edgeToPushBack.fr.index)
				{
					Prefix EfrDiffETPBto = E[j].fr.Pfx.difference(edgeToPushBack.to.Pfx);
					Prefix EtoDiffETPBfr = E[j].to.Pfx.difference(edgeToPushBack.fr.Pfx);
					Prefix PfxDiff = EfrDiffETPBto.difference(EtoDiffETPBfr);
					if (PfxDiff.getSize() == 0) {
						//PfxToUse = EfrDiffETPBto;
						if (E[j].oppo.index != -1)
							cout << "E[" << j << "].oppo = E[" << i + sizE << "] men is already upptagen med E[" << E[j].oppo.index << "]" << endl;
						edgeToPushBack.oppo = Edge(EfrDiffETPBto, j);
						E[j].oppo = Edge(EfrDiffETPBto.getInverse(), i + sizE);
					}
				}
			}*/

				// kolla genom om man kan hitta någon opposite, IGEN
			if (edgeToPushBack.oppo.index == -1) {
				for (int j=0; j<E.size(); j++)
				{
					if (E[j].fr.index == edgeToPushBack.to.index
						&& E[j].to.index == edgeToPushBack.fr.index)
					{
						// A.to * inv(B.fr) * B.to * inv(A.fr)
						Prefix shitness;
						Prefix shitness2;
						shitness.rotate(E[j].to.Pfx);
						shitness2 = edgeToPushBack.fr.Pfx.getInverse();
						shitness.rotate(shitness2);
						shitness.rotate(edgeToPushBack.to.Pfx);
						shitness2 = E[j].fr.Pfx.getInverse();
						shitness.rotate(shitness2);
						shitness.simplify();
						cout << "shitness [" << j << "]: ";
						shitness.print();
						cout << endl;
						if (shitness.getSize() == 0)
						{
							Prefix nyaDiffisen = E[j].to.Pfx;
							shitness2 = edgeToPushBack.fr.Pfx.getInverse();
							nyaDiffisen.rotate(shitness2);
							cout << "fann en oppo here: ";
							nyaDiffisen.print(),
							cout << endl;

							//edgeToPushBack.oppo = Edge(nyaDiffisen, j);
							//E[j].oppo = Edge(nyaDiffisen.getInverse(), i + sizE);
							edgeToPushBack.oppo = Edge(nyaDiffisen.getInverse(), j);
							E[j].oppo = Edge(nyaDiffisen, i + sizE);
						}
					}
				}
			}
			cout << "a" << endl;

			if (edgeToPushBack.oppo.index == -1)
			{


				cout << "oppo is -1 here och i = " << i << endl;
				cout << "denna kan ha opposite med sig self" << endl;
				cout << "fr.index = " << E_ToBe[i].fr.index << endl;
				cout << "to.index = " << E_ToBe[i].to.index << endl;

				if (E_ToBe[i].fr.index == E_ToBe[i].to.index)
				{
					Prefix Pfx = E_ToBe[i].fr.Pfx.difference(E_ToBe[i].to.Pfx);
					cout << "denna har prefix: ";
					Pfx.print();
					cout << endl;

					if (Pfx.getSize() == 2 && Pfx[0]^Pfx[1] == 6)
					{
						edgeToPushBack.oppo.index = E_ToBe[i].fr.index;
						edgeToPushBack.oppo.Pfx = Pfx;
					}
				}

			}
			cout << "b" << endl;

			E.push_back(edgeToPushBack);
		}

		E_ToBe.clear();
		F.push_back(face(sizE, E.size() - sizE, sluten == 2? -1: sluten));

		printAll();
	}

	int insertVertex(point coord_)
	{
		int index_ = V.size();
		V.push_back(coord_);
				// c:s strängmetoder ligger ljusår före c++:s hantering av stärngar
				// c++ har sitt jädra stringstream::stream::string::hittepå som inte
				// effektiviserar för någon någonsin. 

		char os[40];
		snprintf(os, 40, "%d: [%.3f, %.3f]", index_, coord_.x, coord_.y);
		CommMsg messToSend(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
			COMM_MSGTYP_ADD_VERTEX, 0, strlen(os) + 1, os);
		commSendMsg(&messToSend);
		return index_;
		// behöver inte förstöra messToSend
	}

	void insertLine(int x, int y)
	{
		static edge e_[10];
		static int numOfEdges_ = 0;
		point coord_(fromABtoXY(x, y));
    	coord_ = getRootpoint(coord_);
	}

	void drawPoint(point _P)
	{
		int siz_ = 10;	// 10 pixlar hög och 10 pixlar bred punkt
		double delta_ = siz_ *(xMax - xMin)*0.5/scrWidth;
		
		glBegin(GL_LINES);
			glVertex3f(_P.x-delta_, _P.y, 0.0);
			glVertex3f(_P.x+delta_, _P.y, 0.0);
		glEnd();

		delta_ = siz_ *(yMax - yMin)*0.5/scrHeight;
		
		glBegin(GL_LINES);
			glVertex3f(_P.x, _P.y-delta_, 0.0);
			glVertex3f(_P.x, _P.y+delta_, 0.0);
		glEnd();
	}

	void drawCircle(point _P, bool filled)	// egentligen en hexagon
	{
		int siz_ = 10;	// 10 pixlar hög och 10 pixlar bred punkt
		double delta_ = siz_ *(xMax - xMin)*0.5/scrWidth;

		glBegin(filled? GL_POLYGON: GL_LINE_STRIP);
			for (int i=0; i<2; i++)
			{
				glVertex3f(_P.x + 1.0*delta_, 	_P.y + 0.0*delta_, 0.0);
				glVertex3f(_P.x + COS30*delta_, _P.y + SIN30*delta_, 0.0);
				glVertex3f(_P.x + COS60*delta_, _P.y + SIN60*delta_, 0.0);
				glVertex3f(_P.x + 0.0*delta_, 	_P.y + 1.0*delta_, 0.0);
				glVertex3f(_P.x - COS60*delta_, _P.y + SIN60*delta_, 0.0);
				glVertex3f(_P.x - COS30*delta_, _P.y + SIN30*delta_, 0.0);
				delta_ *= -1;
			}
			glVertex3f(_P.x + 1.0*delta_, 	_P.y + 0.0*delta_, 0.0);
		glEnd();
	}

	void drawBrade()
	{
		glColor3f(.2,.2,.2);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0., 0., 0.0);
			glVertex3f(-SIN30, COS30, 0.0);
			glVertex3f(SIN30, COS30, 0.0);

			glVertex3f(0., 0., 0.0);
			glVertex3f(SIN30, COS30, 0.0);
			glVertex3f(1.0, 0.0, 0.0);
			glVertex3f(0., 0., 0.0);

			glVertex3f(SIN30,- COS30, 0.0);
			glVertex3f(1.0, 0., 0.0);
		glEnd();	
	}

	void drawedge(edge &e)
	{
		point fr[9];
		point to[9];



		getAllFromRoots(e.fr.getpoint(), fr);
		getAllFromRoots(e.to.getpoint(), to);

		glBegin(GL_LINES);

		for (int i=0; i<9; i++)
		{
			point sidan = point(-(to[i].y-fr[i].y), to[i].x-fr[i].x) * (.01/sqrt((to[i]-fr[i])*(to[i]-fr[i])));
			fr[i] += sidan;
			to[i] += sidan;
			glVertex3f(fr[i].x, fr[i].y, 0.);
			glVertex3f(to[i].x, to[i].y, 0.);
			fr[i] -= sidan;
			to[i] -= sidan;
		}
		glEnd();
	}


		// Här ska fixas, man ska förkrympa edgen in mot facets mittpunkt
	void drawfaces() {
		
		int i=0;
		for (int j=0; j<F.size(); j++)
		{
			switch(F[j].type)
			{
				case VERTEX_CENTERED:
					glColor3f(.7, .5, .3);
					break;
				case EDGE_CENTERED:
					glColor3f(.3, .7, .5);
					break;
				case FACE_CENTERED:
					glColor3f(.5, .3, .7);
					break;
				default:
					glColor3f(.5, .5, .5);
					break;
			}

			for (int k=0; k<F[j].edges; k++)
			{
				drawedge(E[i]);
				i++;
			}
		}
		/*for (int i=0; i<E.size(); i++) {
			drawedge(E[i]);
		}*/
	}



	void getAllFromRoots(const point vRoot_, point *vAll_)
	{
		//if (vRoot_.y <= -100)
		if (!vRoot_.defined())
		{
			for (int i=0; i<9; i++)
				vAll_[i] = vRoot_;
			return;
		}

		for (int i=0; i<3; i++)
		{
			Orientation ori;
			point OCcoord = ori.getOCFromWC(vRoot_);
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

	void setColorOfVertex(int vert_, double str_)
	{
		    // vald, över eller inte = vit
		    // över, men icke vald = turkos
		    // icke vald och icke över = rosa
		if (indexChosen == vert_)
			glColor3f(1.0*str_, 1.0*str_, 1.0*str_);
		else if (indexMouseOver == vert_)
			glColor3f(0.5*str_, 1.0*str_, 1.0*str_);
		else 
			glColor3f(1.0*str_, 0.5*str_, 0.5*str_);

	}

		

	void display()
	{
		point vAll_[9];

			// clear all pixels
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    	// rita bräde:
	    drawBrade();


		drawfaces();


		setColorOfVertex(VERTEX_CENTERED, 1.0);
		getAllFromRoots(vertexCenteredPoint, vAll_);
		if (vertexPointActive || indexMouseOver == VERTEX_CENTERED)
    		for (int i=1; i<9; i++)
    			drawCircle(vAll_[i], vertexPointActive);

    		// EDGE
  	  	setColorOfVertex(EDGE_CENTERED, 1.0);
  	  	getAllFromRoots(edgeCenteredPoint, vAll_);
  	  	if (edgePointActive || indexMouseOver == EDGE_CENTERED)
	    	for (int i=1; i<9; i++)
	    		drawCircle(vAll_[i], edgePointActive);

    		// FACE
    	setColorOfVertex(FACE_CENTERED, 1.0);
		getAllFromRoots(faceCenteredPoint, vAll_);
		if (facePointActive || indexMouseOver == FACE_CENTERED)
	    	for (int i=1; i<9; i++)
	    		drawCircle(vAll_[i], facePointActive);



	    	// rita musem p, om inte mouseOver
    	point coords_ = fromABtoXY(mouseX, mouseY);
    	//cout << "coords of mouse: ";
    	//coords_.print();
		if (mode == 0) {
			if (indexMouseOver == -1) {
				coords_ = getRootpoint(coords_);
				//cout << "root: ";
				//coords_.print();

		    	glColor3f(.41, .41, .41);
		    	getAllFromRoots(coords_, vAll_);
		    	for (int i=1; i<9; i++)
		    		drawPoint(vAll_[i]);

		    	glColor3f(1, 1, 1);
		    	drawPoint(coords_);
	  	  	}
		} else if (mode == 1) {
			//scout << "nu is sizen on E_ToBe.size() = " << E_ToBe.size() << endl;

			int len = E_ToBe.size();
			//coords_
			glColor3f(1, 0, 0);
			glBegin(GL_LINE_STRIP);
			for (int i=0; i<len; i++)
			{
				point p_ = E_ToBe[i].fr.getpoint();
				glVertex3f(p_.x, p_.y, 0.0);
			}
			glVertex3f(coords_.x, coords_.y, 0.0);
			glEnd();
		}



    		// rita alla vertices även om musen är över den:
    	glColor3f(.61, 0, .21);
    	for (int v = 0; v < V.size(); v++)
    	{
    		
    		getAllFromRoots(V[v], vAll_);

    			// rita i rot-triangeln lite starkare:
			setColorOfVertex(v, 1.0);
    		drawPoint(vAll_[0]);

    			// rita de övriga lite skuggade:
			setColorOfVertex(v, 0.5);
    		for (int i=1; i<9; i++)
    			drawPoint(vAll_[i]);	
    	}


    	// ta dän detta sen:
		glBegin(GL_LINE_STRIP);{
			/*glVertex3f(0.25, 0.05, 0.0);
			glVertex3f(0.65, 0.55, 0.0);
			glVertex3f(0.05, 0.75, 0.0);
			glVertex3f(0.25, 0.05, 0.0);*/
			glVertex3f(pointsToDrawTillfalligt[0].x, pointsToDrawTillfalligt[0].y, 0.0);
			glVertex3f(pointsToDrawTillfalligt[1].x, pointsToDrawTillfalligt[1].y, 0.0);
			glVertex3f(pointsToDrawTillfalligt[2].x, pointsToDrawTillfalligt[2].y, 0.0);
			glVertex3f(pointsToDrawTillfalligt[0].x, pointsToDrawTillfalligt[0].y, 0.0);
		}
		glEnd();



	    glutSwapBuffers();
	}

	void test()
	{
		Prefix BPfx;
		BPfx.rotate(VP);
		BPfx.rotate(FP);
		BPfx.rotate(VP);
		Point B(BPfx, EDGE_CENTERED);
		cout << "B: ";
		B.print();
		
		Prefix CPfx;
		CPfx.rotate(FP);
		CPfx.rotate(VN);
		Point C(CPfx, EDGE_CENTERED);
		cout << endl << "C: ";
		C.print();

		bool hej = B.equalTo(C);
		cout << endl << "equalTo: ";
		cout << (hej? "sant": "falskt") << endl;



/*
		insertVertex(point(0.28, 0.15));
		insertVertex(point(0.4, 0.2));
		insertVertex(point(0.15, 0.1));

		pointsToDrawTillfalligt[0] = point(0.25, 0.05);
		pointsToDrawTillfalligt[1] = point(0.65, 0.55);
		pointsToDrawTillfalligt[2] = point(0.05, 0.75);

		list<Point> PntList;
		int result = getEnclosedPoints(pointsToDrawTillfalligt, PntList);

		cout << "Punkter som ligger i areat: " << endl;
		for (list<Point>::iterator itP = PntList.begin(); itP != PntList.end(); itP++)
		{
			itP->print();
			cout << endl;
		}
*/
		cout << "nu skiter det sig " << endl;
	}
}
