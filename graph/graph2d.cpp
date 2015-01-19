#include "graph2d.h"
#include "orientation/orientation.h"

using namespace std;

namespace Graph2D {

	const int VERTEX_CENTERED = -2;
	const int EDGE_CENTERED = -3;
	const int FACE_CENTERED = -4;

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


	Prefix preE_ToBe;
	vector<edge> E_ToBe;

	
	int indexChosen = -1;
	int indexMouseOver = -1;

	const point vertexCenteredPoint(.0, .0);
	const point edgeCenteredPoint(.25, SIN60*.5);
	const point faceCenteredPoint(.5, .25/COS30);

	point Point::getpoint()
	{
		point rootpoint_;
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
					rootpoint_ = point(-100, -100);
					break;
			}
		} else if (index >= V.size()) {
			cout << "hit skulle den ju inte komma ju ju ju " << endl;
			return point(-100, -100);
		} else {
			rootpoint_ = V[index];
		}

		Orientation ori_;
		point rotatedpoint_ = ori_.getOCFromWC(rootpoint_);
		ori_.rotate(Pfx);
		rotatedpoint_ = ori_.getWCFromOC(rotatedpoint_);
		return rotatedpoint_;
		
		// niclas nosch 11-12
		// erik är från combitech
	}

	void Point::print()
	{
		cout << "(" << index << "), ";
		Pfx.print();
		cout << ", ";
		this->getpoint().print();
	}



	void edge::print()
	{
		cout << "\tPoint fr = ";
		fr.print();
		cout << endl << "\tPoint to = ";
		to.print();
		cout << endl;
		cout << "\tEdge next = " << 0 << endl;
		cout << "\tEdge prev = " << 0 << endl;
		cout << "\tEdge oppo = " << 0 << endl;
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



		// returnerar 
		// 0 om det är felaktigt
		// 1 om det är ok men icke sluten
		// 2 om face är ok.
	int checkE_ToBe()
	{
		if (E_ToBe.size() < 3)
			return 1;


			// kolla om den roterar i positiv z-riktning
		point fr_ = E_ToBe[0].fr.getpoint();
		point to_ = E_ToBe[0].to.getpoint();
		point edge0_ = to_ - fr_;
		point edge1_;
		for (int i=1; i<E_ToBe.size()-1; i++)
		{
			//cout << "i: " << endl;
			fr_ = to_;
			to_ = E_ToBe[i].to.getpoint();
			edge1_ = to_ - fr_;

			/*cout << "edge0: ";
			edge0_.print();
			cout << endl << "edge1_";
			edge1_.print();
			cout << endl;*/

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


			// kolla om den passerar sig själv
		for (int i=2; i<E_ToBe.size(); i++)
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



			// allt funkar.
		return 2;
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

	Point mouseOverPoint(point co_)
	{
		Point P_;
		P_.Pfx = getPrefix(co_);
		co_ = getRootpoint(co_);
		P_.index = mouseOverIndex(co_);
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
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), (char*)&VERTEX_CENTERED);
					commSendMsg(&wakeV);

				} else if (indexMouseOver == EDGE_CENTERED) {
					//indexChosen = EDGE_CENTERED;
					int msgtyp = edgePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
					edgePointActive = true;
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), (char*)&EDGE_CENTERED);
					commSendMsg(&wakeV);

				} else if (indexMouseOver == FACE_CENTERED) {
					//indexChosen = FACE_CENTERED;
					int msgtyp = facePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
					facePointActive = true;
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), (char*)&FACE_CENTERED);
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

			cout << "sluten: " << checkE_ToBe() << endl;
		}

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

	    glutSwapBuffers();
	}
}
