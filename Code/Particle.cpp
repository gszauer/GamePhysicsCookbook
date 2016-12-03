#include "Particle.h"
#include "Geometry3D.h"
#include "FixedFunctionPrimitives.h"

#include <iostream> // TODO: Delete

Particle::Particle() {
	friction = 0.95f;
	bounciness = 0.7f;
	mass = 1.0f;
	gravity = vec3(0.0f, -9.82f, 0.0f);
}

void Particle::Update(float deltaTime) {
	vec3 positionCache = position;

	position = position + ((position) - oldPosition) * deltaTime + allForces * deltaTime;

	/*vec3 acceleration = allForces;// *(1.0f / 0.05f);
	vec3 lastVelocity = position - oldPosition;
	vec3 velocity = lastVelocity + acceleration *deltaTime;
	position = position + (lastVelocity + velocity) * 0.5f * deltaTime;*/

	/*vec3 velocity = position - oldPosition;
	velocity = velocity + allForces * deltaTime;
	position = position + velocity * deltaTime;*/

	/*vec3 OldVel = Vel;
	Vel = Vel + Accel * dt;
	Pos = Pos + (OldVel + Vel) * 0.5 * dt;*/

	oldPosition = positionCache;

	/*vec3 positionCache = position;
	position = position + (((position - oldPosition) * friction) + (allForces * deltaTime * deltaTime)) ;
	oldPosition = positionCache;*/


	/*vec3 positionCache = position;
	vec3 oldVelocity = (position - oldPosition) * friction; // Verlet integration

	vec3 acceleration = allForces;// *(1.0f / mass);
	vec3 velocity = oldVelocity + acceleration * deltaTime;

	position = position + velocity * deltaTime;
	oldPosition = positionCache;*/
}

void Particle::Render() {
	Sphere visual(position, 0.25f);
	::Render(visual);
}

void Particle::ApplyForces() {
	allForces = gravity;
}

void Particle::SolveConstraints(const std::vector<OBB>& constraints) {
	int size = constraints.size();
	for (int i = 0; i < size; ++i) {
		Line traveled(oldPosition, position);
		if (Linetest(constraints[i], traveled)) {
		//if (PointInOBB(position, constraints[i])) {
			vec3 velocity = position - oldPosition;
			vec3 direction = Normalized(velocity);
			
			Ray ray(oldPosition, direction);
			RaycastResult result;
			
			if (Raycast(constraints[i], ray, &result)) {

				vec3 reflection = Reflection(velocity, result.normal);

				vec3 vn = result.normal * Dot(result.normal, velocity);
				vec3 vt = velocity - vn;
				// Place object just a little above collision result
				position = result.point + result.normal * 0.005f;
				oldPosition = result.point - (vt - vn/* reflection */* bounciness);

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
	bounciness = b;
}
void Particle::SetFriction(float f) {
	friction = f;
}

float Particle::GetBounce() {
	return bounciness;
}

float Particle::GetFriction() {
	return friction;
}

vec3 Particle::GetPosition() {
	return position;
}
