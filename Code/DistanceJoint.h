#ifndef _H_DISTANCE_JOINT
#define _H_DISTANCE_JOINT

#include "Particle.h"

class DistanceJoint : public Rigidbody {
protected:
	Particle* p1;
	Particle* p2;
	float length;
public:
	void Initialize(Particle* _p1, Particle* _p2, float len);
	void SolveConstraints(const std::vector<OBB>& constraints);
	void Render();
};

#endif
