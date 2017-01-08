#ifndef _H_JOINT_DEMO_
#define _H_JOINT_DEMO_

#include "DemoBase.h"
#include "PhysicsSystem.h"
#include "DistanceJoint.h"
#include "Particle.h"
#include <vector>
#include <list>

class JointDemo : public DemoBase {
protected:
	PhysicsSystem physicsSystem;
	int numParticles;
	std::vector<Particle> particles;
	std::vector<DistanceJoint> joints;
	std::list<float> deltaTimes;

	float lastFrameTime;
	bool size_imgui_window;
protected:
	void ResetDemo();
public:
	inline JointDemo() : DemoBase(), size_imgui_window(true) { }

	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
	void ImGUI();

	float Random(float min, float max);
	vec3 Random(vec3 min, vec3 max);
};

#endif
