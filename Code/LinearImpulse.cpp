#include "LinearImpulse.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>

void LinearImpulse::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	physicsSystem.RenderRandomColors = true;

	size_imgui_window = true;
	isPaused = true;
	numSteps = 0;
	stepSize = 1;

	use_spheres = false;
	drop = false;

	glPointSize(5.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	camera.SetTarget(vec3(3.75622f, 2.98255f, 0.0f));
	camera.SetZoom(9.0f);
	camera.SetRotation(vec2(-67.9312f, 19.8f));

	ResetDemo();
}

#define sphere0 bodies[0]
#define sphere1 bodies[1]
#define box0 bodies[2]
#define box1 bodies[3]

void LinearImpulse::ResetDemo() {
	physicsSystem.ClearRigidbodys();
	physicsSystem.ClearConstraints();

	bodies.clear();
	bodies.resize(4);

	sphere0.type = sphere1.type = RIGIDBODY_TYPE_SPHERE;
	box0.type = box1.type = RIGIDBODY_TYPE_BOX;

	if (!drop) {
		sphere0.position = vec3(-10, 1.5f, -5);
		sphere1.position = vec3(5, 1.5f, 2.5f);
		box0.position = vec3(-10, 1.5f, -5);
		box1.position = vec3(5, 1.5f, 2.5f);

		vec3 impulseDirection = vec3(-10, 1.5f, -5) - vec3(5, 1.5f, 2.5f);
		sphere1.AddLinearImpulse(impulseDirection + vec3(0, 5, 0));
		box1.AddLinearImpulse(impulseDirection + vec3(0, 5, 0));

		impulseDirection = vec3(5, 1.5f, 2.5f) - vec3(-10, 1.5f, -5);
		sphere0.AddLinearImpulse(impulseDirection + vec3(0, 5, 0));
		box0.AddLinearImpulse(impulseDirection + vec3(0, 5, 0));
	}
	else {
		sphere0.position = vec3(0, 1.2, 0);
		sphere1.position = vec3(0, 4, 0);
		box0.position = vec3(0, 1.2, 0);
		box1.position = vec3(0, 4, 0);
	}

	groundBox = RigidbodyVolume(RIGIDBODY_TYPE_BOX);
	groundBox.box.size = vec3(10.0f, 0.15f, 10.0f);
	groundBox.mass = 0.0f;

	if (use_spheres) {
		physicsSystem.AddRigidbody(&sphere0);
		physicsSystem.AddRigidbody(&sphere1);
	}
	else {
		physicsSystem.AddRigidbody(&box0);
		physicsSystem.AddRigidbody(&box1);
	}

	physicsSystem.AddRigidbody(&groundBox);
}

float LinearImpulse::Random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}

vec3 LinearImpulse::Random(vec3 min, vec3 max) {
	vec3 result;
	result.x = Random(min.x, max.x);
	result.y = Random(min.y, max.y);
	result.z = Random(min.z, max.z);
	return result;
}

void LinearImpulse::ImGUI() {
	DemoBase::ImGUI();

	if (size_imgui_window) {
		size_imgui_window = false;
		ImGui::SetNextWindowPos(ImVec2(400, 10));
		ImGui::SetNextWindowSize(ImVec2(370, 100));
	}

	ImGui::Begin("Linear Impulse Demo", 0, ImGuiWindowFlags_NoResize);

	if (ImGui::Button("Step")) {
		numSteps = stepSize;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputInt("Frames", &stepSize);

	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		ResetDemo();
	}
	ImGui::SameLine();
	ImGui::Checkbox("Pause", &isPaused);

	bool wasUsingSpheres = use_spheres;
	bool wasDropping = drop;

	ImGui::Checkbox("Spheres", &use_spheres);
	ImGui::SameLine();
	ImGui::Checkbox("Drop", &drop);

	if (wasUsingSpheres != use_spheres || wasDropping != drop) {
		ResetDemo();
	}

	ImGui::End();
}

void LinearImpulse::Render() {
	DemoBase::Render();

	float val[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	physicsSystem.Render();
}

void LinearImpulse::Update(float dt) {
	DemoBase::Update(dt);

	if (!isPaused) {
		physicsSystem.Update(dt);
	}
	else if (numSteps > 0) {
		numSteps -= 1;
		physicsSystem.Update(dt);
	}
}