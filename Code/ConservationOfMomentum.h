#ifndef _H_CONSERVATION_OF_MOMENTUM_
#define _H_CONSERVATION_OF_MOMENTUM_

#include "DemoBase.h"
#include "Geometry3D.h"
#include "PhysicsSystem.h"
#include "RigidbodyVolume.h"

class ConservationOfMomentum : public DemoBase {
protected:
	PhysicsSystem physicsSystem;
	std::vector<RigidbodyVolume> bodies;
	RigidbodyVolume groundBox;

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
