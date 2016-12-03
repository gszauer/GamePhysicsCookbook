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
	float bodyColor[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	float specialColor[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	float constracintColor[] = { 0.0f, 0.0f, 1.0f, 0.0f };

	glColor3f(bodyColor[0], bodyColor[1], bodyColor[2]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, bodyColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, bodyColor);
	for (int i = 0, size = bodies.size(); i < size; ++i) {
		bodies[i]->Render();
	}

	// First constraint is usually the ground
	// Rendering it diferent color is just a hack to make visualization easyer
	// Normally, you wouldn't even render physics geo!
	if (constraints.size() > 0) {
		glColor3f(specialColor[0], specialColor[1], specialColor[2]);
		glLightfv(GL_LIGHT0, GL_AMBIENT, specialColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, specialColor);
		::Render(constraints[0]);
	}

	glColor3f(constracintColor[0], constracintColor[1], constracintColor[2]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, constracintColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, constracintColor);
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