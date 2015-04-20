#include "graph2d.h"

using namespace std;

Camera::Camera() {
	//	VEC position;
	//	MAT orientation;

	scrWidth = 0;
	scrHeight = 0;

    /* select clearing (background) color */
    glClearColor (0.0, 0.0, 0.0, 0.0);

    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    xMin = -.5;
    xMax = 1;
    yMin = -.5;
    yMax = 1;

    gluLookAt(	0, 0, 1,	// position
    			0, 0, 0,	// en punkt jag tittar på
				0, 1, 0);	// uppåt på skärmen

    glOrtho(xMin, xMax, yMin, yMax, -1.0, .5);

    setPositionAndOrientationFromProjectionMatrix();
}

void Camera::updateCamera()
{

}

void Camera::setPositionAndOrientationFromProjectionMatrix()
{
	double matProjection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);

	double td;
	ori.xx = matProjection[0];
	ori.xy = matProjection[4];
	ori.xz = matProjection[8];
	td = sqrt(ori.xx*ori.xx + ori.xy*ori.xy + ori.xz*ori.xz);
	ori.xx /= td;
	ori.xy /= td;
	ori.xz /= td;


	ori.yx = matProjection[1];
	ori.yy = matProjection[5];
	ori.yz = matProjection[9];
	td = sqrt(ori.xx*ori.xx + ori.xy*ori.xy + ori.xz*ori.xz);
	ori.yx /= td;
	ori.yy /= td;
	ori.yz /= td;

	ori.zx = matProjection[2];
	ori.zy = matProjection[6];
	ori.zz = matProjection[10];

	cout << endl << endl << "\tmatProjection" << endl;
	for (int w=0; w<4; w++)
	{
		for (int v=0; v<4; v++)
			cout << matProjection[4*w + v] << ", ";
		cout << endl;
	}



	int viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );

	cout << endl << endl << "\tviewport" << endl;
	for (int v=0; v<4; v++)
			cout << "v: " << viewport[v] << endl;

	cout << "kom ur" << endl;
}

VEC Camera::fromABtoXY(int x, int y)
{
	return VEC(
		xMin + x*(xMax - xMin)/scrWidth,
		yMax - y*(yMax - yMin)/scrHeight);

}

void Camera::fromXYtoAB(VEC XY, int *ABx, int *ABy)
{
	*ABx = scrWidth*(XY.x + 0.5)*2.0/3.0;
	*ABy = scrHeight*(XY.y + 1.0)*COS30;
}

void Camera::setScreenSize(int x, int y)
{
	scrWidth = x;
	scrHeight = y;
	glViewport(0, 0, scrWidth, scrHeight);
}

MAT Camera::getOrientation()
{
	return ori;
}

VEC Camera::getPosition()
{
	return pos;
}


void Camera::setPosition(VEC newPos) {}

void Camera::rotateWC(VEC rot, double angle) {}
void Camera::rotateSC(VEC rot, double angle) {}
void Camera::translateWC(VEC tra) {}
void Camera::translateSC(VEC tra) {}


void Camera::setPositionAndOrientation(const VEC &newPos, const MAT &newOri, double timeLapse)
{

}


