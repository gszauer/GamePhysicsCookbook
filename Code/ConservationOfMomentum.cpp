#include "ConservationOfMomentum.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>

void ConservationOfMomentum::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	physicsSystem.RenderRandomColors = true;
	physicsSystem.ImpulseIteration = 20;

	size_imgui_window = true;

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

void ConservationOfMomentum::ResetDemo() {
	physicsSystem.ClearRigidbodys();
	physicsSystem.ClearConstraints();

	bodies.clear();
	bodies.resize(5);

	for (int i = 0; i < 5; ++i) {
		bodies[i].type = RIGIDBODY_TYPE_SPHERE;
		float pos = (float)i * (bodies[i].sphere.radius + 0.00001f) * 2.0f;
		if (i == 4) {
			pos = (float)i * (bodies[i].sphere.radius + 0.00001f) * 3.0f;
		}
		bodies[i].position = vec3(pos, 1.16, 0.0f);
		physicsSystem.AddRigidbody(&bodies[i]);
	}
	vec3 impulse = vec3(-bodies[4].position.x, 0.0f, 0.0f);
	Normalize(impulse);
	bodies[4].AddLinearImpulse(impulse * 20.0f);

	groundBox = RigidbodyVolume(RIGIDBODY_TYPE_BOX);
	groundBox.box.size = vec3(15.0f, 0.15f, 15.0f);
	groundBox.mass = 0.0f;

	physicsSystem.AddRigidbody(&groundBox);
}

float ConservationOfMomentum::Random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}

vec3 ConservationOfMomentum::Random(vec3 min, vec3 max) {
	vec3 result;
	result.x = Random(min.x, max.x);
	result.y = Random(min.y, max.y);
	result.z = Random(min.z, max.z);
	return result;
}

void ConservationOfMomentum::ImGUI() {
	DemoBase::ImGUI();

	if (size_imgui_window) {
		size_imgui_window = false;
		ImGui::SetNextWindowPos(ImVec2(400, 10));
		ImGui::SetNextWindowSize(ImVec2(370, 100));
	}

	ImGui::Begin("Conservation Demo", 0, ImGuiWindowFlags_NoResize);

	if (ImGui::Button("Reset")) {
		ResetDemo();
	}

	ImGui::End();
}

void ConservationOfMomentum::Render() {
	DemoBase::Render();

	float val[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	physicsSystem.Render();
}

void ConservationOfMomentum::Update(float dt) {
	DemoBase::Update(dt);

	physicsSystem.Update(dt);
}