#ifndef _H_RIGIDBODY_
#define _H_RIGIDBODY_

#include <vector>
#include <string>
#include "Geometry3D.h"
#include "Compare.h"

#define RIGIDBODY_TYPE_BASE		0
#define RIGIDBODY_TYPE_PARTICLE	1
#define RIGIDBODY_TYPE_SPHERE	2
#define RIGIDBODY_TYPE_BOX		3

#define GRAVITY_CONST vec3(0.0f, -9.82f, 0.0f)

class Rigidbody {
public:
	vec3 position;
	vec3 velocity;
	// linear acceleration = F / M

	vec3 forces; // sumForces
	vec3 torques; // Sum torques

	vec3 orientation;
	vec3 angVel;

	//vec3 inertia;
	std::string debug;

	float mass;
	float cor; // Coefficient of restitution
	float staticFriction;
	float dynamicFriction;

	int type;
	OBB box;
	Sphere sphere;
public:
	inline vec3 InvTensor() {
		if (mass == 0.0f) {
			return vec3();
		}

		float width = box.size.x;
		float height = box.size.y;
		float depth = box.size.z;

		float xTensor = 0.083f * mass*(height*height + depth*depth);
		float yTensor = 0.083f * mass*(width*width + depth*depth);
		float zTensor = 0.083f * mass*(width*width + height*height);

		xTensor = CMP(xTensor, 0.0f) ? 0.0f : 1.0f / xTensor;
		yTensor = CMP(yTensor, 0.0f) ? 0.0f : 1.0f / yTensor;
		zTensor = CMP(zTensor, 0.0f) ? 0.0f : 1.0f / zTensor;

		return vec3(xTensor, yTensor, zTensor);
	}

	inline Rigidbody() :
		cor(0.5f), mass(1.0f),
		staticFriction(0.5f),
		dynamicFriction(0.3f),
		type(RIGIDBODY_TYPE_BASE) {
		angVel = vec3();
		debug = "none";
	}

	inline Rigidbody(int bodyType) :
		cor(0.5f), mass(1.0f),
		staticFriction(0.5f),
		dynamicFriction(0.3f),
		type(bodyType) {
		angVel = vec3();
		debug = "none";
	}

	virtual ~Rigidbody() { }

	virtual void Render();
	virtual void Update(float dt);

	virtual void ApplyForces();
	float InvMass();
	void SynchCollisionVolumes();

	virtual void AddLinearImpulse(const vec3& impulse);
	virtual void AddRotationalImpulse(const vec3& point, const vec3& impulse);
	inline virtual void SolveConstraints(const std::vector<OBB>& constraints) { }
};

CollisionManifold FindCollisionFeatures(Rigidbody& ra, Rigidbody& rb);
void ApplyImpulse(Rigidbody& A, Rigidbody& B, const CollisionManifold& M, int c);

#endif