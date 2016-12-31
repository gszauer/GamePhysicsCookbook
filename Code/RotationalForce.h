#ifndef _H_ROTATIONAL_FORCE_
#define _H_ROTATIONAL_FORCE_

#include "DemoBase.h"
#include "Geometry3D.h"
#include "PhysicsSystem.h"

class RotationalForce : public DemoBase {
protected:
	PhysicsSystem physicsSystem;
	std::vector<Rigidbody> bodies;
	Rigidbody groundBox;
	Ray ray;
	float force;
	bool allowForceAdd;
	bool step;
	bool allowed;

	bool size_imgui_window;
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
