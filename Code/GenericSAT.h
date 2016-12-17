#ifndef _H_GENERIC_SAT_DEMO_
#define _H_GENERIC_SAT_DEMO_

#include "DemoBase.h"
#include "Geometry3D.h"

class GenericSAT : public DemoBase {
protected:
	//Sphere sphere;
	AABB aabb;
	OBB obb;
	Triangle triangle;

	float* manipulator;
	mat4 manTranslation;
	mat4 manRotation;
	mat4 manScale;
	int manipulating;
	bool transformWorld;
	Triangle centroidOffset;
public:
	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
	void ImGUI();
};

#endif