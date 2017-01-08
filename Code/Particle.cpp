#include "Particle.h"
#include "Geometry3D.h"
#include "FixedFunctionPrimitives.h"

Particle::Particle() {
	type = RIGIDBODY_TYPE_PARTICLE;
	friction = 0.95f;
	bounce = 0.7f;
	gravity = vec3(0.0f, -9.82f, 0.0f);

#ifdef EULER_INTEGRATION
	mass = 1.0f;
#endif
}

void Particle::Update(float deltaTime) {
#ifdef EULER_INTEGRATION
	oldPosition = position;
	vec3 acceleration = forces *InvMass();
	#ifdef ACCURATE_EULER_INTEGRATION
		vec3 oldVelocity = velocity;
		velocity = velocity * friction + acceleration * deltaTime;
		position = position + (oldVelocity + velocity) * 0.5f * deltaTime;
	#else
		velocity = velocity * friction + acceleration * deltaTime;
		position = position + velocity * deltaTime;
	#endif
#else
	vec3 velocity = position - oldPosition;
	oldPosition = position;
	float deltaSquare = deltaTime * deltaTime;
	position = position + (velocity * friction + forces * deltaSquare);
#endif
}

void Particle::Render() {
	Sphere visual(position, 0.1f);
	::Render(visual);
}

void Particle::ApplyForces() {
#ifdef EULER_INTEGRATION
	forces = gravity *mass;
#else
	forces = gravity *mass;
#endif
}

void Particle::SolveConstraints(const std::vector<OBB>& constraints) {
	int size = constraints.size();
	for (int i = 0; i < size; ++i) {
		Line traveled(oldPosition, position);
		if (Linetest(constraints[i], traveled)) {
			//if (PointInOBB(position, constraints[i])) {
#ifndef EULER_INTEGRATION
			vec3 velocity = position - oldPosition;
#endif
			vec3 direction = Normalized(velocity);
			Ray ray(oldPosition, direction);
			RaycastResult result;

			if (Raycast(constraints[i], ray, &result)) {
				// Place object just a little above collision result
				position = result.point + result.normal * 0.003f;

				vec3 vn = result.normal * Dot(result.normal, velocity);
				vec3 vt = velocity - vn;

#ifdef EULER_INTEGRATION
				oldPosition = position;
				velocity = vt - vn * bounce;
#else
				oldPosition = position - (vt - vn * bounce);
#endif
				break;
			}
		}
	}
}

void Particle::SetPosition(const vec3& pos) {
	position = pos;
	oldPosition = pos;
}

vec3 Particle::GetPosition() {
	return position;
}

void Particle::SetBounce(float b) {
	bounce = b;
}

float Particle::GetBounce() {
	return bounce;
}

void Particle::AddImpulse(const vec3& impulse) {
#ifdef EULER_INTEGRATION
	velocity = velocity + impulse;
#else
	vec3 velocity = position - oldPosition;
	velocity = velocity + impulse;
	oldPosition = position - velocity;
#endif
}

float Particle::InvMass() {
	if (mass == 0.0f) { return 0.0f; }
	return 1.0f / mass;
}

void Particle::SetMass(float m) {
	mass = m;
}

vec3 Particle::GetVelocity() {
#ifdef EULER_INTEGRATION
	return velocity;
#else
	return position - oldPosition;
#endif
}

void Particle::SetFriction(float f) {
	friction = f;
}