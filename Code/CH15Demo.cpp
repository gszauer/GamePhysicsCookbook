#include "CH15Demo.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>

void CH15Demo::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	physicsSystem.RenderRandomColors = true;
	physicsSystem.ImpulseIteration = 8;
	physicsSystem.DoLinearProjection = true;
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

void CH15Demo::ResetDemo() {
	physicsSystem.ClearRigidbodys();
	physicsSystem.ClearConstraints();

	bodies.clear();
	bodies.resize(2);

	bodies[0].type = RIGIDBODY_TYPE_BOX;
	bodies[0].position = vec3(0.5f, 6, 0);
#ifndef LINEAR_ONLY
	bodies[0].orientation = vec3(0.0f, 0.0f, 0.4f);
#endif

	bodies[1].type = RIGIDBODY_TYPE_BOX;
	bodies[1].position = vec3(0, 1, 0);
	bodies[1].mass = 5.0f;

	groundBox = RigidbodyVolume(RIGIDBODY_TYPE_BOX);
	groundBox.position = vec3(0, -0.5f, 0) * vec3(1, 0.5f, 1);
	groundBox.box.size = vec3(50, 1, 50) * 0.25f;
	groundBox.mass = 0.0f;
	groundBox.SynchCollisionVolumes();

	for (int i = 0; i < bodies.size(); ++i) {
		bodies[i].SynchCollisionVolumes();
		physicsSystem.AddRigidbody(&bodies[i]);
	}
	physicsSystem.AddRigidbody(&groundBox);
}

void CH15Demo::ImGUI() {
	DemoBase::ImGUI();

	if (size_imgui_window) {
		size_imgui_window = false;
		ImGui::SetNextWindowPos(ImVec2(400, 10));
		ImGui::SetNextWindowSize(ImVec2(370, 75));
	}

	ImGui::Begin("Chapter 15 Demo", 0, ImGuiWindowFlags_NoResize);

	ImGui::PushItemWidth(55);
	ImGui::SliderFloat("Porjection", &physicsSystem.LinearProjectionPercent, 0.2f, 0.8f);
	ImGui::SameLine();
	ImGui::PushItemWidth(55);
	ImGui::SliderFloat("Slop", &physicsSystem.PenetrationSlack, 0.01f, 0.1f);
	ImGui::SameLine();
	ImGui::PushItemWidth(55);
	ImGui::SliderInt("Iteration", &physicsSystem.ImpulseIteration, 1, 20);

	if (ImGui::Button("Reset")) {
		ResetDemo();
	}
	ImGui::SameLine();
	ImGui::Checkbox("Debug Render", &physicsSystem.DebugRender);
	ImGui::SameLine();
	ImGui::Checkbox("Linear Projection", &physicsSystem.DoLinearProjection);

	ImGui::End();
}

void CH15Demo::Render() {
	DemoBase::Render();

	float val[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	physicsSystem.Render();
}

void CH15Demo::Update(float dt) {
	DemoBase::Update(dt);


	physicsSystem.Update(dt);
}