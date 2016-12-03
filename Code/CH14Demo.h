#ifndef _H_CH14_DEMO_
#define _H_CH14_DEMO_

#include "DemoBase.h"

#include "PhysicsSystem.h"
#include "Particle.h"
#include <vector>
#include <list>

class CH14Demo : public DemoBase {
protected:
	PhysicsSystem physicsSystem;
	int numParticles;
	std::vector<Particle> particles;
	std::list<float> deltaTimes;

	float lastFrameTime;
protected:
	void ResetDemo();
public:
	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
	void ImGUI();

	float Random(float min, float max);
	vec3 Random(vec3 min, vec3 max);
};

#endif 
