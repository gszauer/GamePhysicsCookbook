#include "CH14Demo.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include <iostream>>

extern double GetMilliseconds();

void CH14Demo::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);
	val[0] = 0.0f; val[1] = 1.0f; val[2] = 0.0f; val[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, val);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, val);
	val[0] = 1.0f; val[2] = 1.0f;
	glLightfv(GL_LIGHT0, GL_SPECULAR, val);

	numParticles = 50;

	lastFrameTime = GetMilliseconds();

	ResetDemo();
}

void CH14Demo::ResetDemo() {
	particles.clear();
	particles.reserve(numParticles);

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

	/*for (int i = 0; i < numParticles; ++i) {
		particles.push_back(Particle());
		particles[i].SetPosition(Random(spawnMin, spawnMax));
		//std::cout << "Spawn at: " << particles[i].GetPosition().x << ", " << particles[i].GetPosition().y << ", " << particles[i].GetPosition().z << "\n";
		//particles[i].SetBounce(Random(0.5f, 1.5f));
		physicsSystem.AddRigidbody(&particles[i]);
	}*/

	vec3 spawnTest[] = {
		vec3(-2.42043, 6.62717, -2.24747),
		vec3(0.688805, 6.67002, -1.09121),
		vec3(-1.07645, 7.29192, 0.292606),
		vec3(0.449552, 5.84822, 0.438277),
		vec3(0.310536, 6.52707, -1.80085),
		vec3(-2.36755, 5.68281, -1.55691),
		vec3(-1.85802, 5.8318, 0.961117),
		vec3(-0.709112, 5.73817, -3.00858),
		vec3(-2.39094, 6.25576, -0.882245),
		vec3(-0.225904, 6.70353, -0.577604),
	};
	int testIndex = 2;

	particles.push_back(Particle());
	particles[0].SetPosition(spawnTest[testIndex]);
	physicsSystem.AddRigidbody(&particles[0]);
}

void CH14Demo::Render() {
	DemoBase::Render();

	physicsSystem.Render();
}

void CH14Demo::ImGUI() {
	DemoBase::ImGUI();

	float avgTime = 0;
	std::list<float>::iterator iterator = deltaTimes.begin();
	for (; iterator != deltaTimes.end(); iterator++) {
		avgTime += *iterator;
	}
	if (deltaTimes.size() > 0) {
		avgTime /= deltaTimes.size();
	}

	ImGui::Begin("Chapter 14 Demo", 0, ImGuiWindowFlags_NoResize);
	ImGui::Text("Simulation delta: %.3f ms/frame", avgTime);
	if (ImGui::Button("Reset")) {
		ResetDemo();
	}

	if (ImGui::Button("Log Camera")) {

	}
	ImGui::End();
}

void CH14Demo::Update(float dt) {
	DemoBase::Update(dt);

	double thisFrameTime = GetMilliseconds();
	deltaTimes.push_back(float(thisFrameTime - lastFrameTime));
	while (deltaTimes.size() > 120) {
		deltaTimes.pop_front();
	}
	lastFrameTime = thisFrameTime;

	physicsSystem.Update(dt);

	for (int i = 0; i < particles.size(); ++i) {
		vec3 position = particles[i].GetPosition();
		if (position.y < -5.0f) {
			position.y = -5.0f;
			particles[i].SetPosition(position);
		}
	}
}

float CH14Demo::Random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}

vec3 CH14Demo::Random(vec3 min, vec3 max) {
	vec3 result;
	result.x = Random(min.x, max.x);
	result.y = Random(min.y, max.y);
	result.z = Random(min.z, max.z);
	return result;
}