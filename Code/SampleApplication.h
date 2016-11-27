#ifndef _H_SAMPLE_APPLICATION_
#define _H_SAMPLE_APPLICATION_

// TODO: DELETE THIS FILE!

#include "GLWindow.h"
#include "vectors.h"
#include "Camera.h"

class SampleApplication : public GLWindow {
protected:
	vec2 cameraPos;
	float cameraDist;
	vec2 m_prevMousePos;

	Mesh meshObject;
	Model modelObject;
	AABB ground;
	class Scene* scene;

	bool doRaycast = false;
	Ray cast;
	vec2 castMouse;
public:
	OrbitCamera camera;

	SampleApplication(const char* title, int width, int height) 
		: GLWindow(title, width, height) {
		m_nFixedFPS = 30.0f; 
		m_nTargetFPS = 60.0f;
	}
	virtual ~SampleApplication() { }

	virtual void OnRender();
	virtual void OnResize(int width, int height);
	virtual void OnInitialize();
	virtual void OnUpdate(float deltaTime);
	virtual void OnShutdown();

public:
	float random(float min, float max);
};

// Static instance defined in .cpp file!

#endif 
