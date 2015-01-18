#ifndef PREFIX_H
#define PREFIX_H

#include <list>

#define TYP		unsigned char
#define INV_ROTATION(r)		(r^1)

#define VP		2	// 010
#define VN		3	// 110
#define FP		4	// 001
#define FN		5	// 101


		// omvandla alla:	summa = 1023
	// VP, VN = 1 			0001 	0
	// VP, FP, VP = FN		0002 	1
	// VN, VP = 1 			0004 	2
	// VN, FN, VN = FP 		0008 	3
	// FP, VP, FP = VN		0010 	4
	// FP, FP = FN			0020 	5
	// FP, FN = 1 			0040 	6
	// FN, VN, FN = VP		0080  	7
	// FN, FN = FP			0100 	8
	// FN, FP = 1 			0200 	9


namespace Graph2D {

	class Prefix {
	public:
		std::list<TYP> R;
		
		Prefix();

		int findInList();
		void simplify();
		void print();
		bool rotate(TYP r);
		int getSize();

		Prefix difference(Prefix A);
	};
}

#endif