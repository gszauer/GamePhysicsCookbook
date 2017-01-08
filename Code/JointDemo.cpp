#include "JointDemo.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include <iostream>

extern double GetMilliseconds();

void JointDemo::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	numParticles = 50;
	size_imgui_window = true;
	lastFrameTime = GetMilliseconds();

	camera.SetTarget(vec3(3.75622f, 2.98255f, 0.0f));
	camera.SetZoom(9.0f);
	camera.SetRotation(vec2(-67.9312f, 19.8f));

	ResetDemo();
}

void JointDemo::ResetDemo() {
	particles.clear();
	joints.clear();
	joints.reserve(numParticles / 2 + 1);
	particles.reserve(numParticles + 1);

	physicsSystem.ClearRigidbodys();
	physicsSystem.ClearConstraints();


	OBB ground;
	ground.size = vec3(10.0f, 0.15f, 10.0f);

	OBB obb1;
	obb1.position = vec3(0.0f, 1.86f, -1.92f);
	obb1.orientation = Rotation3x3(30.716f, 0.0f, 0.0f);
	obb1.size = vec3(2.0f, 0.15f, 2.0f);

	OBB obb2;
	obb2.position = vec3(-1.0f, 3.6f, 1.2f);
	obb2.orientation = Rotation3x3(-33.964f, -24.233f, 9.128f);
	obb2.size = vec3(2.0f, 0.15f, 2.0f);

	OBB obb3;
	obb3.position = vec3(0.0f, 3.93f, -2.27f);
	obb3.orientation = Rotation3x3(24.702f, 0.0f, 0.0f);
	obb3.size = vec3(2.0f, 0.15f, 0.7817011f);

	physicsSystem.AddConstraint(ground);
	physicsSystem.AddConstraint(obb1);
	physicsSystem.AddConstraint(obb2);
	physicsSystem.AddConstraint(obb3);

	vec3 spawnPos = vec3(-0.5f, 6.5f, -1.01f);
	vec3 spawnSize = vec3(3.8505f, 2, 4.034834f);
	vec3 spawnMin = spawnPos - spawnSize;
	vec3 spawnMax = spawnPos + spawnSize;

	int createCount = numParticles;
	if (numParticles % 2 != 0) {
		numParticles += 1;
	}
	int numJoints = createCount / 2;

	for (int i = 0; i < createCount; ++i) {
		particles.push_back(Particle());
		particles[i].SetPosition(Random(spawnMin, spawnMax));
		particles[i].SetBounce(Random(0, 1));
	}

	for (int i = 0; i < createCount; ++i) {
		physicsSystem.AddRigidbody(&particles[i]);
	}

	for (int i = 0; i < numJoints; ++i) {
		joints.push_back(DistanceJoint());
		joints[i].Initialize(&particles[i], &particles[i + numJoints], Random(0.5f, 1.0f));
		physicsSystem.AddRigidbody(&joints[i]);
	}
}

void JointDemo::Render() {
	DemoBase::Render();

	float val[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	physicsSystem.Render();
}

void JointDemo::ImGUI() {
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

	ImGui::Begin("Joint Demo", 0, ImGuiWindowFlags_NoResize);

	ImGui::Text("Particle count");
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("", &numParticles, 2, 100);
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		ResetDemo();
	}
	ImGui::SameLine();
	if (ImGui::Button("Show Help")) {
		show_help = true;
	}
	ImGui::End();
}

void JointDemo::Update(float dt) {
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
}

float JointDemo::Random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}

vec3 JointDemo::Random(vec3 min, vec3 max) {
	vec3 result;
	result.x = Random(min.x, max.x);
	result.y = Random(min.y, max.y);
	result.z = Random(min.z, max.z);
	return result;
}