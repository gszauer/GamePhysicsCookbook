#ifndef _H_RIGIDBODY_
#define _H_RIGIDBODY_

#include <vector>
#include "Geometry3D.h"

#define RIGIDBODY_TYPE_BASE		0
#define RIGIDBODY_TYPE_PARTICLE	1
#define RIGIDBODY_TYPE_SPHERE	2
#define RIGIDBODY_TYPE_BOX		3

class Rigidbody {
public:
	int type;
public:
	inline Rigidbody() {
		type = RIGIDBODY_TYPE_BASE;
	}
	virtual inline ~Rigidbody() { }

	virtual inline void Update(float deltaTime) { }
	virtual inline void Render() { }
	virtual inline void ApplyForces() { }
	virtual inline void SolveConstraints(const std::vector<OBB>& constraints) { }

	inline bool HasVolume() {
		return type == RIGIDBODY_TYPE_SPHERE || type == RIGIDBODY_TYPE_BOX;
	}
};

#endif