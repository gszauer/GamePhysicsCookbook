#include "PhysicsSystem.h"
#include "RigidbodyVolume.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include <iostream>

PhysicsSystem::PhysicsSystem() {
	LinearProjectionPercent = 0.45f;
	PenetrationSlack = 0.01f;
	ImpulseIteration = 5;

	DebugRender = false;
	DoLinearProjection = true;
	RenderRandomColors = false;

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
		CollisionManifold result;
		for (int i = 0, size = bodies.size(); i < size; ++i) {
			for (int j = i; j < size; ++j) {
				if (i == j) {
					continue;
				}
				ResetCollisionManifold(&result);
				if (bodies[i]->HasVolume() && bodies[j]->HasVolume()) {
					RigidbodyVolume* m1 = (RigidbodyVolume*)bodies[i];
					RigidbodyVolume* m2 = (RigidbodyVolume*)bodies[j];
					result = FindCollisionFeatures(*m1, *m2);
				}
				if (result.colliding) {
#if 0 
					bool isDuplicate = false;
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
					if (!isDuplicate)
#endif

					{
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

	// Same as above, calculate forces acting on cloths
	for (int i = 0, size = cloths.size(); i < size; ++i) {
		cloths[i]->ApplyForces();
	}

	// Apply impulses to resolve collisions
	for (int k = 0; k < ImpulseIteration; ++k) { // Apply impulses
		for (int i = 0, size = results.size(); i < size; ++i) {
			for (int j = 0, jSize = results[i].contacts.size(); j < jSize; ++j) {
				if (colliders1[i]->HasVolume() && colliders2[i]->HasVolume()) {
					RigidbodyVolume* m1 = (RigidbodyVolume*)colliders1[i];
					RigidbodyVolume* m2 = (RigidbodyVolume*)colliders2[i];
					ApplyImpulse(*m1, *m2, results[i], j);
				}
			}
		}
	}

	// Integrate velocity and impulse of objects
	for (int i = 0, size = bodies.size(); i < size; ++i) {
		bodies[i]->Update(deltaTime);
	}

	// Same as above, integrate velocity and impulse of cloths
	for (int i = 0, size = cloths.size(); i < size; ++i) {
		cloths[i]->Update(deltaTime);
	}

	// Correct position to avoid sinking!
	if (DoLinearProjection) {
		for (int i = 0, size = results.size(); i < size; ++i) {
			if (!colliders1[i]->HasVolume() && !colliders2[i]->HasVolume()) {
				continue;
			}

			RigidbodyVolume* m1 = (RigidbodyVolume*)colliders1[i];
			RigidbodyVolume* m2 = (RigidbodyVolume*)colliders2[i];
			float totalMass = m1->InvMass() + m2->InvMass();

			if (totalMass == 0.0f) {
				continue;
			}

			float depth = fmaxf(results[i].depth - PenetrationSlack, 0.0f);
			float scalar = (totalMass == 0.0f) ? 0.0f : depth / totalMass;
			vec3 correction = results[i].normal * scalar * LinearProjectionPercent;

			m1->position = m1->position - correction * m1->InvMass();
			m2->position = m2->position + correction * m2->InvMass();

			m1->SynchCollisionVolumes();
			m2->SynchCollisionVolumes();
		}
	}

	// Apply spring forces
	for (int i = 0, size = springs.size(); i < size; ++i) {
		springs[i].ApplyForce(deltaTime);
	}

	// Same as above, apply spring forces for cloths
	for (int i = 0, size = cloths.size(); i < size; ++i) {
		cloths[i]->ApplySpringForces(deltaTime);
	}

	// Solve constraints
	for (int i = 0, size = bodies.size(); i < size; ++i) {
		bodies[i]->SolveConstraints(constraints);
	}

	// Same as above, solve cloth constraints
	for (int i = 0, size = cloths.size(); i < size; ++i) {
		cloths[i]->SolveConstraints(constraints);
	}
}

void PhysicsSystem::Render() {
	if (DebugRender) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
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
		if (DebugRender && bodies[i]->type == RIGIDBODY_TYPE_BOX) {
			RigidbodyVolume* mb = (RigidbodyVolume*)bodies[i];
			mb->SynchCollisionVolumes();
			::Render(GetEdges(mb->box));
		}
		else {
			bodies[i]->Render();
		}
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
	if (DebugRender) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GLboolean status;
		glGetBooleanv(GL_LIGHTING, &status);

		glDisable(GL_LIGHTING);
		for (int i = 0; i < results.size(); ++i) {
			::Render(results[i]);
		}
		if (status) {
			glEnable(GL_LIGHTING);
		}
	}

	// Render springs
	GLboolean status;
	glGetBooleanv(GL_LIGHTING, &status);
	for (int i = 0, size = springs.size(); i < size; ++i) {
		for (int i = 0, size = springs.size(); i < size; ++i) {
			if (springs[i].GetP1() == 0 || springs[i].GetP2() == 0) {
				continue;
			}

			Line l(springs[i].GetP1()->GetPosition(), springs[i].GetP2()->GetPosition());
			::Render(l);
		}
	}
	if (status) {
		glEnable(GL_LIGHTING);
	}

	// Render all cloths
	for (int i = 0, size = cloths.size(); i < size; ++i) {
		cloths[i]->Render(DebugRender);
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

void PhysicsSystem::AddSpring(const Spring& spring) {
	springs.push_back(spring);
}

void PhysicsSystem::ClearSprings() {
	springs.clear();
}

void PhysicsSystem::AddCloth(Cloth* cloth) {
	cloths.push_back(cloth);
}

void PhysicsSystem::ClearCloths() {
	cloths.clear();
}