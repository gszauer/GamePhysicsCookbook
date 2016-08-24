#ifndef _H_CAMERA_
#define _H_CAMERA_

#include "matrices.h"

class Camera {
protected:
	mat4 viewMatrix;
	mat4 projectionMatrix;

public:

	void Perspective(float fov, float aspect, float zNear, float zFar);
	float Orthographic(); // TODO
};

#endif 
