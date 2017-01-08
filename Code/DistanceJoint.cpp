#include "DistanceJoint.h"
#include "FixedFunctionPrimitives.h"

void DistanceJoint::Initialize(Particle* _p1, Particle* _p2, float len) {
	p1 = _p1;
	p2 = _p2;
	length = len;
}

void DistanceJoint::SolveConstraints(const std::vector<OBB>& constraints) {
	vec3 delta = p2->GetPosition() - p1->GetPosition();
	float distance = Magnitude(delta);
	float correction = (distance - length) / distance;
	
	p1->SetPosition(p1->GetPosition() + delta * 0.5f * correction);
	p2->SetPosition(p2->GetPosition() - delta * 0.5f * correction);

	p1->SolveConstraints(constraints);
	p2->SolveConstraints(constraints);
}

void DistanceJoint::Render() {
	vec3 pos1 = p1->GetPosition();
	vec3 pos2 = p2->GetPosition();
	Line l(pos1, pos2);
	::Render(l);
}