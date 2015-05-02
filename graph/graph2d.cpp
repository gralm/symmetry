#include "graph2d.h"
//#include "orientation/orientation.h"

using namespace std;


// Graph2D symmetryObject;
SymmetryObject symmetryObject;
SymmetryDrawable *symmetryDrawable = 0;
Camera *camera;

int mode = 0;



const unsigned char 	SP = 2;		// positiv rotation i triangeln
const unsigned char 	SN = 3;		// negativ rotation i triangeln
const unsigned char 	RP = 4;		// positiv rotation till nästa triangel
const unsigned char 	RN = 5;		// negativ rotation tlil föregående triangel


int symmetryType = 0;

int mouseX = -100;
int mouseY = -100;

int indexChosen = -1;
int indexMouseOver = -1;

double minimumDistanceForMouseOverSquare = 0.0003;

void graphDisplay()
{
	if (symmetryDrawable)
		symmetryDrawable->display();

	if (camera)
		camera->updateCamera();
}

void initGraph()
{
		// initiera först med en lite halvful olaglig c, dynamic_cast<>() funkar inte ens för att bryta mot lagarna, men jag vet vad jag gör så håll käft!!!
	symmetryDrawable = (SymmetryDrawable*) &symmetryObject;
	camera = new Camera();


	cout << "fixar kamera rotation nu" << endl;;
	/*

	double angle = -0.31;
	double c = cos(angle);
	double s = sin(angle);
	camera->setPositionAndOrientation(
			VEC(.5, .0005, 1.0),
			MAT(	c, 0, s,
					0, 1, 0,
					-s,0, c),
			5000);*/
	cout << "done" << endl;


	//camera->updateCamera();
}

void setMousePosition(int x, int y)
{
	mouseX = x;
	mouseY = y;
	VEC coords = camera->fromABtoXY(x, y);
	Point p = symmetryObject.getClosestPoint(coords);
	if (p.isDefined()) {
		VEC closestVertex = symmetryObject.getVec(p);
		
		if ((closestVertex-coords)*(closestVertex-coords) < minimumDistanceForMouseOverSquare)
			indexMouseOver = p.index;
		else
			indexMouseOver = -1;
	} else 
		indexMouseOver = -1;

	if (indexMouseOver != -1)
		return;

	double distanceToCenteredSquare = 0;
	p = getClosestCenteredPoint(coords, &distanceToCenteredSquare);
	if (distanceToCenteredSquare < minimumDistanceForMouseOverSquare)
	{
		indexMouseOver = p.index;
	}
}


	// AB är koordinater på skärmen, pixelposition


	//XY är transformerade koordinater, edgelängden = 1.0
/*void fromXYtoAB(VEC XY, int *ABx, int *ABy)
{
	*ABx = scrWidth*(XY.x + 0.5)*2.0/3.0;
	*ABy = scrHeight*(XY.y + 1.0)*COS30;
}*/



