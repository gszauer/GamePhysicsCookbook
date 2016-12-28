#include "AngularVelocity.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>

void AngularVelocity::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	physicsSystem.RenderRandomColors = true;
	physicsSystem.ImpulseIteration = 8;
	//physicsSystem.PenetrationSlack = 0.1f;
	//physicsSystem.LinearProjectionPercent = 0.8f;

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
	bodies.resize(3);

	bodies[0].type = RIGIDBODY_TYPE_BOX;
	bodies[0].position = vec3(4.5f, 10, 0);
	bodies[0].orientation = vec3(0.0f, 0.0f, 0.4f);
	bodies[0].box.size = vec3(0.75, 0.75, 0.75);
	bodies[0].mass = 5.0f;
	bodies[0].debug = "Box 0";

	bodies[1].type = RIGIDBODY_TYPE_BOX;
	bodies[1].position = vec3(0, 3, 0);
	bodies[1].box.size = vec3(20, 2, 5) * 0.25f;
	bodies[1].debug = "Box 1";
	bodies[1].cor = 0.0f;

	bodies[2].type = RIGIDBODY_TYPE_BOX;
	bodies[2].position = vec3(0, 1, 0);
	bodies[2].box.size = vec3(5, 5, 5) * 0.25f;
	bodies[2].debug = "Blue";
	bodies[2].cor = 0.0f;

	groundBox = Rigidbody(RIGIDBODY_TYPE_BOX);
	groundBox.position = vec3(0, -0.5f, 0) * vec3(1, 0.5f, 1);
	groundBox.box.size = vec3(50, 1, 50) * 0.25f;
	groundBox.mass = 0.0f;
	groundBox.SynchCollisionVolumes();
	groundBox.debug = "Ground";

	for (int i = 0; i < bodies.size(); ++i) {
		bodies[i].SynchCollisionVolumes();
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