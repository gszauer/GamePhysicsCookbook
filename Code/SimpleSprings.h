#ifndef _H_SIMPLE_SPRINGS_
#define _H_SIMPLE_SPRINGS_

#include "DemoBase.h"

#include "PhysicsSystem.h"
#include "Particle.h"
#include "Spring.h"
#include <vector>
#include <list>

class SimpleSprings : public DemoBase {
protected:
	PhysicsSystem physicsSystem;
	std::vector<Particle> particles;
	std::list<float> deltaTimes;

	float lastFrameTime;
	bool size_imgui_window;
protected:
	void ResetDemo();
public:
	inline SimpleSprings() : DemoBase(), size_imgui_window(true) { }

	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
	void ImGUI();

	float Random(float min, float max);
	vec3 Random(vec3 min, vec3 max);
};

#endif 