list<string> mouseClick(int x, int y)
{
	list<string> commMsgList;
	cout << "here klickas it: " << camera->fromABtoXY(x, y) << endl;

	static char strMess[200];

	VEC realCoord(camera->fromABtoXY(x, y));
	VEC rootCoord = getRootpoint(realCoord);


	if (mode == 0)
	{
		cout << "mode = 0, mouseklick: [" << rootCoord.x << ", " << rootCoord.y << "] = " << indexMouseOver << endl;
    	if (indexMouseOver < 0) {

    		indexChosen = indexMouseOver;
    		if (indexMouseOver == NOT_CENTERED) {
    			double distanceToCenteredVertex;
    			Point P = getClosestCenteredPoint(realCoord, &distanceToCenteredVertex);

    			if (distanceToCenteredVertex < minimumDistanceForMouseOverSquare) {
    				symmetryObject.insertCenteredVertex(P.index);
    				indexChosen = indexMouseOver = P.index;
    			} else {
    				indexChosen = indexMouseOver = symmetryObject.insertVertex(rootCoord);
    			}
    		}
    		int msgtyp;
    		switch(indexMouseOver)
    		{
    			case VERTEX_CENTERED:
    				//msgtyp = vertexPointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
    				msgtyp = symmetryObject.getCenteredActive(VERTEX_CENTERED)? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_VERTEX;
    				rootCoord = SymmetryObject::vertexCenteredPoint;
    				symmetryObject.insertCenteredVertex(VERTEX_CENTERED);
    				//symmetryObject.vertexPointActive = true;
    				break;
    			case EDGE_CENTERED:
					//msgtyp = edgePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
    				msgtyp = symmetryObject.getCenteredActive(EDGE_CENTERED)? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_VERTEX;
    				rootCoord  = SymmetryObject::edgeCenteredPoint;
    				symmetryObject.insertCenteredVertex(EDGE_CENTERED);
    				//symmetryObject.edgePointActive = true;
    				break;
    			case FACE_CENTERED:
					//msgtyp = facePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
    				msgtyp = symmetryObject.getCenteredActive(FACE_CENTERED)? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_VERTEX;
    				rootCoord = SymmetryObject::faceCenteredPoint;
    				symmetryObject.insertCenteredVertex(FACE_CENTERED);
    				//symmetryObject.facePointActive = true;
    				break;
    			default:
    				msgtyp = COMM_MSGTYP_ADD_VERTEX;//vertexPointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
    				break;
    		}

    		cout << "msgtyp: " << msgtyp << endl;
			snprintf(strMess, 200, "%d, %d, %.3f, %.3f, %.3f, 0.000", COMM_MSGTYP_UPDATE_VERTEX, indexMouseOver, rootCoord.x, rootCoord.y, rootCoord.z);
			commMsgList.push_back(strMess);


		} else {	// väljer en vertex som man klickat på.
				indexChosen = indexMouseOver;
				snprintf(strMess, 200, "%d, %d", COMM_MSGTYP_CHOOSE_VERTEX, indexChosen);
				commMsgList.push_back(strMess);
		}
	} else if (mode == 1 && indexMouseOver != -1) {

		Point nyPunkt = symmetryObject.getClosestPoint(realCoord);


		cout << "mode = 1, mouseklick: [";


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
		int hurSluten;

		switch(sluten)
		{
			case 2:
				hurSluten = NOT_CENTERED;
				cout << "DET blev ett vanligt FACE" << endl;
				break;
			case VERTEX_CENTERED:
				hurSluten = VERTEX_CENTERED;
				cout << "DET blev en VERTEX FACE" << endl;
				break;
			case EDGE_CENTERED:
				hurSluten = EDGE_CENTERED;
				cout << "DET blev en EDGE FACE" << endl;
				break;
			case FACE_CENTERED:
				hurSluten = FACE_CENTERED;
				cout << "DET blev en FACE FACE" << endl;
				break;
			default:
				hurSluten = -1000;
				cout << "Det blev jättefel här" << endl;
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

					// Addera facet
				// msgtyp, id, fr, len, type, flat
				//snprintf(strMess, 200, "%d, %d, %d", COMM_MSGTYP_UPDATE_FACE, symmetryObject.F.size(), sluten);
				snprintf(strMess, 200, "%d, %d, %d, %d, %d, 0.000", 
					COMM_MSGTYP_UPDATE_FACE, symmetryObject.F.size(), 
					symmetryObject.E.size(), SymmetryObject::E_ToBe.size(), 
					sluten==2? NOT_CENTERED: sluten);


				//snprintf(strToSend, 200, "%d,%d,%d,%d", 
				//F.size()-1, sizE, E.size() - sizE,
				// (sluten == 2? -1: sluten));

				commMsgList.push_back(strMess);

				symmetryObject.addFaceToBe(sluten==2? NOT_CENTERED: sluten);

						// Addera alla edges
				//int fNum = symmetryObject.F.size()-1;
				//int e = 0;
				for (int f=0; f<symmetryObject.F.size(); f++)
				{
					for (int e=symmetryObject.F[f].fr; e<symmetryObject.F[f].fr + symmetryObject.F[f].edges; e++)
					{
							// id, fr, to, next, prev, oppo, face, len, k, d, len0, theta
						snprintf(strMess, 200, "%d, %d, %s, %s, %s, %s, %s, %d", COMM_MSGTYP_UPDATE_EDGE, e, symmetryObject.E[e].fr.toString().c_str(),
							symmetryObject.E[e].to.toString().c_str(), symmetryObject.E[e].next.toString().c_str(),
							symmetryObject.E[e].prev.toString().c_str(), symmetryObject.E[e].oppo.toString().c_str(), f);
						cout << "graph2d.cpp, strMess: \t" << strMess << endl;
						symmetryObject.E[e].print();
						cout << endl;


						commMsgList.push_back(strMess);
					}
				}

				cout << "PRINT ALL:" << endl;
				symmetryObject.printAll();

				cout << endl;
				break;}
			default:
				cout << "Hit ska den icke anlanda" << endl;
				break;
		}
	}

	return commMsgList;
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
			if (!symmetryObject.fullTest())
			{
				cout << "Graph2D:	failed fullTest()" << endl;
				return mode;
			}


			double angle = -0.31;
			double c = cos(angle);
			double s = sin(angle);

			camera->setPositionAndOrientation(
				VEC(.5, .0005, 1.0),
				MAT(	c, 0, s,
						0, 1, 0,
						-s,0, c),
				5000);

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
	double delta_ = 0.02;
	
	glBegin(GL_LINES);
		glVertex3f(_P.x-delta_, _P.y, 0.0);
		glVertex3f(_P.x+delta_, _P.y, 0.0);
	glEnd();
	
	glBegin(GL_LINES);
		glVertex3f(_P.x, _P.y-delta_, 0.0);
		glVertex3f(_P.x, _P.y+delta_, 0.0);
	glEnd();
}

void SymmetryDrawable::drawCircle(VEC _P, bool filled)	// egentligen en hexagon
{
	double delta_ = 0.02;

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
		glVertex3f(0., 0., 0.);

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
	for (unsigned int f=0; f<F.size(); f++)
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
	VEC coords_ = camera->fromABtoXY(mouseX, mouseY);
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
  	  	} else {
  	  		//cout << "mouse over: " << indexMouseOver << endl;
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
	for (unsigned int v = 0; v < V.size(); v++)
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


