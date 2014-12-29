#include "graph2d.h"



namespace Graph2D {
	int scrWidth = 0;
	int scrHeight = 0;

	struct prefix {
		unsigned char r[10];
		void update()
		{
			
		}
	};

	struct point {
		TYP x;
		TYP y;
	};


	struct edge {
		prefix preFr;
		int fr;

		prefix preTo;
		int to;
	};

	std::vector<point> V;
	std::vector<edge> E;

		// AB är koordinater på skärmen, pixelposition
	point fromABtoXY(int x, int y)
	{
		point Ret;
		Ret.x = (1.5*x) / scrWidth - 0.5;
		Ret.y = (y/COS30) / scrHeight - 1.0;
		return Ret;
	}

	void fromXYtoAB(point XY, int *ABx, int *ABy)
	{
		*ABx = scrWidth*(XY.x + 0.5)*2.0/3.0;
		*ABy = scrHeight*(XY.y + 1.0)*COS30;
	}
}

/*


		//XY är transformerade koordinater, edgelängden = 1.0
	void fromXYtoAB(point2D XY, int *ABx, int *ABy)
	{
		*ABx = scrWidth*(XY.x + 0.5)*2.0/3.0;
		*ABy = scrHeight*(XY.y + 1.0)*COS30;
	}*/
