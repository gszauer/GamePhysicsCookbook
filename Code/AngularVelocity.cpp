#include "AngularVelocity.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>

void AngularVelocity::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	physicsSystem.RenderRandomColors = true;
	physicsSystem.ImpulseIteration = 20;

	size_imgui_window = true;
	applyRaycast = false;
	force = 10.0f;

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

void AngularVelocity::ResetDemo() {
	physicsSystem.ClearRigidbodys();
	physicsSystem.ClearConstraints();

	bodies.clear();
	bodies.resize(2);

	bodies[0].type = RIGIDBODY_TYPE_BOX;
	bodies[0].position = vec3(0, 3, 0);

	bodies[1].type = RIGIDBODY_TYPE_BOX;
	bodies[1].position = vec3(0, 6, 0);

	groundBox = Rigidbody(RIGIDBODY_TYPE_BOX);
	groundBox.box.size = vec3(15.0f, 0.15f, 15.0f);
	groundBox.mass = 0.0f;

	for (int i = 0; i < bodies.size(); ++i) {
		physicsSystem.AddRigidbody(&bodies[i]);
	}
	physicsSystem.AddRigidbody(&groundBox);
}

void AngularVelocity::ImGUI() {
	DemoBase::ImGUI();

	if (size_imgui_window) {
		size_imgui_window = false;
		ImGui::SetNextWindowPos(ImVec2(400, 10));
		ImGui::SetNextWindowSize(ImVec2(370, 100));
	}

	ImGui::Begin("Conservation Demo", 0, ImGuiWindowFlags_NoResize);

	ImGui::DragFloat("Force", &force, 1.0f, 0.0f, 50.0f);
	ImGui::SameLine();
	if (ImGui::Button("Raycast")) {
		applyRaycast = true;
	}

	if (ImGui::Button("Reset")) {
		ResetDemo();
	}

	ImGui::End();
}

void AngularVelocity::Render() {
	DemoBase::Render();

	float val[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	physicsSystem.Render();

	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 1.0f);
	::Render(ray);
	glEnable(GL_LIGHTING);
}

void AngularVelocity::Update(float dt) {
	DemoBase::Update(dt);

	if (mouseLeftDown) {
		ray = GetPickRay(mousePos, vec2(), size, camera.GetViewMatrix(), camera.GetProjectionMatrix());
		ray.NormalizeDirection();
	}

	if (applyRaycast) {
		RaycastResult hit;
		if (Raycast(bodies[0].box, ray, &hit)) {
			ray.NormalizeDirection();
			bodies[0].AddRotationalImpulse(hit.point, ray.direction * force);
		}
		applyRaycast = false;
	}

	physicsSystem.Update(dt);
}