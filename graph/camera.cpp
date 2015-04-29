#include "graph2d.h"

using namespace std;

Camera::Camera() {
	//	VEC position;
	//	MAT orientation;

	near_ = 0.5;
	far_ = 3.0;

	fovy = 112.;
	tfovy = tan(fovy*.5 * (M_PI/180.));
	aspect = 1.0;

	pos = VEC(0, 0, 1);
    rotationVector = VEC(0,0,0);
    scrHeight = scrWidth = timeWhenChange = timeUntilChanged = theta = thetaFull = 0;

    /* select clearing (background) color */
    glClearColor (0.0, 0.0, 0.0, 0.0);

    /* initialize viewing values */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluLookAt(	0, 0, 2,	// position
    			0, 0, -1,	// en punkt jag tittar på
				0, 1, 0);	// uppåt på skärmen

    ori = MAT(1, 0, 0, 0, 1, 0, 0, 0, 1);

    gluPerspective(fovy, aspect, near_, far_);
}

VEC Camera::fromABtoXY(int x, int y)
{
	//double tfovy = tan(fovy*.5 * (M_PI/180.));
	VEC dir = ori.row(0)*aspect*tfovy*(2.*x/scrWidth - 1.) + ori.row(1)*tfovy*(1. - 2.*y/scrHeight) - ori.row(2);

	if (dir.z == 0)
		return VEC(0, 0, 0);

	double k = -pos.z/dir.z;
	VEC returnVec = pos + dir*k;

	return returnVec;
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

	cout << "rotationsmatrisen som används i setoraosdfgåasdogihjöoihj = " << endl;
	cout << R << endl;
	cout << "R determinant: " << R.determinant() << endl;

	cout << "newOri" << endl;
	cout << newOri << endl;
	cout << "newOri determinant: " << newOri.determinant() << endl;

	cout << "ori" << endl;
	cout << ori << endl;
	cout << "ori determinant: " << ori.determinant() << endl;

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
	double c = cos(theta);
	double s = sin(theta);
	MAT toRota = createRotationalMatrix(rotationVector, c, s);
	ori = toRota * oldOri;

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
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double p = 2;

	gluPerspective(fovy, aspect, near_, far_);
    gluLookAt(	pos.x, pos.y, pos.z,	// position
    			pos.x - ori.zx*p, pos.y - ori.zy*p, pos.z - ori.zz*p,	// en punkt jag tittar på
				ori.yx, ori.yy, ori.yz);	// uppåt på skärmen
}
