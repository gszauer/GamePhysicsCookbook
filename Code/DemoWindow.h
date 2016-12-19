#ifndef _H_DEMO_WINDOW_
#define _H_DEMO_WINDOW_

#include "GLWindow.h"
#include "vectors.h"
#include "DemoBase.h"

class DemoWindow : public GLWindow {
protected:
	vec2 m_prevMousePos;
	int m_selectedDemo; // -1: None, 0: CH14, 1: CH15, 2: CH16
	DemoBase* m_pDemo;
	bool imgui_init;

	bool select_all;
protected:
	void ApplyDemoCamera();
public:

	DemoWindow(const char* title, int width, int height)
		: GLWindow(title, width, height), select_all(true) {
		m_nFixedFPS = 30.0f;
		m_nTargetFPS = 60.0f;
	}
	~DemoWindow();

	virtual void OnRender();
	virtual void OnResize(int width, int height);
	virtual void OnInitialize();
	virtual void OnFixedUpdate(float deltaTime);
	virtual void OnUpdate(float deltaTime);
	virtual void OnShutdown();

	void StopDemo();
	void Start14();
	void Start15();
	void Start16();
};

// Static instance defined in .cpp file!

#endif 