#include "graph2d.h"
//#include "orientation/orientation.h"

using namespace std;


// Graph2D symmetryObject;
SymmetryDrawable symmetryObject;

int mode = 0;

double xMin = 0;
double xMax = 0;
double yMin = 0;
double yMax = 0;
int scrWidth = 0;
int scrHeight = 0;


const unsigned char 	SP = 2;		// positiv rotation i triangeln
const unsigned char 	SN = 3;		// negativ rotation i triangeln
const unsigned char 	RP = 4;		// positiv rotation till nästa triangel
const unsigned char 	RN = 5;		// negativ rotation tlil föregående triangel


int symmetryType = 0;

int mouseX = -100;
int mouseY = -100;

int indexChosen = -1;
int indexMouseOver = -1;


void graphDisplay()
{
	symmetryObject.display();
}

void setMousePosition(int x, int y)
{
	mouseX = x;
	mouseY = y;
	Point p = symmetryObject.getClosestPoint(fromABtoXY(x,y));
	indexMouseOver = p.index;
	//indexMouseOver = symmetryObject.vecOverIndex(getRootpoint(fromABtoXY(x, y)));
}


	// AB är koordinater på skärmen, pixelposition
VEC fromABtoXY(int x, int y)
{
	return VEC(
		xMin + x*(xMax - xMin)/scrWidth,
		yMax - y*(yMax - yMin)/scrHeight);
	
}

	//XY är transformerade koordinater, edgelängden = 1.0
void fromXYtoAB(VEC XY, int *ABx, int *ABy)
{
	*ABx = scrWidth*(XY.x + 0.5)*2.0/3.0;
	*ABy = scrHeight*(XY.y + 1.0)*COS30;
}


void Edge::print() const
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

string Edge::toString() const
{
	if (index == -1)
		return "undef.";

	stringstream ss;
	ss << index;
	ss << Pfx.toString();
	return ss.str();
}

bool Edge::isDefined() const
{
	return index != -1;
}



void edge::print() const
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
		// avsluta om inte ens punkterna stämmer
	if (fr.index != e.to.index || to.index != e.fr.index)
		return false;

	Prefix X;

		//
	cout << " ** drive isOppositeOf" << endl;
	cout << "this: " << endl;
	this->print();
	cout << endl << "edge e: " << endl;
	e.print();


	switch(fr.index)
	{
		case VERTEX_CENTERED: {
			cout << "a" << endl;

				// fr = X e.to
			X = fr.Pfx;
			X.rotate(e.to.Pfx.getInverse());
			Point e_fr(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}
			
				// fr [VP] = X e.to 			
			X = fr.Pfx;
			X.rotate(VP);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}
			
				// fr [VN] = X e.to
			X = fr.Pfx;
			X.rotate(VN);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

			cout << "a" << endl;
			break;
		}
		case EDGE_CENTERED: {
			cout << "b" << endl;

			X = fr.Pfx;
			X.rotate(e.to.Pfx.getInverse());
			Point e_fr(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}
			
				// fr [VP] = X e.to 			
			X = fr.Pfx;
			X.rotate(VP);
			X.rotate(FP);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

			break;
		}
		case FACE_CENTERED: {	
			cout << "c" << endl;

				// fr = X e.to
			X = fr.Pfx;
			X.rotate(e.to.Pfx.getInverse());
			Point e_fr(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}
			
				// fr [FP] = X e.to 			
			X = fr.Pfx;
			X.rotate(FP);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}
			
				// fr [FN] = X e.to
			X = fr.Pfx;
			X.rotate(FN);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

			cout << "c" << endl;
			break;
		}
		default: {

			cout << "D" << endl;
			X = fr.Pfx;
			X.rotate(e.to.Pfx.getInverse());
			Point e_fr(X * e.fr.Pfx, e.fr.index);				
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}
			cout << "D" << endl;
			break;
		}
	}


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




/*Prefix getPrefix(VEC coord)
{
	Prefix pfx;
	Orientation ori;

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
}*/



