#include "graph2d.h"


using namespace std;

namespace Graph2D {

	const unsigned char SP = 1;		// positiv rotation i triangeln
	const unsigned char SN = 2;		// negativ rotation i triangeln
	const unsigned char RP = 4;		// positiv rotation till nästa triangel
	const unsigned char RN = 8;		// negativ rotation tlil föregående triangel

	const int VERTEX_CENTERED = -2;
	const int EDGE_CENTERED = -3;
	const int FACE_CENTERED = -4;

	int mode = 0;

	TYP xMin = 0;
	TYP xMax = 0;
	TYP yMin = 0;
	TYP yMax = 0;
	int scrWidth = 0;
	int scrHeight = 0;
	
	bool facePointActive = false;
	bool edgePointActive = false;
	bool vertexPointActive = false;

	int mouseX = -100;
	int mouseY = -100;

	std::vector<point> V;
	std::vector<edge> E;


	prefix preE_ToBe;
	vector<edge> E_ToBe;

	
	int vertexChosen = -1;
	int vertexMouseOver = -1;
	struct direction;
	//point direction::rotate(int dir_, point p_);

		// face-centered point = -3
		// edge-centered point = -2
		// vertex-centered point = -1

	point::point() 				
	{
		x = y = 0;
	}
	point::point(TYP x_, TYP y_) 	
	{
		x = x_; 	
		y = y_;
	}
	point::point(const point &A_) 	
	{
		x = A_.x;	
		y = A_.y;
	}
	void point::print()
	{
		std::cout << "[" << x << ", " << y << "]" << std::endl;
	}

	//point point::getpoint(Point p_);

	const point vertexCenteredPoint(.0, .0);
	const point edgeCenteredPoint(.25, SIN60*.5);
	const point faceCenteredPoint(.5, .25/COS30);


		// hänvisar till objekt i relation till sig själv


	prefix::prefix() {
		for (int i=0; i<10; i++)
			r[i] = (unsigned char) 0;
	}

	void prefix::update()
	{

	}

	void prefix::print()
	{
		cout << "[";
		
		for (int i=0; i<10 && r[i]; i++)
		{
			if (i)
				cout << ", ";

			switch(r[i]) {
				case SP:
					cout << "SP";
					break;
				case RP:
					cout << "RP";
					break;
				case SN:
					cout << "SN";
					break;
				case RN:
					cout << "RN";
					break;
				default:
					cout << "XX";
					break;
			}
		}
		cout << "]";
	}




	point Point::getpoint()
	{
		//point X_ = V[v];
		//cout << "kom in i getpoint()" << endl;
		point tjena_;
		//cout << "v = " << v << endl;
		if (v < 0) {
			switch (v)
			{
				case VERTEX_CENTERED:
					tjena_ = vertexCenteredPoint;
					break;
				case EDGE_CENTERED:
					tjena_ = edgeCenteredPoint;
					break;
				case FACE_CENTERED:
					tjena_ = faceCenteredPoint;
					break;
				default:
					cout << "it became default o det ska det inte" << endl;
					tjena_ = point(-100, -100);
					break;
			}
		} else if (v >= V.size()) {
			cout << "hit skulle den ju inte komma ju ju ju " << endl;
			return point(-100, -100);
		} else {
			tjena_ = V[v];
		}
		direction dir_;
		for (int i=0; i<10 && P.r[i]; i++)
		{
			//cout << "i: " << i << endl;
			tjena_ = dir_.rotate(P.r[i], tjena_);
		}
		
		return tjena_;
	}

	void Point::print()
	{
		cout << "(" << v << "), ";
		P.print();
		cout << ", ";
		this->getpoint().print();
		//getpoint(*this).print();
	}

	
	point point::operator+(point &A)
	{
		return point(x + A.x, y + A.y);
	}
	void point::operator+=(point &A)
	{
		x += A.x;
		y += A.y;
	}

