#include "graph2d.h"


using namespace std;

namespace Graph2D {

	const unsigned char SP = 1;		// positiv rotation i triangeln
	const unsigned char SN = 2;		// negativ rotation i triangeln
	const unsigned char RP = 4;		// positiv rotation till nästa triangel
	const unsigned char RN = 8;		// negativ rotation tlil föregående triangel


	TYP xMin = 0;
	TYP xMax = 0;
	TYP yMin = 0;
	TYP yMax = 0;
	int scrWidth = 0;
	int scrHeight = 0;
	bool facePointActive = false;
	bool vertexPointActive = false;

	int mouseX = -100;
	int mouseY = -100;

	std::vector<point> V;
	std::vector<edge> E;

		// face-centered point = -2
		// vertex-centered point = -1
	struct point {
		TYP x;
		TYP y;

		point() 				{x = y = 0;}
		point(TYP x_, TYP y_) 	{x = x_; 	y = y_;}
		point(const point &A_) 	{x = A_.x;	y = A_.y;}
		void print()
		{
			std::cout << "[" << x << ", " << y << "]" << std::endl;
		}
	};	

		// hänvisar till objekt i relation till sig själv
	struct prefix {
		unsigned char r[10];

		void update()
		{

		}

		point getPoint(int v)
		{
			return point(V[v].x, V[v].y)	;
		}

	};



	struct Point {
		prefix P;
		int v;
		point getPoint()
		{
			point X_ = V[v];
		}
	};

	struct Edge {
		prefix P;
		int e;
	};

	struct edge {
		Point fr;
		Point to;

		Edge next;
		Edge prev;
		Edge oppo;
	};

	struct direction {
		TYP Rx, Ry;
		TYP Sx, Sy;

		direction() {
			Rx = Ry = 0;
			Sx = 0.5;
			Sy = COS30/3;
		}

		void rotate(int dir_)
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

		point rotate(int dir_, point p_)
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


		void print()
		{
			std::cout << "R: " << Rx << ", " << Ry << std::endl;
			std::cout << "S: " << Sx << ", " << Sy << std::endl;
		}

	};



	void printDirection(int x, int y)
	{

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
		dist[0] = p_.x*p_.x + p_.y*p_.y;

		d_.rotate(SP);
		dist[1] = (p_.x-d_.Rx)*(p_.x-d_.Rx) + (p_.y-d_.Ry)*(p_.y-d_.Ry);

		d_.rotate(SP);
		dist[2] = (p_.x-d_.Rx)*(p_.x-d_.Rx) + (p_.y-d_.Ry)*(p_.y-d_.Ry);

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



	void drawPoint(point _P)
	{
		glColor3f(1, 0, 0);
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

	void display()
	{
			    /* clear all pixels */
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    	// rita bräde:
	    drawBrade();

	    	// rita en mus
    	TYP mx = xMin + mouseX*(xMax - xMin)/scrWidth;
    	TYP my = yMax - mouseY*(yMax - yMin)/scrHeight;
    	point coords_(fromABtoXY(mouseX, mouseY));
	    //drawPoint(coords_);

    	coords_ = getRootPoint(coords_);
		//drawPoint(coords_);

    	direction d_;
    	coords_ = d_.rotate(RP, coords_);
    	for (int tri = 0; tri<3; tri++)
    	{
    		for (int sect = 0; sect < 3; sect++)
    		{
    			coords_ = d_.rotate(SP, coords_);
    			drawPoint(coords_);
    		}
			coords_ = d_.rotate(RN, coords_);
    	}
	    

	    glutSwapBuffers();
	}
}
