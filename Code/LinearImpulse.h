#ifndef _H_LINEAR_IMPULSE_
#define _H_LINEAR_IMPULSE_

#include "DemoBase.h"
#include "Geometry3D.h"
#include "PhysicsSystem.h"

class LinearImpulse : public DemoBase {
protected:
	PhysicsSystem physicsSystem;
	std::vector<Rigidbody> bodies;
	Rigidbody groundBox;

	bool size_imgui_window;
	bool isPaused;
	int numSteps;
	int stepSize;
	bool use_spheres;
	bool drop;
protected:
	void ResetDemo();
	float Random(float min, float max);
	vec3 Random(vec3 min, vec3 max);
public:
	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
	void ImGUI();
};


#endif
