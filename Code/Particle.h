#ifndef _H_PARTICLE_
#define _H_PARTICLE_

#include "Rigidbody.h";

// The particles will either use euler or verlet integration
// If EULER_INTEGRATION is defined, euler integration will
// be used. Otherwise, verlet integration is the default

// If ACCURATE_EULER_INTEGRATION is defined, a slightly more
// accurate integration model is used. This should help 
// keep the simulation stable over long periods of time

#define EULER_INTEGRATION
//#define ACCURATE_EULER_INTEGRATION

class Particle : public Rigidbody {
	vec3 position;
	vec3 oldPosition;
	vec3 forces;

	vec3 gravity;
	float friction;
	float bounce;

#ifdef EULER_INTEGRATION
	vec3 velocity;
#endif
	float mass;
public:
	Particle();

	void Update(float deltaTime);
	virtual void Render();
	void ApplyForces();
	void SolveConstraints(const std::vector<OBB>& constraints);

	void SetPosition(const vec3& pos);
	vec3 GetPosition();

	void SetBounce(float b);
	float GetBounce();

	inline void AddImpulse(const vec3& impulse) {
#ifdef EULER_INTEGRATION
		velocity = velocity + impulse;
#else
		vec3 velocity = position - oldPosition;
		velocity = velocity + impulse;
		oldPosition = position - velocity;
#endif
	}

	inline float InvMass() {
		if (mass == 0.0f) { return 0.0f; }
		return 1.0f / mass;
	}

	inline void SetMass(float m) {
		mass = m;
	}

	inline vec3 GetVelocity() {
#ifdef EULER_INTEGRATION
		return velocity;
#else
		return position - oldPosition;
#endif
	}
};

#endif
