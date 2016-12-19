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

void LinearImpulse::ResetDemo() {
	physicsSystem.ClearRigidbodys();
	physicsSystem.ClearConstraints();

	spheres.clear();
	spheres.resize(2);

	boxes.clear();
	boxes.resize(2);

	if (!drop) {
		spheres[0].SetPosition(vec3(-10, 1.2f, -5));
		spheres[1].SetPosition(vec3(5, 1.2f, 2.5f));
		boxes[0].SetPosition(vec3(-10, 1.2f, -5));
		boxes[1].SetPosition(vec3(5, 1.2f, 2.5f));

		vec3 impulseDirection = vec3(-10, 1, -5) - vec3(5, 1, 2.5f);
		impulseDirection = impulseDirection;
		spheres[1].AddLinearImpulse(impulseDirection + vec3(0, 5, 0));
		boxes[1].AddLinearImpulse(impulseDirection + vec3(0, 5, 0));
		impulseDirection = vec3(5, 1, 2.5f) - vec3(-10, 1, -5);
		impulseDirection = impulseDirection;
		spheres[0].AddLinearImpulse(impulseDirection + vec3(0, 5, 0));
		boxes[0].AddLinearImpulse(impulseDirection + vec3(0, 5, 0));
	}
	else {
		spheres[0].SetPosition(vec3(0, 1.2, 0));
		spheres[1].SetPosition(vec3(0, 4, 0));
		boxes[0].SetPosition(vec3(0, 1.2, 0));
		boxes[1].SetPosition(vec3(0, 4, 0));
	}

	groundBox = RigidBox();
	groundBox.box.size = vec3(10.0f, 0.15f, 10.0f);
	groundBox.mass = 0.0f;

	if (use_spheres) {
		for (int i = 0; i < spheres.size(); ++i) {
			physicsSystem.AddRigidbody(&spheres[i]);
		}
	}
	else {
		for (int i = 0; i < boxes.size(); ++i) {
			physicsSystem.AddRigidbody(&boxes[i]);
		}
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

	ImGui::Checkbox("Spheres", &use_spheres);
	ImGui::SameLine();
	ImGui::Checkbox("Drop", &drop);

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