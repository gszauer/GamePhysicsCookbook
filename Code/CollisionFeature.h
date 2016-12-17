#ifndef _H_GENERIC_SAT_DEMO_
#define _H_GENERIC_SAT_DEMO_

#include "DemoBase.h"
#include "Geometry3D.h"

class CollisionFeature : public DemoBase {
protected:
	Sphere sphere;
	OBB obb[2];

	float* manipulator;
	mat4 manTranslation;
	mat4 manRotation;
	mat4 manScale;
	int manipulating;
	bool transformWorld;
public:
	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
	void ImGUI();
};

#endif