#ifndef _H_PARTICLE_
#define _H_PARTICLE_

#include "Rigidbody.h";

class Particle : public Rigidbody {
	vec3 oldPosition;
	vec3 position;
	vec3 velocity;
	vec3 forces;
	float bounce;
	float friction;
	vec3 gravity;
	float mass;
public:
	Particle();

	void Update(float deltaTime);
	virtual void Render();
	void ApplyForces();
	void SolveConstraints(const std::vector<OBB>& constraints);

	void SetBounce(float b);
	void SetFriction(float f);
	void SetPosition(const vec3& pos);

	vec3 GetPosition();
	float GetBounce();
	float GetFriction();
};

#endif
