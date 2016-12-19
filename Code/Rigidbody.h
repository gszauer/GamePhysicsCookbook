#ifndef _H_RIGIDBODY_
#define _H_RIGIDBODY_

#include <vector>
#include "Geometry3D.h"
#include <iostream>

#define RIGIDBODY_USE_FRICTION

#define RIGIDBODY_TYPE_BASE		0
#define RIGIDBODY_TYPE_PARTICLE	1
#define RIGIDBODY_TYPE_SPHERE	2
#define RIGIDBODY_TYPE_BOX		3

#define GRAVITY_CONST vec3(0.0f, -9.82f, 0.0f)

class Rigidbody {
public:
	int type;

	vec3 position;
	vec3 velocity;

	float mass;
	vec3 forces;

	float cor; // Coefficient of restitution
public:
	Rigidbody() : type(RIGIDBODY_TYPE_BASE), cor(0.5f) { }
	virtual ~Rigidbody() { }

	inline virtual void Render() { }
	inline virtual void SolveConstraints(const std::vector<OBB>& constraints) { }

	inline virtual void Update(float dt) {
		float invMass = (mass == 0.0f) ? 0.0f : 1.0f / mass;
		vec3 acceleration = forces * invMass;
		velocity = velocity + acceleration * dt;
		position = position + velocity * dt;
	}

	inline virtual void SetPosition(const vec3& pos) {
		position = pos;
	}

	inline virtual void ApplyForces() {
		forces = GRAVITY_CONST;
	}

	inline virtual int GetType() {
		return type;
	}

	inline virtual CollisionManifest IsColliding(Rigidbody& rb) { 
		CollisionManifest result;
		ResetCollisionManifest(&result);
		return result;
	}

	inline virtual void AddLinearImpulse(const vec3& impulse) {
		velocity = velocity + impulse;
	}

	inline virtual float LinearImpulse(Rigidbody& other, const CollisionManifest& features) {
		float invMass1 = (mass == 0.0f)? 0.0f : 1.0f / mass;
		float invMass2 = (other.mass == 0.0f)? 0.0f : 1.0f / other.mass;

		// Relative velocity
		vec3 vel = velocity - other.velocity;
		// Relative collision normal
		vec3 norm = features.normal;

		// Moving away from each other? Do nothing!
		if (Dot(vel, norm) > 0.0f) {
			return 0.0f;
		}

		//vec3 tan = vel - norm *  Dot(vel, norm);
		//Normalize(tan);
		//std::cout << "normal: " << norm.x << ", " << norm.y << ", " << norm.z << "\n";

		float e = fminf(cor, other.cor);

/*#ifdef  RIGIDBODY_USE_FRICTION
		float numerator = (-(1.0f + e) * Dot(vel, tan));
#else */
		float numerator = (-(1.0f + e) * Dot(vel, norm));
//#endif
		float denominator = (invMass1 + invMass2);
		float j = numerator / denominator;

		vec3 impulse = norm * j;

		velocity = velocity + impulse *  invMass1;
		other.velocity = other.velocity - impulse *  invMass2;

		return j;
	}
};

#endif
