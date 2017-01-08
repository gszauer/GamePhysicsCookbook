#ifndef _H_CLOTH_
#define _H_CLOTH_

#include "Particle.h"
#include "Spring.h"
#include <vector>

class Cloth {
protected:
	std::vector<Particle> verts;
	std::vector<Spring> structural;
	std::vector<Spring> shear;
	std::vector<Spring> bend;
	float clothSize;
public:
	// Public API
	void Initialize(int gridSize, float distance, const vec3& position);

	void SetStructuralSprings(float k, float b);
	void SetShearSprings(float k, float b);
	void SetBendSprings(float k, float b);
	void SetParticleMass(float mass);

	// For Physics System
	void ApplyForces();
	void Update(float dt);
	void SolveConstraints(const std::vector<OBB>& constraints);
	void ApplySpringForces(float dt);
	void Render(bool debug);
};

#endif