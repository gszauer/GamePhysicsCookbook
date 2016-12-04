#include "PhysicsSystem.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"

void PhysicsSystem::Update(float deltaTime) {
	// Add forces
	for (int i = 0, size = bodies.size(); i < size; ++i) {
		bodies[i]->ApplyForces();
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


	glColor3f(rigidbodyDiffuse[0], rigidbodyDiffuse[1], rigidbodyDiffuse[2]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, rigidbodyAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, rigidbodyDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
	for (int i = 0, size = bodies.size(); i < size; ++i) {
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