void mouseClick(int x, int y)
{
	cout << "here klickas it: " << fromABtoXY(x, y) << endl;

	if (mode == 0)
	{
		VEC coord_(fromABtoXY(x, y));
		coord_ = getRootpoint(coord_);

		//indexMouseOver = symmetryObject.vecOverIndex(coord_);
		Point pointMouseOver = symmetryObject.getClosestPoint(coord_); 
		indexMouseOver = pointMouseOver.index;
		//indexMouseOver = symmetryObject.vecOverIndex(coord_);
		cout << "mode = 0, mouseklick: [" << coord_.x << ", " << coord_.y << "] = " << indexMouseOver << endl;
	
    	if (indexMouseOver < 0) {
    		indexChosen = indexMouseOver;
    		if (indexMouseOver == -1) {
    			indexChosen = indexMouseOver = symmetryObject.insertVertex(coord_);
    		}
    		int msgtyp;
    		switch(indexMouseOver)
    		{
    			case VERTEX_CENTERED:
    				//msgtyp = vertexPointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
    				msgtyp = symmetryObject.vertexPointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_VERTEX;
    				coord_ = SymmetryObject::vertexCenteredPoint;
    				symmetryObject.vertexPointActive = true;
    				break;
    			case EDGE_CENTERED:
					//msgtyp = edgePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
    				msgtyp = symmetryObject.edgePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_VERTEX;
    				coord_  = SymmetryObject::edgeCenteredPoint;
    				symmetryObject.edgePointActive = true;
    				break;
    			case FACE_CENTERED:
					//msgtyp = facePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
    				msgtyp = symmetryObject.facePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_VERTEX;
    				coord_ = SymmetryObject::faceCenteredPoint;
    				symmetryObject.facePointActive = true;
    				break;
    			default:
    				msgtyp = COMM_MSGTYP_ADD_VERTEX;//vertexPointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
    				break;
    		}
    		cout << "vertexPointActive: " << (symmetryObject.vertexPointActive? "true": "false") << endl;
    		cout << "edgePointActive: " << (symmetryObject.edgePointActive? "true": "false") << endl;
    		cout << "facePointActive: " << (symmetryObject.facePointActive? "true": "false") << endl;

    		char strMess[200];
			//snprintf(strMess, 40, "%d: [%.3f, %.3f]", indexMouseOver, coord_.x, coord_.y);
			cout << "msgtyp: " << msgtyp << endl;
			snprintf(strMess, 200, "%d, %.3f, %.3f, %.3f, 0.000", indexMouseOver, coord_.x, coord_.y, coord_.z);
			cout << "Texten: " << strMess << endl;
			CommMsg commMsgNewVertex(COMM_THREAD_GLUT, COMM_THREAD_MAIN, msgtyp, 0, strlen(strMess) + 1, strMess);
			commSendMsg(&commMsgNewVertex);
		} else {	// väljer en vertex som man klickat på.
				indexChosen = indexMouseOver;
				CommMsg nyttMess(COMM_THREAD_GLUT, COMM_THREAD_MAIN, COMM_MSGTYP_CHOOSE_VERTEX, 0, sizeof(int), (char*)&indexChosen);
				commSendMsg(&nyttMess);
				nyttMess.destroy();
		}
	} else if (mode == 1 && indexMouseOver != -1) {


		VEC coord_(fromABtoXY(x, y));
		coord_ = getRootpoint(coord_);
		Point nyPunkt = symmetryObject.getClosestPoint(coord_);
		//Point nyPunkt = symmetryObject.vecOverPoint(fromABtoXY(x, y));

		//point coord_(fromABtoXY(x, y));
		//Point nyPunkt = getRootpoint2(coord_);

		//indexMouseOver = mouseOverIndex(V[nyPunkt.v].x, V[nyPunkt.v].y);
		cout << "mode = 1, mouseklick: [";// << coord_.x << ", " << coord_.y << "] = " << indexMouseOver << endl;


		cout << "Point: ";
		nyPunkt.print();
		cout << endl;

		cout << "nu startar man ritande over vertex" << endl;
		int len = symmetryObject.E_ToBe.size();
		edge e_;
		if (len == 0) {
			e_.fr = nyPunkt;
		} else {
			symmetryObject.E_ToBe[len-1].to = e_.fr = nyPunkt;
		}
		symmetryObject.E_ToBe.push_back(e_);
		len = symmetryObject.E_ToBe.size();
		for (int i=0; i<len; i++)
		{
			cout << "Edge[" << i << "]" << endl;
			symmetryObject.E_ToBe[i].print();
			cout << endl;
		}

		int sluten = symmetryObject.checkE_ToBe();
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
				symmetryObject.E_ToBe.clear();
				break;
			case 1:
				cout << "Den is icke fardig just nu men on god way" << endl;
				break;
			case 2:
			case VERTEX_CENTERED:
			case EDGE_CENTERED:
			case FACE_CENTERED:{
				cout << "Den ska vara sluten nu" << endl;

				cout << (symmetryObject.addFaceToBe(sluten)? "gick bra att adda": "gick icke laya too");

					// hämta nu senaste facet om det gick bra att adda face och skicka över det till guiet.
				/*CommMsg nyttFace(COMM_THREAD_GLUT, COMM_THREAD_MAIN, COMM_MSGTYP_UPDATE_FACE, 0, strlen(strToSend), strToSend);
				commSendMsg(&nyttFace);
				nyttFace.destroy();


				int fNum = F.size()-1;
				int e = 0;
				for (int f=0; f<F.size(); f++)
				{
					for (int e=F[f].fr; e<F[f].fr + F[f].edges; e++)
					{
							// id, fr, to, next, prev, oppo, face, len, k, d, len0, theta
						snprintf(strToSend, 200, "%d, %s, %s, %s, %s, %s, %d", e, E[e].fr.toString().c_str(), 
							E[e].to.toString().c_str(), E[e].next.toString().c_str(),
							E[e].prev.toString().c_str(), E[e].oppo.toString().c_str(), 
							f);
						cout << strToSend << endl;

						CommMsg nyttEdge(COMM_THREAD_GLUT, COMM_THREAD_MAIN, COMM_MSGTYP_UPDATE_EDGE, 0, strlen(strToSend), strToSend);
						commSendMsg(&nyttEdge);
						nyttEdge.destroy();
					}
				}*/

				cout << endl;
				break;}
			default:
				cout << "Hit ska den icke anlanda" << endl;
				break;
		}
	}
}


