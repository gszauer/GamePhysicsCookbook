#ifndef _H_PARTICLE_
#define _H_PARTICLE_

#include "Rigidbody.h";

// The particles will either use euler or verlet integration
// If EULER_INTEGRATION is defined, euler integration will
// be used. Otherwise, verlet integration is the default

// If ACCURATE_EULER_INTEGRATION is defined, a slightly more
// accurate integration model is used. This should help 
// keep the simulation stable over long periods of time

//#define EULER_INTEGRATION
#define ACCURATE_EULER_INTEGRATION

class Particle : public Rigidbody {
	vec3 oldPosition;

	float friction;
	float bounce;
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
};

#endif
