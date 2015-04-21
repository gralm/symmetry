#include "graph2d.h"

using namespace std;

Camera::Camera() {
	//	VEC position;
	//	MAT orientation;

	pos = VEC(0, 0, 1);
    rotationVector = VEC(0,0,0);
    scrHeight = scrWidth = timeWhenChange = timeUntilChanged = theta = thetaFull = 0;

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


    glOrtho(xMin, xMax, yMin, yMax, -1.0, 1.);

    setPositionAndOrientationFromProjectionMatrix();
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
	td = sqrt(ori.yx*ori.yx + ori.yy*ori.yy + ori.yz*ori.yz);
	ori.yx /= td;
	ori.yy /= td;
	ori.yz /= td;


	ori.zx = matProjection[2];
	ori.zy = matProjection[6];
	ori.zz = matProjection[10];
	td = -sqrt(ori.zx*ori.zx + ori.zy*ori.zy + ori.zz*ori.zz);
	ori.zx /= td;
	ori.zy /= td;
	ori.zz /= td;


	cout << endl << endl << "\tmatProjection" << endl;
	for (int w=0; w<4; w++)
	{
		for (int v=0; v<4; v++)
			cout << matProjection[4*w + v] << ", ";
		cout << endl;
	}

	cout << "Ori: " << endl << ori << endl;

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

MAT Camera::getOrientation() const
{
	return ori;
}

VEC Camera::getPosition() const
{
	return pos;
}


void Camera::setPosition(VEC newPos) {}

void Camera::rotateWC(VEC rot, double angle) {}
void Camera::rotateSC(VEC rot, double angle) {}
void Camera::translateWC(VEC tra) {}
void Camera::translateSC(VEC tra) {}


void Camera::setPositionAndOrientation(const VEC &newPos, const MAT &newOri, int msTimeLapse)
{
		// R A = A'
		// R = A' A^T
	if (msTimeLapse <= 0)
		return;

	timeWhenChange = glutGet(GLUT_ELAPSED_TIME);
	timeUntilChanged = timeWhenChange + msTimeLapse;
	MAT R = newOri/ori;


	double cn = (R.xx + R.yy + R.zz - 1) / 2;
	thetaFull = acos(cn);
	double sn = sin(thetaFull);
	rotationVector = VEC(R.yz - R.zy, R.zx - R.xz, R.xy - R.yx) / (2*sn);
	oldOri = ori;

	translationVector = newPos - pos;
	oldPos = pos;
	cout << "cos(theta): " << cn << endl;
	cout << "sin(theta): " << sn << endl;
	cout << "RotationsVector: " << rotationVector << endl;
	cout << "thetaFull: " << thetaFull << endl;

    theta = 0.;
}


void Camera::updateCamera()
{
	int timeNow = glutGet(GLUT_ELAPSED_TIME);

		// inga förändringa görs så avsluta
	if (timeUntilChanged == 0)
		return;


	if (timeNow > timeUntilChanged) {
		theta = thetaFull;
		timeUntilChanged = 0;
		cout << (theta / thetaFull) << endl;
	} else
		theta = thetaFull * (timeNow - timeWhenChange) / (timeUntilChanged - timeWhenChange);


	pos = oldPos + translationVector * (theta/thetaFull);
	ori = createRotationalMatrix(rotationVector, cos(theta), sin(theta)) * oldOri;


	{
		cout << "theta: " << theta << "/" << thetaFull << endl;
		cout << "ori: " << ori << endl;
		cout << "pos: " << pos << endl;
		cout << endl;
		double p = 1;
		cout << pos.x << "," << pos.y << "," << pos.z << endl;
		cout << pos.x - ori.zx*p << "," << pos.y - ori.zy*p << "," << pos.z - ori.zz*p << endl;
		cout << ori.yx << ", " << ori.yy << ", " << ori.yz << endl;
		cout << endl;
	}

	updateOpenGLCamera();
}

MAT Camera::createRotationalMatrix(VEC v, double c, double s)
{
	double c1 = 1 - c;
	return MAT(	c + v.x*v.x*c1,			c1*v.x*v.y + s*v.z,			c1*v.x*v.z - s*v.y,
				c1*v.x*v.y - s*v.z,		c + v.y*v.y*c1,				c1*v.y*v.z + s*v.x,
				c1*v.x*v.z + s*v.y,		c1*v.y*v.z - s*v.x, 		c + c1*v.z*v.z);
}

void Camera::updateOpenGLCamera()
{
	glLoadIdentity();
	double p = 2;
    gluLookAt(	pos.x, pos.y, pos.z,	// position
    			pos.x - ori.zx*p, pos.y - ori.zy*p, pos.z - ori.zz*p,	// en punkt jag tittar på
				ori.yx, ori.yy, ori.yz);	// uppåt på skärmen

    glOrtho(xMin, xMax, yMin, yMax, -1.0, 1.);
}