int setMode(int newModeVal)		// returnerar det satta värdet
{
	switch(newModeVal) {
		case 0: {
			cout << "Mode = 0, skapa vertice mode" << endl;
			break;}
		case 1:
			/*if (V.size() == 0)
			{
				cout << "V.len = 0, kan inte ta sig till next step" << endl;
				return mode;
			}*/
			cout << "Här måste tester göras för att det ska gå o släppa vidare till step 1.";
			cout << "Mode = 1, skapa face/edges mode" << endl;
			break;
		case 2: {

			cout << "Här måste tester göras för att det ska gå o släppa vidare till step 2.";
			/*if (E.size() == 0)
			{
				cout << "Mode cannot be 2, no edges exist." << endl;
				return mode;
			}

			for (int e=0; e<E.size(); e++)
			{
				if (!E[e].oppo.isDefined())
				{
					cout << "edge[" << e << "].oppo is not yet defined" << endl;
					return mode;
				}
			}*/

			cout << "Mode = 2, choose symmetry" << endl;
			break;
		}
		case 3:
			cout << "Mode = 3, relax towards flat surfaces" << endl;
			break;
		case 4:
			cout << "Mode = 4, relax flat surfaces" << endl;
			break;
		default:
			cout << "Mode = ?, funkar inte" << endl;
			return mode;
			break;
	}
	mode = newModeVal;
	return mode;
}


// rita den där klassen

SymmetryDrawable::SymmetryDrawable()
{

}

void SymmetryDrawable::drawPoint(VEC _P)
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

