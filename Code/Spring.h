#ifndef _H_SPRING_
#define _H_SPRING_

#include "Particle.h"


// F = -kx-bv
// k - spring "tightness" constant [negative to 0] rigid at 0
// x - displacement of spring from equalibrium
// b - constant (coefficient) dampening
// v - realtive velocity of points of spring

class Spring {
public:
	Particle* p1;
	Particle* p2;

	float k; // [-x to 0] higher = stiff sprint, lower = loose spring
	float restingLength;
	float b;
public:
	Spring(float _k, float _b, float len) {
		k = _k;
		b = _b;
		restingLength = len;
	}

	inline void SetParticles(Particle* _p1, Particle* _p2) {
		p1 = _p1;
		p2 = _p2;
	}

	inline Particle* GetP1() {
		return p1;
	}

	inline Particle* GetP2() {
		return p2;
	}

	inline void ApplyForce(float dt){
		vec3 relPos = p2->GetPosition() - p1->GetPosition();
		vec3 relVel = p2->GetVelocity() - p1->GetVelocity();

		float x = Magnitude(relPos) - restingLength;
		float v = Magnitude(relVel);

		float f_k = -k * x;
		float f_v = -b * v;

		vec3 impulse = Normalized(relPos) * (f_v + f_k);

		p1->AddImpulse(impulse * p1->InvMass());
		p2->AddImpulse(impulse*  -1.0f * p2->InvMass());
	}
};

#endif