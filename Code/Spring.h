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
	Spring(float _k, float _b, float len);
	void SetParticles(Particle* _p1, Particle* _p2);
	Particle* GetP1();
	Particle* GetP2();
	void SetConstants(float _k, float _b);
	void ApplyForce(float dt);
};

#endif