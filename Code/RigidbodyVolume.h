#ifndef _H_MASS_RIGIDBODY_
#define _H_MASS_RIGIDBODY_

#include "Rigidbody.h"

#define GRAVITY_CONST vec3(0.0f, -9.82f, 0.0f)

class RigidbodyVolume : public Rigidbody {
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

	OBB box;
	Sphere sphere;
public:

	inline RigidbodyVolume() :
		cor(0.5f), mass(1.0f),
#ifdef DYNAMIC_FRICTION
		staticFriction(0.5f),
		dynamicFriction(0.3f)
#else
		friction(0.6f)
#endif
		{
		type = RIGIDBODY_TYPE_BASE;
	}

	inline RigidbodyVolume(int bodyType) :
		cor(0.5f), mass(1.0f),
#ifdef DYNAMIC_FRICTION
		staticFriction(0.5f),
		dynamicFriction(0.3f)
#else
		friction(0.6f)
#endif
		{
			type = bodyType;
	}

	virtual ~RigidbodyVolume() { }

	virtual void Render();
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
};

CollisionManifold FindCollisionFeatures(RigidbodyVolume& ra, RigidbodyVolume& rb);
void ApplyImpulse(RigidbodyVolume& A, RigidbodyVolume& B, const CollisionManifold& M, int c);

#endif