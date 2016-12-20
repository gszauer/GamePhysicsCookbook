#ifndef _H_PHYSICS_SYSTEM_
#define _H_PHYSICS_SYSTEM_

#include "Rigidbody.h"

class PhysicsSystem {
protected:
	std::vector<Rigidbody*> bodies;
	std::vector<OBB> constraints;

	std::vector<Rigidbody*> colliders1;
	std::vector<Rigidbody*> colliders2;
	std::vector<CollisionManifold> results;
public:
	float LinearProjectionPercent; // [0.2 to 0.8], Smaller = less jitter / more penetration
	float PenetrationSlack; // [0.01 to 0.1],  Samller = more accurate
	int ImpulseIteration;

	bool RenderRandomColors;

	PhysicsSystem();

	void Update(float deltaTime);
	void Render();
	
	void AddRigidbody(Rigidbody* body);
	void AddConstraint(const OBB& constraint);

	void ClearRigidbodys();
	void ClearConstraints();
};

#endif