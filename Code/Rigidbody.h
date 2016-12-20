#ifndef _H_RIGIDBODY_
#define _H_RIGIDBODY_

#include <vector>
#include "Geometry3D.h"

#define RIGIDBODY_TYPE_BASE		0
#define RIGIDBODY_TYPE_PARTICLE	1
#define RIGIDBODY_TYPE_SPHERE	2
#define RIGIDBODY_TYPE_BOX		3

#define GRAVITY_CONST vec3(0.0f, -9.82f, 0.0f)

class Rigidbody {
public:
	vec3 position;
	vec3 velocity;

	float mass;
	vec3 forces;

	float cor; // Coefficient of restitution
	float staticFriction;
	float dynamicFriction;
	
	int type;
	OBB box;
	Sphere sphere;
public:
	Rigidbody() :
		cor(0.5f), mass(1.0f),
		staticFriction(0.5f),
		dynamicFriction(0.3f),
		type(RIGIDBODY_TYPE_BASE) { }

	Rigidbody(int bodyType) :
		cor(0.5f), mass(1.0f),
		staticFriction(0.5f),
		dynamicFriction(0.3f),
		type(bodyType) { }

	virtual ~Rigidbody() { }

	virtual void Render();
	virtual void Update(float dt);
	
	virtual void ApplyForces();
	float InvMass();
	void SynchCollisionVolumes();

	virtual void AddLinearImpulse(const vec3& impulse);
	inline virtual void SolveConstraints(const std::vector<OBB>& constraints) { }
};

CollisionManifold FindCollisionFeatures(Rigidbody& ra, Rigidbody& rb);
void ApplyImpulse(Rigidbody& A, Rigidbody& B, const CollisionManifold& M);

#endif
