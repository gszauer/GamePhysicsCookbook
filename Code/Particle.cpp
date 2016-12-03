#include "Particle.h"
#include "Geometry3D.h"
#include "FixedFunctionPrimitives.h"

#include <iostream> // TODO: Delete

Particle::Particle() {
	mass = 1.0f;
	friction = 0.95f;
	bounce = 0.7f;
	gravity = vec3(0.0f, -9.82f, 0.0f);
}

void Particle::Update(float deltaTime) {
	oldPosition = position;

	vec3 acceleration = forces * (1.0f / mass);
	velocity = velocity * friction + acceleration * deltaTime;
	position = position + velocity * deltaTime;
}

void Particle::Render() {
	Sphere visual(position, 0.1f);
	::Render(visual);
}

void Particle::ApplyForces() {
	forces = gravity;
}

void Particle::SolveConstraints(const std::vector<OBB>& constraints) {
	int size = constraints.size();
	for (int i = 0; i < size; ++i) {
		Line traveled(oldPosition, position);
		if (Linetest(constraints[i], traveled)) {
		//if (PointInOBB(position, constraints[i])) {
			vec3 direction = Normalized(velocity);
			
			Ray ray(oldPosition, direction);
			RaycastResult result;
			
			if (Raycast(constraints[i], ray, &result)) {
				//velocity = Reflection(velocity, result.normal);

				vec3 vn = result.normal * Dot(result.normal, velocity);
				vec3 vt = velocity - vn;
				// Place object just a little above collision result
				position = result.point + result.normal * 0.005f;
				oldPosition = result.point - (vt - vn * bounce);
				velocity = position - oldPosition;
				oldPosition = position;

				//std::cout << "Constraint delta: " + Magnitude(position - oldPosition << "\n";

				break;
			}
		}
	}
}

void Particle::SetPosition(const vec3& pos) {
	position = pos;
	oldPosition = pos;
}

void Particle::SetBounce(float b) {
	bounce = b;
}
void Particle::SetFriction(float f) {
	friction = f;
}

float Particle::GetBounce() {
	return bounce;
}

float Particle::GetFriction() {
	return friction;
}

vec3 Particle::GetPosition() {
	return position;
}
