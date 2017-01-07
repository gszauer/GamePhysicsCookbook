#include "SimpleSprings.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include <iostream>

extern double GetMilliseconds();
Spring spring(-2.0f, 0.5f, 1.0f);

void SimpleSprings::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	size_imgui_window = true;
	lastFrameTime = GetMilliseconds();

	camera.SetTarget(vec3(3.75622f, 2.98255f, 0.0f));
	camera.SetZoom(9.0f);
	camera.SetRotation(vec2(-67.9312f, 19.8f));

	ResetDemo();
}

void SimpleSprings::ResetDemo() {
	particles.clear();
	physicsSystem.ClearRigidbodys();
	physicsSystem.ClearConstraints();
	physicsSystem.ClearSprings();

	OBB ground;
	ground.size = vec3(10.0f, 0.15f, 10.0f);


	physicsSystem.AddConstraint(ground);

	particles.push_back(Particle());
	particles.push_back(Particle());
	particles[0].SetPosition(vec3(0, 6, 0));
	particles[0].SetMass(50.0f);
	particles[1].SetPosition(vec3(2, 6, 0));
	physicsSystem.AddRigidbody(&particles[0]);
	physicsSystem.AddRigidbody(&particles[1]);
	
	spring.SetParticles(&particles[0], &particles[1]);
	physicsSystem.AddSpring(spring);
}

void SimpleSprings::Render() {
	DemoBase::Render();

	float val[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	physicsSystem.Render();
}

void SimpleSprings::ImGUI() {
	DemoBase::ImGUI();

	float avgTime = 0;
	std::list<float>::iterator iterator = deltaTimes.begin();
	for (; iterator != deltaTimes.end(); iterator++) {
		avgTime += *iterator;
	}
	if (deltaTimes.size() > 0) {
		avgTime /= deltaTimes.size();
	}

	if (size_imgui_window) {
		size_imgui_window = false;
		ImGui::SetNextWindowPos(ImVec2(400, 10));
		ImGui::SetNextWindowSize(ImVec2(370, 75));
	}

	ImGui::Begin("Simple Springs Demo", 0, ImGuiWindowFlags_NoResize);

	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("k", &spring.k, -5.0f, 0.0f);
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("b", &spring.b, 0.0f, 1.0f);
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("l", &spring.restingLength, 0.0f, 5.0f);

	if (ImGui::Button("Reset")) {
		ResetDemo();
	}
	ImGui::SameLine();
	if (ImGui::Button("Show Help")) {
		show_help = true;
	}
	ImGui::End();
}

void SimpleSprings::Update(float dt) {
	DemoBase::Update(dt);

	double thisFrameTime = GetMilliseconds();
	deltaTimes.push_back(float(thisFrameTime - lastFrameTime));
	while (deltaTimes.size() > 120) {
		deltaTimes.pop_front();
	}
	lastFrameTime = thisFrameTime;

	physicsSystem.Update(dt);

	// This next part isn't really needed. It's just in place 
	// in case the simulation becomes unstable and something 
	// falls below the floor of the world!
	for (int i = 0; i < particles.size(); ++i) {
		vec3 position = particles[i].GetPosition();
		if (position.y < -5.0f) {
			position.y = -5.0f;
			particles[i].SetPosition(position);
		}
	}

	//float distance = Magnitude(particles[0].GetPosition() - particles[1].GetPosition());
	//std::cout << "Distance: " << distance << "\n";
}

float SimpleSprings::Random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}

vec3 SimpleSprings::Random(vec3 min, vec3 max) {
	vec3 result;
	result.x = Random(min.x, max.x);
	result.y = Random(min.y, max.y);
	result.z = Random(min.z, max.z);
	return result;
}