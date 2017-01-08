#ifndef _H_PHYSICS_SYSTEM_
#define _H_PHYSICS_SYSTEM_

#include "Rigidbody.h"
#include "Spring.h"
#include "Cloth.h"

class PhysicsSystem {
protected:
	std::vector<Rigidbody*> bodies;
	std::vector<Cloth*> cloths;
	std::vector<OBB> constraints;
	std::vector<Spring> springs;

	std::vector<Rigidbody*> colliders1;
	std::vector<Rigidbody*> colliders2;
	std::vector<CollisionManifold> results;
public:
	float LinearProjectionPercent; // [0.2 to 0.8], Smaller = less jitter / more penetration
	float PenetrationSlack; // [0.01 to 0.1],  Samller = more accurate
	int ImpulseIteration;

	// Not in book, just for debug purposes
	bool DebugRender;
	bool DoLinearProjection;
	bool RenderRandomColors;

	PhysicsSystem();

	void Update(float deltaTime);
	void Render();
	
	void AddRigidbody(Rigidbody* body);
	void AddCloth(Cloth* cloth);
	void AddSpring(const Spring& spring);
	void AddConstraint(const OBB& constraint);

	void ClearRigidbodys();
	void ClearConstraints();
	void ClearSprings();
	void ClearCloths();
};

#endif