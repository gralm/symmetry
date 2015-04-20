#ifndef PUBLIC_DEFINES_H
#define PUBLIC_DEFINES_H

	// standard för alla processer
#define UPD_GRAPH       25
#define UPD_PHYS        100
#define UPD_PRINT       1000
#define UPD_THREAD      100


	// Grafik
#define GLUT_WINDOW_POSITION_X			600
#define GLUT_WINDOW_POSITION_Y			100
#define GLUT_WINDOW_WIDTH			600
#define GLUT_WINDOW_HEIGHT			600
#define WIN32_WINDOW_POSITION_X
#define WIN32_WINDOW_POSITION_Y

	// Matematik
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
#define MAT			tmath::matrix<double,3,3>

#define TYP		unsigned char
#define INV_ROTATION(r)		(r^1)

#define VP		((TYP) 2)	// 010
#define VN		((TYP) 3)	// 011
#define FP		((TYP) 4)	// 100
#define FN		((TYP) 5)	// 101

//enum Centered {NotCentered=-100, VertexCentered=-2, EdgeCentered=-3, FaceCentered=-4, UndefinedCentered=-1};

#define NOT_CENTERED		(-1)
#define VERTEX_CENTERED		(-2)
#define EDGE_CENTERED		(-3)
#define FACE_CENTERED		(-4)



#define UNDEFINED_VEC		VEC(-100., -100.)
//#define IS_DEFINED(X)		(((X)-UNDEFINED_VEC) * ((X)-UNDEFINED_VEC) < 0.001)
#define IS_DEFINED(X)		(((X)-UNDEFINED_VEC) * ((X)-UNDEFINED_VEC) > 0.001)
#define IS_DEFINED2(X)		(((X)*(X) - 200*((X).x + (X).y) + 20000) > 0.001)






			/* COMMANDON FÖR TRÅDKOMMUNIKATION */
	// för thread-id
#define COMM_THREAD_MAIN		3
#define COMM_THREAD_GLUT		4

	// Message typ
#define COMM_MSGTYP_EXIT					0
#define COMM_MSGTYP_PAUSE					1
#define COMM_MSGTYP_CHOOSE_VERTEX			2
#define COMM_MSGTYP_ADD_VERTEX				3

#define COMM_MSGTYP_ADD_CENTERED_VERTEX		4
#define COMM_MSGTYP_SET_MODE				5
#define COMM_MSGTYP_SET_SYMMETRY_VALUE		9

#define COMM_MSGTYP_UPDATE_VERTEX			20
#define COMM_MSGTYP_UPDATE_EDGE				21
#define COMM_MSGTYP_UPDATE_FACE				22

	// return values messages (msg)
#define COMM_RET_ID_CREATED			11
#define COMM_RET_ID_OVERWRITTEN		12
#define COMM_RET_ID_MISSING			13
#define COMM_RET_ERROR				14
#define COMM_RET_ID_OK				15



/*
 * update vertex:	"COMM_MSGTYP_UPDATE_VERTEX, index, x, y, z"
 * välj vertex: 	"COMM_MSGTYP_CHOOSE_VERTEX, index"
 *
 * */


#endif
