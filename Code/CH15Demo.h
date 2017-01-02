#ifndef _H_ANGULAR_VELOCITY_
#define _H_ANGULAR_VELOCITY_

#include "DemoBase.h"
#include "Geometry3D.h"
#include "PhysicsSystem.h"
#include "RigidbodyVolume.h"

class CH15Demo : public DemoBase {
protected:
	PhysicsSystem physicsSystem;
	std::vector<RigidbodyVolume> bodies;
	RigidbodyVolume groundBox;

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
