#ifndef _H_SAMPLE_APPLICATION_
#define _H_SAMPLE_APPLICATION_

#include "GLWindow.h"
#include "vectors.h"

// TODO: Get Rid of this
#include "../../PhysicsBookDraftCode/Physics/Camera.h"

class SampleApplication : public GLWindow {
protected:
	vec2 cameraPos;
	float cameraDist;
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
