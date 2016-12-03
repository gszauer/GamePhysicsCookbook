#ifndef _H_RIGIDBODY_
#define _H_RIGIDBODY_

#include <vector>
#include "Geometry3D.h"

class Rigidbody {
public:
	Rigidbody() { }
	virtual ~Rigidbody() { }

	virtual void Update(float deltaTime) { }
	virtual void Render() { }
	virtual void ApplyForces() { }
	virtual void SolveConstraints(const std::vector<OBB>& constraints) { }
};

#endif
