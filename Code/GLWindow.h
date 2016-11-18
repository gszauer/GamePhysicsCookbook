#ifndef _H_GL_WINDOW_
#define _H_GL_WINDOW_

#include "IWindow.h"
#include "matrices.h"

// TODO: Re-Design whole system to be OpenGL 2.1 specific!
// This will eliminate GLEW as a dependency and make porting
// to other platforms MUCH easyer. Should also help with
// IMGUI window implementation!

// TODO: Get Rid of this
#include "../../PhysicsBookDraftCode/Physics/Camera.h"


class GLWindow : public IWindow {
protected:
	vec2 m_vecMousePos;
	unsigned int mouseButtonState;
	bool keyboardState[256];
public:
	Camera camera;

	GLWindow(const char* title, int width, int height);
	virtual ~GLWindow() { }

	void SetPerspective(float fov, float zNear, float zFar);
	void SetOrtho(float width, float height, float zNear, float zFar);
	void SetClearColor(float r, float g, float b);

	virtual void OnInitialize();
	virtual void OnRender();
	virtual void OnResize(int width, int height);

	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseDown(int mouseCode);
	virtual void OnMouseUp(int mouseCode);

	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);

	mat4 GetView();
	void SetProjection(const mat4& proj);
	mat4 GetProjection();

	vec2 GetMousePosition();
	bool MouseButonDown(int button);
	bool KeyDown(int key);
};

#endif