void SymmetryDrawable::drawCircle(VEC _P, bool filled)	// egentligen en hexagon
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

void SymmetryDrawable::drawBrade()
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

void SymmetryDrawable::drawedge(edge &e)
{
	VEC fr[9];
	VEC to[9];



	getAllFromRoots(getVec(e.fr), fr);
	getAllFromRoots(getVec(e.to), to);

	glBegin(GL_LINES);

	for (int i=0; i<9; i++)
	{
		VEC sidan = VEC(-(to[i].y-fr[i].y), to[i].x-fr[i].x) * (.01/sqrt((to[i]-fr[i])*(to[i]-fr[i])));
		fr[i] += sidan;
		to[i] += sidan;
		glVertex3f(fr[i].x, fr[i].y, 0.);
		glVertex3f(to[i].x, to[i].y, 0.);
		fr[i] -= sidan;
		to[i] -= sidan;
	}
	glEnd();
}


void SymmetryDrawable::drawFace(face &F_)
{
	static VEC V_[200];
	int fVertices = F_.edges;
	double edgeGap = 0.1;
	VEC faceCenter(0., 0.);
	int numOfCopies = 9;

	for (int v=0; v<fVertices; v++)
		V_[numOfCopies*v] = getVec(E[F_.fr + v].fr);
	
	V_[numOfCopies*fVertices] = getVec(E[F_.fr + fVertices - 1].to);
	fVertices++;
	
	switch(F_.type) {
		case VERTEX_CENTERED:
			glColor3f(.7, .5, .3);
			faceCenter = vertexCenteredPoint;
			break;

		case EDGE_CENTERED: {
			glColor3f(.3, .7, .5);
			switch(E[F_.fr + F_.edges - 1].to.Pfx[0])
			{
				case VP:
				case FN:
					faceCenter = edgeCenteredPoint;
					break;

				case FP:
				case VN:
					faceCenter = VEC(.5, 0);
					break;

				default:
					cout << "hit ska det iCKKKE komma!" << endl;
					break;
			}
			
			break;
		}

		case FACE_CENTERED:
			glColor3f(.5, .3, .7);
			faceCenter = faceCenteredPoint;
			break;

		default: {
			glColor3f(.5, .5, .5);
			for (int fVert=0; fVert<F_.edges; fVert++)
				faceCenter += V_[numOfCopies*fVert];
			faceCenter *= 1.0/F_.edges;
			break;

		}
	}

	for (int v=0; v<fVertices; v++) {
		V_[numOfCopies*v] = V_[numOfCopies*v]*(1.0 - edgeGap) + faceCenter*edgeGap;
		getAllFromRoots(V_[numOfCopies*v], V_ + numOfCopies*v);
	}


	glBegin(GL_LINES);
	for (int copies = 0; copies < numOfCopies; copies++)
	{
		for (int v=0; v<fVertices-1; v++)
		{
			glVertex3f(V_[v*numOfCopies+copies].x, V_[v*numOfCopies+copies].y, 0.);
			glVertex3f(V_[v*numOfCopies+numOfCopies+copies].x, V_[v*numOfCopies+numOfCopies+copies].y, 0.);
		}
	}
	glEnd();				
}

void SymmetryDrawable::drawFaces() {
	for (int f=0; f<F.size(); f++)
		drawFace(F[f]);
}


void SymmetryDrawable::setColorOfVertex(int vert_, double str_)
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



void SymmetryDrawable::display()
{
	VEC vAll_[9];

		// clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// rita bräde:
    drawBrade();

    	// rita ut alla färdiga faces på nåt snyggt sätt.
	drawFaces();


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
	VEC coords_ = fromABtoXY(mouseX, mouseY);
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
			VEC p_ = getVec(E_ToBe[i].fr);
			glVertex3f(p_.x, p_.y, 0.0);
		}
		glVertex3f(coords_.x, coords_.y, 0.0);
		glEnd();
	} else if (mode == 2) {

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

    glutSwapBuffers();
}


