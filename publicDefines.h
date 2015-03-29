#ifndef PUBLIC_DEFINES_H
#define PUBLIC_DEFINES_H

#define COS30			0.86602540378444
#define SIN30			0.5
#define COS60			0.5
#define SIN60			0.86602540378444
#define COS120			(-0.5)
#define SIN120			0.86602540378444
#define TAN30			0.57735026918962
#define TAN60			1.73205080756887


#define VEC0(S)   	(tmath::vectorn<S, 3>(0,0,0))
#define VEC 		tmath::vectorn<double, 3>

#define TYP		unsigned char
#define INV_ROTATION(r)		(r^1)

#define VP		((TYP)2)	// 010
#define VN		((TYP)3)	// 011
#define FP		((TYP)4)	// 100
#define FN		((TYP)5)	// 101

//enum Centered {NotCentered=-100, VertexCentered=-2, EdgeCentered=-3, FaceCentered=-4, UndefinedCentered=-1};

#define NOT_CENTERED		(-1)
#define VERTEX_CENTERED		(-2)
#define EDGE_CENTERED		(-3)
#define FACE_CENTERED		(-4)

#define UNDEFINED_VEC		VEC(-100., -100.)
//#define IS_DEFINED(X)		(((X)-UNDEFINED_VEC) * ((X)-UNDEFINED_VEC) < 0.001)
#define IS_DEFINED(X)		(((X)-UNDEFINED_VEC) * ((X)-UNDEFINED_VEC) > 0.001)
#define IS_DEFINED2(X)		(((X)*(X) - 200*((X).x + (X).y) + 20000) > 0.001)

#endif
