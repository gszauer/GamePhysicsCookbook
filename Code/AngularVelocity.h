#ifndef _H_ANGULAR_VELOCITY_
#define _H_ANGULAR_VELOCITY_

#include "DemoBase.h"
#include "Geometry3D.h"
#include "PhysicsSystem.h"

class AngularVelocity : public DemoBase {
protected:
	PhysicsSystem physicsSystem;
	std::vector<Rigidbody> bodies;
	Rigidbody groundBox;

	bool applyRaycast;
	Ray ray;
	float force;

	bool size_imgui_window;
protected:
	void ResetDemo();
public:
	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
	void ImGUI();
};


#endif
