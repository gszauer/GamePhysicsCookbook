#include "RotationalForce.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>

void RotationalForce::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	physicsSystem.RenderRandomColors = true;
	physicsSystem.ImpulseIteration = 20;

	size_imgui_window = true;
	force = 5;
	allowForceAdd = false;

	step = true;
	allowed = false;

	glPointSize(5.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	camera.SetTarget(vec3(3.75622f, 2.98255f, 0.0f));
	camera.SetZoom(12.0f);
	camera.SetRotation(vec2(-67.9312f, 19.8f));

	ResetDemo();
}

void RotationalForce::ResetDemo() {
	physicsSystem.ClearRigidbodys();
	physicsSystem.ClearConstraints();


	bodies.clear();
	bodies.resize(5);

	bodies.resize(2);
	bodies[0].type = RIGIDBODY_TYPE_BOX;
	bodies[0].position = vec3(0, 10, 0);

	bodies[1].type = RIGIDBODY_TYPE_BOX;
	bodies[1].position = vec3(1.6, 2, 0);


	for (int i = 0; i < bodies.size(); ++i) {
		physicsSystem.AddRigidbody(&bodies[i]);
	}

	groundBox = Rigidbody(RIGIDBODY_TYPE_BOX);
	groundBox.box.size = vec3(15.0f, 0.15f, 15.0f);
	groundBox.mass = 0.0f;

	physicsSystem.AddRigidbody(&groundBox);
}

float RotationalForce::Random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}

vec3 RotationalForce::Random(vec3 min, vec3 max) {
	vec3 result;
	result.x = Random(min.x, max.x);
	result.y = Random(min.y, max.y);
	result.z = Random(min.z, max.z);
	return result;
}

void RotationalForce::ImGUI() {
	DemoBase::ImGUI();

	if (size_imgui_window) {
		size_imgui_window = false;
		ImGui::SetNextWindowPos(ImVec2(400, 10));
		ImGui::SetNextWindowSize(ImVec2(370, 100));
	}

	ImGui::Begin("Conservation Demo", 0, ImGuiWindowFlags_NoResize);

	ImGui::Checkbox("Step", &step);
	ImGui::SameLine();
	if (ImGui::Button("Go")) {
		allowed = true;
	}
	ImGui::SameLine();
	ImGui::Checkbox("Allow force", &allowForceAdd);
	ImGui::SameLine();
	ImGui::DragFloat("Force", &force, 1.0f, 0.0f, 10.0f);


	if (ImGui::Button("Reset")) {
		ResetDemo();
	}

	ImGui::End();
}

void RotationalForce::Render() {
	DemoBase::Render();

	float val[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	physicsSystem.Render();

	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 1.0f);
	::Render(ray);
	glEnable(GL_LIGHTING);
}

void RotationalForce::Update(float dt) {
	DemoBase::Update(dt);

	if (step) {
		if (allowed) {
			physicsSystem.Update(dt);
			allowed = false;
		}
	}
	else {
		physicsSystem.Update(dt);
		if (mouseLeftDown) {
			ray = GetPickRay(mousePos, vec2(), size, camera.GetViewMatrix(), camera.GetProjectionMatrix());
			ray.NormalizeDirection();
		}

		RaycastResult result;
		if (allowForceAdd && Raycast(bodies[0].box, ray, &result)) {
			bodies[0].AddRotationalImpulse(result.point, ray.direction * force);
			allowForceAdd = false;
		}
	}
}