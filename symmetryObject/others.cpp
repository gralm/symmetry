#ifndef SYMMETRY_OBJECT_OTHERS_CPP
#define SYMMETRY_OBJECT_OTHERS_CPP

#include "objs.h"


using namespace std;

Edge::Edge()
{
	index = 0;
}

Edge::Edge(Prefix Pfx_, int index_)
{
	Pfx = Pfx_;
	index = index_;
}

void Edge::print() const
{
	cout << "index (";
	switch(index)
	{
		case -1:
			cout << "undef.";
			break;
		default:
			cout << index;
			break;
	}
	cout << "), ";
	Pfx.print();
}

string Edge::toString() const
{
	if (index == -1)
		return "undef.";

	stringstream ss;
	ss << index;
	ss << Pfx.toString();
	return ss.str();
}

bool Edge::isDefined() const
{
	return index != -1;
}

edge::edge()
{
	/*Point fr;
	Point to;

	Edge next;
	Edge prev;
	Edge oppo;*/
}

void edge::print() const
{
	cout << "\tPoint fr = ";
	fr.print();
	cout << endl << "\tPoint to = ";
	to.print();
	cout << endl;

	cout << "\tEdge next = ";
	next.print();
	cout << endl << "\tEdge prev = ";
	prev.print();
	cout << endl << "\tEdge oppo = ";
	oppo.print();
	cout << endl;
}

	// returns true if e is opposite edge. If pfx != null, pfx
bool edge::isOppositeOf(const edge &e, Prefix *pfx)
{
		// avsluta om inte ens punkterna stämmer
	if (fr.index != e.to.index || to.index != e.fr.index)
		return false;

	Prefix X;

		//
	cout << " ** drive isOppositeOf" << endl;
	cout << "this: " << endl;
	this->print();
	cout << endl << "edge e: " << endl;
	e.print();


	switch(fr.index)
	{
		case VERTEX_CENTERED: {
			cout << "a" << endl;

				// fr = X e.to
			X = fr.Pfx;
			X.rotate(e.to.Pfx.getInverse());
			Point e_fr(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

				// fr [VP] = X e.to
			X = fr.Pfx;
			X.rotate(VP);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

				// fr [VN] = X e.to
			X = fr.Pfx;
			X.rotate(VN);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

			cout << "a" << endl;
			break;
		}
		case EDGE_CENTERED: {
			cout << "b" << endl;

			X = fr.Pfx;
			X.rotate(e.to.Pfx.getInverse());
			Point e_fr(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

				// fr [VP] = X e.to
			X = fr.Pfx;
			X.rotate(VP);
			X.rotate(FP);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

			break;
		}
		case FACE_CENTERED: {
			cout << "c" << endl;

				// fr = X e.to
			X = fr.Pfx;
			X.rotate(e.to.Pfx.getInverse());
			Point e_fr(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

				// fr [FP] = X e.to
			X = fr.Pfx;
			X.rotate(FP);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

				// fr [FN] = X e.to
			X = fr.Pfx;
			X.rotate(FN);
			X.rotate(e.to.Pfx.getInverse());
			e_fr = Point(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}

			cout << "c" << endl;
			break;
		}
		default: {

			cout << "D" << endl;
			X = fr.Pfx;
			X.rotate(e.to.Pfx.getInverse());
			Point e_fr(X * e.fr.Pfx, e.fr.index);
			if (to.equalTo(e_fr))	{*pfx = X;	return true;}
			cout << "D" << endl;
			break;
		}
	}


	//pfx->R.clear();
	return false;
}


face::face()
{
	fr = -1;
	edges = 0;
	type = NOT_CENTERED;
}

face::face(int fr_, int edges_, int type_)
{
	fr = fr_;
	edges = edges_;
	type = type_;
}

void face::print()
{
	cout << "from: " << fr << "\t len: " << edges << "\t typ: ";
	switch(type)
	{
		case -1:
			cout << "Not-Centered";
			break;
		case VERTEX_CENTERED:
			cout << "Vertex-Centered";
			break;
		case EDGE_CENTERED:
			cout << "Edge-Centered";
			break;
		case FACE_CENTERED:
			cout << "Face-Centered";
			break;
		default:
			cout << "Felaktig";
			break;
	}
}



#endif

