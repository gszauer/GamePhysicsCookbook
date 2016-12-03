#ifndef _H_PHYSICS_SYSTEM_
#define _H_PHYSICS_SYSTEM_

#include "Rigidbody.h"

class PhysicsSystem {
protected:
	std::vector<Rigidbody*> bodies;
	std::vector<OBB> constraints;
public:
	void Update(float deltaTime);
	void Render();
	
	void AddRigidbody(Rigidbody* body);
	void AddConstraint(const OBB& constraint);

	void ClearRigidbodys();
	void ClearConstraints();
};

#endif