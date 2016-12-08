#ifndef _H_RAYCAST_DEMO_
#define _H_RAYCAST_DEMO_

#include "DemoBase.h"
#include "Geometry3D.h"

class RaycastDemo : public DemoBase {
protected:
	RaycastResult raycastResult;
	Sphere sphere;
	AABB aabb;
	OBB obb;
	Plane plane;
	Triangle triangle;
	Ray screenRay;
public:
	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
};

#endif
