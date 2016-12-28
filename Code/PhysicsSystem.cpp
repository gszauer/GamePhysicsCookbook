#include "PhysicsSystem.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include <iostream>

PhysicsSystem::PhysicsSystem() : RenderRandomColors(false) {
	LinearProjectionPercent = 0.45f;
	PenetrationSlack = 0.01f;
	ImpulseIteration = 5;

	colliders1.reserve(100);
	colliders2.reserve(100);
	results.reserve(100);
}

void PhysicsSystem::Update(float deltaTime) {
	colliders1.clear();
	colliders2.clear();
	results.clear();

	{ // Find objects whom are colliding
	  // First, build a list of colliding objects
		for (int i = 0, size = bodies.size(); i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (i == j) {
					continue;
				}
				CollisionManifold result = FindCollisionFeatures(*bodies[i], *bodies[j]);
				if (result.colliding) {
					bool isDuplicate = false;

					// Duplicate detection code was left out of the book
					// Mostly a matter of space and performance!
#if 0
					for (int k = 0, kSize = colliders1.size(); k < kSize; ++k) {
						if (colliders1[k] == bodies[i] || colliders1[k] == bodies[j]) {
							if (colliders2[k] == bodies[i] || colliders2[k] == bodies[j]) {
								isDuplicate = true;
								break;
							}
						}
					}

					if (!isDuplicate) {
						for (int k = 0, kSize = colliders2.size(); k < kSize; ++k) {
							if (colliders2[k] == bodies[i] || colliders2[k] == bodies[j]) {
								if (colliders1[k] == bodies[i] || colliders1[k] == bodies[j]) {
									isDuplicate = true;
									break;
								}
							}
						}
					}
#endif

					if (!isDuplicate) {
						colliders1.push_back(bodies[i]);
						colliders2.push_back(bodies[j]);
						results.push_back(result);
					}
				}
			}
		}
	}

	// Calculate foces acting on the object
	for (int i = 0, size = bodies.size(); i < size; ++i) {
		bodies[i]->ApplyForces();
	}

	// Apply impulses to resolve collisions
	for (int k = 0; k < ImpulseIteration; ++k) { // Apply impulses
		for (int i = 0, size = results.size(); i < size; ++i) {
			for (int j = 0, jSize = results[i].contacts.size(); j < jSize; ++j) {
				ApplyImpulse(*colliders1[i] , *colliders2[i], results[i], j);
			}
		}
	}

	// Correct position to avoid sinking!
	for (int i = 0, size = results.size(); i < size; ++i) {
		float totalMass = colliders1[i]->InvMass() + colliders2[i]->InvMass();

		float depth = fmaxf(results[i].depth - PenetrationSlack, 0.0f);
		float scalar = (totalMass == 0.0f) ? 0.0f : depth / totalMass;
		vec3 correction = results[i].normal * (scalar * LinearProjectionPercent);

		colliders1[i]->position = colliders1[i]->position - correction * colliders1[i]->InvMass();
		colliders2[i]->position = colliders2[i]->position + correction * colliders2[i]->InvMass();
	}

	// Update object positions
	for (int i = 0, size = bodies.size(); i < size; ++i) {
		bodies[i]->Update(deltaTime);
	}

	// Solve constraints
	for (int i = 0, size = bodies.size(); i < size; ++i) {
		bodies[i]->SolveConstraints(constraints);
	}
}

void PhysicsSystem::Render() {
	static const float rigidbodyDiffuse[]{ 200.0f / 255.0f, 0.0f, 0.0f, 0.0f };
	static const float rigidbodyAmbient[]{ 200.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 0.0f };

	static const float groundDiffuse[]{ 0.0f, 0.0f, 200.0f / 255.0f, 0.0f };
	static const float groundAmbient[]{ 50.0f / 255.0f, 50.0f / 255.0f, 200.0f / 255.0f, 0.0f };

	static const float constraintDiffuse[]{ 0.0f, 200.0f / 255.0f, 0.0f, 0.0f };
	static const float constraintAmbient[]{ 50.0f / 255.0f, 200.0f / 255.0f, 50.0f / 255.0f, 0.0f };
	
	static const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	std::vector<const float*> ambient;
	std::vector<const float*> diffuse;
	if (RenderRandomColors) {
		ambient.push_back(rigidbodyAmbient);
		ambient.push_back(groundAmbient);
		ambient.push_back(constraintAmbient);
		diffuse.push_back(rigidbodyDiffuse);
		diffuse.push_back(groundDiffuse);
		diffuse.push_back(constraintDiffuse);
	}

	glColor3f(rigidbodyDiffuse[0], rigidbodyDiffuse[1], rigidbodyDiffuse[2]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, rigidbodyAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, rigidbodyDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
	for (int i = 0, size = bodies.size(); i < size; ++i) {
		if (RenderRandomColors) {
			int a_i = i % ambient.size();
			int d_i = i % diffuse.size();
			glColor3f(diffuse[d_i][0], diffuse[d_i][1], diffuse[d_i][2]);
			glLightfv(GL_LIGHT0, GL_AMBIENT, ambient[a_i]);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse[d_i]);
			glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
		}
		bodies[i]->Render();
	}

	// First constraint is usually the ground
	// Rendering it diferent color is just a hack to make visualization easyer
	// Normally, you wouldn't even render physics geo!
	if (constraints.size() > 0) {
		glColor3f(groundDiffuse[0], groundDiffuse[1], groundDiffuse[2]);
		glLightfv(GL_LIGHT0, GL_AMBIENT, groundAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, groundDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
		::Render(constraints[0]);
	}

	glColor3f(constraintDiffuse[0], constraintDiffuse[1], constraintDiffuse[2]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, constraintAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, constraintDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
	for (int i = 1, size = constraints.size(); i < size; ++i) {
		::Render(constraints[i]);
	}
}

void PhysicsSystem::AddRigidbody(Rigidbody* body) {
	bodies.push_back(body);
}

void PhysicsSystem::AddConstraint(const OBB& obb) {
	constraints.push_back(obb);
}

void PhysicsSystem::ClearRigidbodys() {
	bodies.clear();
}

void PhysicsSystem::ClearConstraints() {
	constraints.clear();
}