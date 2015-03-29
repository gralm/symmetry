#ifndef SYMMETRY_RELAXATION_H
#define SYMMETRY_RELAXATION_H

#include "../objs.h"
#include <vector>
#include "../../publicDefines.h"


class SymmetryRelaxation: public SymmetryObject {
private:
	double dt;


	bool facePointActive;
	bool edgePointActive;
	bool vertexPointActive;

	std::vector<VEC> verticeVelocity;
	std::vector<VEC> verticeForce;
	std::vector<VEC> verticeDistance;
	std::vector<VEC> verticeDistanceStrength;

	std::vector<VEC> edgeStrength;
	std::vector<VEC> edgeDamping;
	std::vector<VEC> edgeLength;
	//std::vector<edge> dE;
	//std::vector<face> F;

public:
	SymmetryRelaxation(const SymmetryObject* oldObject);//:SymmetryObject(oldObject);
	void setTimeStep(double newDt);
	//void copySettingsTo(SymmetryObject *toBeChanged);
};

#endif