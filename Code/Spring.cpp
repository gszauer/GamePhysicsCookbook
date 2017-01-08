#include "Spring.h"

Spring::Spring(float _k, float _b, float len) {
	k = _k;
	b = _b;
	restingLength = len;
}

void Spring::SetParticles(Particle* _p1, Particle* _p2) {
	p1 = _p1;
	p2 = _p2;
}

Particle* Spring::GetP1() {
	return p1;
}

Particle* Spring::GetP2() {
	return p2;
}

void Spring::SetConstants(float _k, float _b) {
	k = _k;
	b = _b;
}

void Spring::ApplyForce(float dt) {
	vec3 relPos = p2->GetPosition() - p1->GetPosition();
	vec3 relVel = p2->GetVelocity() - p1->GetVelocity();

	// Prevent underflow
	for (int i = 0; i < 3; ++i) {
		relPos[i] = (fabsf(relPos[i]) < 0.0000001f) ? 0.0f : relPos[i];
		relVel[i] = (fabsf(relVel[i]) < 0.0000001f) ? 0.0f : relVel[i];
	}

	float x = Magnitude(relPos) - restingLength;
	float v = Magnitude(relVel);

	float F = -k * x - b * v;
	/*float f_k = -k * x;
	float f_b = b * v;
	f_b = (f_b > f_k)? f_k : f_b;
	float F = f_k - f_b;*/

	vec3 impulse = Normalized(relPos) * F;
	p1->AddImpulse(impulse * p1->InvMass());
	p2->AddImpulse(impulse*  -1.0f * p2->InvMass());
}