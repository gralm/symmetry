#include "orientation.h"

namespace Graph2D {

	point::point() 				
	{
		x = y = 0;
	}
	point::point(double x_, double y_) 	
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


	bool point::defined() const
	{
		return (x+100.)*(x+100.) + (y+100.)*(y+100.) > .001;
	}

	point point::operator+ (point A)	{	return point(A.x+x, A.y+y);}
	void point::operator+= (point A)	{	x += A.x;	y += A.y;}

	point point::operator- (point A)	{	return point(x-A.x, y-A.y);}
	void point::operator-= (point A)	{	x -= A.x;	y -= A.y;}

	double point::operator* (point A)	{	return A.x*x + A.y*y;}
	void point::operator*= (double a)	{	x *= a;		y *= a;}

	point point::operator* (double a)	{	return point(x*a, y*a);}
	point point::operator~ () 			{	return point(-y, x);}

}