	point point::operator-(point &A)
	{
		return point(x - A.x, y - A.y);
	}
	void point::operator-=(point &A)
	{
		x -= A.x;
		y -= A.y;
	}

	point point::operator*(TYP a)
	{
		return point(x*a, y*a);
	}

	void point::operator*=(TYP a)
	{
		x *= a;
		y *= a;
	}

	TYP point::operator&(point &A)
	{
		return x*A.y - y*A.x;
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


	direction::direction() {
		Rx = Ry = 0;
		Sx = 0.5;
		Sy = COS30/3;
	}

	void direction::rotate(int dir_)
	{
		rotate(dir_, point(0,0));
		return;
		switch (dir_)
		{
			case SP:{
				TYP Rx_ = Sx + (Rx-Sx)*COS120 - (Ry-Sy)*SIN120;
				Ry = Sy + (Rx-Sx)*SIN120 + (Ry-Sy)*COS120;
				Rx = Rx_;
				break;
				}
			case SN:{
				TYP Rx_ = Sx + (Rx-Sx)*COS120 + (Ry-Sy)*SIN120;
				Ry = Sy - (Rx-Sx)*SIN120 + (Ry-Sy)*COS120;
				Rx = Rx_;
				break;
				}
			case RP:{
				TYP Sx_ = Rx + (Sx-Rx)*COS60 - (Sy-Ry)*SIN60;
				Sy = Ry + (Sx-Rx)*SIN60 + (Sy-Ry)*COS60;
				Sx = Sx_;
				break;}
			case RN:{
				TYP Sx_ = Rx + (Sx-Rx)*COS60 + (Sy-Ry)*SIN60;
				Sy = Ry - (Sx-Rx)*SIN60 + (Sy-Ry)*COS60;
				Sx = Sx_;
				break;
				}
		}
	}

	point direction::rotate(int dir_, point p_)
	{
		TYP x_;
		switch (dir_)
		{
			case SP:
				x_ = Sx + (Rx-Sx)*COS120 - (Ry-Sy)*SIN120;
				Ry = Sy + (Rx-Sx)*SIN120 + (Ry-Sy)*COS120;
				Rx = x_;
				
				x_ = Sx + (p_.x-Sx)*COS120 - (p_.y-Sy)*SIN120;
				p_.y = Sy + (p_.x-Sx)*SIN120 + (p_.y-Sy)*COS120;
				p_.x = x_;					
				break;
				
			case SN:
				x_ = Sx + (Rx-Sx)*COS120 + (Ry-Sy)*SIN120;
				Ry = Sy - (Rx-Sx)*SIN120 + (Ry-Sy)*COS120;
				Rx = x_;
				x_ = Sx + (p_.x-Sx)*COS120 + (p_.y-Sy)*SIN120;
				p_.y = Sy - (p_.x-Sx)*SIN120 + (p_.y-Sy)*COS120;
				p_.x = x_;
				break;
				
			case RP:
				x_ = Rx + (Sx-Rx)*COS60 - (Sy-Ry)*SIN60;
				Sy = Ry + (Sx-Rx)*SIN60 + (Sy-Ry)*COS60;
				Sx = x_;
				x_ = Rx + (p_.x-Rx)*COS60 - (p_.y-Ry)*SIN60;
				p_.y = Ry + (p_.x-Rx)*SIN60 + (p_.y-Ry)*COS60;
				p_.x = x_;
				break;
			case RN:
				x_ = Rx + (Sx-Rx)*COS60 + (Sy-Ry)*SIN60;
				Sy = Ry - (Sx-Rx)*SIN60 + (Sy-Ry)*COS60;
				Sx = x_;
				x_ = Rx + (p_.x-Rx)*COS60 + (p_.y-Ry)*SIN60;
				p_.y = Ry - (p_.x-Rx)*SIN60 + (p_.y-Ry)*COS60;
				p_.x = x_;
				break;
				
		}
		return p_;
	}


	void direction::print()
	{
		std::cout << "R: " << Rx << ", " << Ry << std::endl;
		std::cout << "S: " << Sx << ", " << Sy << std::endl;
	}


	point getRootPoint(point p_)
	{
		//point p_ = fromABtoXY(x, y);

		//std::cout << "--------------" << std::endl;
		direction d_;
		int inTri;

		if (p_.y > 0) {
			if (p_.x*COS30 > SIN30*p_.y && p_.x>0)
				inTri = 0;
			else {
				inTri = 1;
				d_.rotate(RP);
			}
			//inTri = ((p_.x/p_.y > SIN30/COS30 && p_.x>0) ? 0: 1);
		} else {
			inTri = 2;
			d_.rotate(RN);
		}

		int sect;

		TYP dist[3];

		for (int i=0; i<3; i++)
		{
			dist[i] = (p_.x-d_.Rx)*(p_.x-d_.Rx) + (p_.y-d_.Ry)*(p_.y-d_.Ry);
			if ((p_.x-d_.Rx)*(d_.Sx-d_.Rx) + (p_.y-d_.Ry)*(d_.Sy-d_.Ry) > 0.5)
				return point(-100, -100);

			if (i != 2)
				d_.rotate(SP);
		}

		if (dist[0] < dist[1])
			sect = (dist[0]<dist[2] ?0 :2);
		else
			sect = (dist[1]<dist[2] ?1 :2);
		
		//cout << "Triangel: " << inTri << "\tsection: " << sect << endl;
		switch(sect)
		{
			case 0:
				d_.rotate(SP);
				break;
			case 1:
				d_.rotate(SN);
				break;
		}
		//d_.print();
			///////////////////////
		point realp(p_.x, p_.y);
		switch(sect)
		{
			case 1:
				realp = d_.rotate(SN, realp);
				break;
			case 2:
				realp = d_.rotate(SP, realp);
				break;
		}

		switch(inTri)
		{
			case 1:
				realp = d_.rotate(RN, realp);
				break;
			case 2:
				realp = d_.rotate(RP, realp);
				break;
		}

		return realp;
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
			cout << "i: " << endl;
			fr_ = to_;
			to_ = E_ToBe[i].to.getpoint();
			edge1_ = to_ - fr_;

			cout << "edge0: ";
			edge0_.print();
			cout << endl << "edge1_";
			edge1_.print();
			cout << endl;

			if ((edge0_ & edge1_) < 0.0)
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


			// allt funkar.
		return 2;
	}


	int mouseOverVertex(point co_)
	{
		TYP radius2 = 20.0 / (scrHeight + scrHeight);
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
	int mouseOverVertex(int x, int y)
	{
		return  mouseOverVertex(getRootPoint(fromABtoXY(x, y)));
	}

	prefix getRootPrefix(point coord_);

	Point mouseOverVertex2(int x, int y)
	{
		point co_ = fromABtoXY(x, y);

		Point P_;
		P_.P = getRootPrefix(co_);
		co_ = getRootPoint(co_);
		P_.v = mouseOverVertex(co_);
		//return  mouseOverVertex(getRootPoint(co_));
		return P_;
	}

	prefix getRootPrefix(point coord_)
	{
		int v_ = -1;
		point g_[2];
		cout << "\t\tgetRootPrefix:" << endl;
		cout << "coords: " << coord_.x << ", " << coord_.y << endl;

		if (coord_.x < -coord_.y*TAN30) {
			cout << "hamnar utanfor 1" << endl;
			return prefix();
		} else if (coord_.x > 1.0 - coord_.y*TAN30) {
			cout << "hamnar utanfor 2" << endl;
			return prefix();
		} else if (coord_.y > COS30) {
			cout << "hamnar utanfor 3" << endl;
			return prefix();
		} else if (coord_.x > 1.0 + coord_.y*TAN30) {
			cout << "hamnar utanfor 4" << endl;
			return prefix();
		} else if (coord_.y < 0) {
			g_[0] = point(SIN30, -COS30);
			g_[1] = point(1.0, 0);
			v_ = 8;
		} else if (coord_.x > coord_.y*TAN30) {
			g_[0] = point(1.0, 0.0);
			g_[1] = point(SIN30, COS30);
			v_ = 0;
		} else {
			g_[0] = point(SIN30, COS30);
			g_[1] = point(-SIN30, COS30);
			v_ = 4;
		}

		cout << "first v: " << v_ << endl;

		TYP diff0 = coord_.x*coord_.x +  coord_.y*coord_.y;
		TYP diff1 = (coord_.x-g_[0].x)*(coord_.x-g_[0].x) + (coord_.y-g_[0].y)*(coord_.y-g_[0].y);
		TYP diff2 = (coord_.x-g_[1].x)*(coord_.x-g_[1].x) + (coord_.y-g_[1].y)*(coord_.y-g_[1].y);
		if (diff0 > diff1 || diff0 > diff2) {
			v_ += diff1 < diff2? 1: 2;
		}

		cout << "final v: " << v_ << endl;

		prefix p_;
		switch (v_)
		{
			case 1:
				p_.r[0] = SP;
				break;
			case 2:
				p_.r[0] = SN;
				break;
			case 4:
				p_.r[0] = RP;
				break;
			case 5:
				p_.r[0] = RP;
				p_.r[1] = SP;
				break;
			case 6:
				p_.r[0] = RP;
				p_.r[1] = SN;
				break;
			case 8:
				p_.r[0] = RN;
				break;
			case 9:
				p_.r[0] = RN;
				p_.r[1] = SP;
				break;
			case 10:
				p_.r[0] = RN;
				p_.r[1] = SN;
				break;
			default:
				cout << "Default" << endl;
				break;
		}
		cout << "prefix: ";
		p_.print();

		cout << "---------" << endl;
		return p_;
	}

	void setMousePosition(int x, int y)
	{
		mouseX = x;
		mouseY = y;
		vertexMouseOver = mouseOverVertex(x, y);
		//cout << vertexMouseOver;
	}

	void mouseClick(int x, int y)
	{
		if (mode == 0)
    	{
    		point coord_(fromABtoXY(x, y));
    		coord_ = getRootPoint(coord_);

    		vertexMouseOver = mouseOverVertex(x, y);
    		cout << "mode = 0, mouseklick: [" << coord_.x << ", " << coord_.y << "] = " << vertexMouseOver << endl;
    	
	    	if (vertexMouseOver < 0) {
	    		vertexChosen = vertexMouseOver;



	    		if (vertexMouseOver == -1) {
					vertexChosen = vertexMouseOver = insertVertex(coord_);
		    	} else if (vertexMouseOver == VERTEX_CENTERED) {
		    		//vertexChosen = VERTEX_CENTERED;
					int msgtyp = vertexPointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
					vertexPointActive = true;
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), (char*)&VERTEX_CENTERED);
					commSendMsg(&wakeV);

				} else if (vertexMouseOver == EDGE_CENTERED) {
					//vertexChosen = EDGE_CENTERED;
					int msgtyp = edgePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
					edgePointActive = true;
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), (char*)&EDGE_CENTERED);
					commSendMsg(&wakeV);

				} else if (vertexMouseOver == FACE_CENTERED) {
					//vertexChosen = FACE_CENTERED;
					int msgtyp = facePointActive? COMM_MSGTYP_CHOOSE_VERTEX: COMM_MSGTYP_ADD_CENTERED_VERTEX;
					facePointActive = true;
					CommMsg wakeV(COMM_THREAD_GLUT, COMM_THREAD_MAIN, 
						msgtyp, 0, sizeof(int), (char*)&FACE_CENTERED);
					commSendMsg(&wakeV);
				}
			} else {
					vertexChosen = vertexMouseOver;
					CommMsg nyttMess(COMM_THREAD_GLUT, COMM_THREAD_MAIN, COMM_MSGTYP_CHOOSE_VERTEX, 0, sizeof(int), (char*)&vertexChosen);
					commSendMsg(&nyttMess);
					nyttMess.destroy();
			}
		} else if (mode == 1 && vertexMouseOver != -1) {

			Point nyPunkt = mouseOverVertex2(x, y);
	
    		//point coord_(fromABtoXY(x, y));
    		//Point nyPunkt = getRootPoint2(coord_);

    		//vertexMouseOver = mouseOverVertex(V[nyPunkt.v].x, V[nyPunkt.v].y);
    		cout << "mode = 1, mouseklick: [";// << coord_.x << ", " << coord_.y << "] = " << vertexMouseOver << endl;


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
    	coord_ = getRootPoint(coord_);
	}

	void drawPoint(point _P)
	{
		int siz_ = 10;	// 10 pixlar hög och 10 pixlar bred punkt
		TYP delta_ = siz_ *(xMax - xMin)*0.5/scrWidth;

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

	void drawCircle(point _P, bool filled)
	{
		int siz_ = 10;	// 10 pixlar hög och 10 pixlar bred punkt
		TYP delta_ = siz_ *(xMax - xMin)*0.5/scrWidth;

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
		if (vRoot_.y <= -100)
		{
			for (int i=0; i<9; i++)
				vAll_[i] = vRoot_;
			return;
		}

    	direction d_;
    	int n = 1;
    	point nyItv(d_.rotate(RP, vRoot_));
    	for (int tri = 0; tri<3; tri++)
    	{
    		for (int sect = 0; sect < 3; sect++)
    		{
    			nyItv = d_.rotate(SP, nyItv);
    			vAll_[(tri!=1 || sect!=2)? n++: 0] = nyItv;
    		}
			nyItv = d_.rotate(RN, nyItv);
    	}
	}	

	void setColorOfVertex(int vert_, TYP str_)
	{
		    // vald, över eller inte = vit
		    // över, men icke vald = turkos
		    // icke vald och icke över = rosa
		if (vertexChosen == vert_)
			glColor3f(1.0*str_, 1.0*str_, 1.0*str_);
		else if (vertexMouseOver == vert_)
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
		if (vertexPointActive || vertexMouseOver == VERTEX_CENTERED)
    		for (int i=1; i<9; i++)
    			drawCircle(vAll_[i], vertexPointActive);

    		// EDGE
  	  	setColorOfVertex(EDGE_CENTERED, 1.0);
  	  	getAllFromRoots(edgeCenteredPoint, vAll_);
  	  	if (edgePointActive || vertexMouseOver == EDGE_CENTERED)
	    	for (int i=1; i<9; i++)
	    		drawCircle(vAll_[i], edgePointActive);

    		// FACE
    	setColorOfVertex(FACE_CENTERED, 1.0);
		getAllFromRoots(faceCenteredPoint, vAll_);
		if (facePointActive || vertexMouseOver == FACE_CENTERED)
	    	for (int i=1; i<9; i++)
	    		drawCircle(vAll_[i], facePointActive);



	    	// rita musem p, om inte mouseOver
    	point coords_ = fromABtoXY(mouseX, mouseY);
		if (mode == 0) {
			if (vertexMouseOver == -1) {
				coords_ = getRootPoint(coords_);

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
				//cout << "i: " << i << "\t";
				//cout << "to: " << E_ToBe[i].fr.v << endl;
				point p_ = E_ToBe[i].fr.getpoint();
				glVertex3f(p_.x, p_.y, 0.0);
			}
			//cout << ":";
			glVertex3f(coords_.x, coords_.y, 0.0);
			glEnd();
			//if (vertexMouseOver == -1) {

			//} else {

			//}
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
