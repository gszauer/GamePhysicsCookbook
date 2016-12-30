#ifndef _H_RIGIDBODY_
#define _H_RIGIDBODY_

#include <vector>
#include <string>
#include "Geometry3D.h"
#include "Compare.h"

//#define LINEAR_ONLY
//#define DYNAMIC_FRICTION

#define RIGIDBODY_TYPE_BASE		0
#define RIGIDBODY_TYPE_PARTICLE	1
#define RIGIDBODY_TYPE_SPHERE	2
#define RIGIDBODY_TYPE_BOX		3

#define GRAVITY_CONST vec3(0.0f, -9.82f, 0.0f)

class Rigidbody {
public:
	vec3 position;
	vec3 velocity;

#ifndef LINEAR_ONLY
	vec3 orientation;
	vec3 angVel;
#endif

	vec3 forces; // sumForces
#ifndef LINEAR_ONLY
	vec3 torques; // Sum torques
#endif

	//vec3 inertia;
	float mass;
	float cor; // Coefficient of restitution
#ifdef DYNAMIC_FRICTION
	float staticFriction;
	float dynamicFriction;
#else
	float friction;
#endif

	int type;
	OBB box;
	Sphere sphere;
public:

	inline Rigidbody() :
		cor(0.5f), mass(1.0f),
#ifdef DYNAMIC_FRICTION
		staticFriction(0.5f),
		dynamicFriction(0.3f),
#else
		friction(0.6f),
#endif
		type(RIGIDBODY_TYPE_BASE) {
	}

	inline Rigidbody(int bodyType) :
		cor(0.5f), mass(1.0f),
#ifdef DYNAMIC_FRICTION
		staticFriction(0.5f),
		dynamicFriction(0.3f),
#else
		friction(0.6f),
#endif
		type(bodyType) {
	}

	virtual ~Rigidbody() { }

	virtual void Render();
	virtual void UpdateVelocity(float dt);
	virtual void Update(float dt); // Update Position

	float InvMass();
#ifndef LINEAR_ONLY
	mat4 InvTensor();
#endif

	virtual void ApplyForces();
	void SynchCollisionVolumes();

	virtual void AddLinearImpulse(const vec3& impulse);
#ifndef LINEAR_ONLY
	virtual void AddRotationalImpulse(const vec3& point, const vec3& impulse);
#endif
	inline virtual void SolveConstraints(const std::vector<OBB>& constraints) { }
};

CollisionManifold FindCollisionFeatures(Rigidbody& ra, Rigidbody& rb);
void ApplyImpulse(Rigidbody& A, Rigidbody& B, const CollisionManifold& M, int c);

#endif