#ifndef _H_GL_WINDOW_
#define _H_GL_WINDOW_

#include "IWindow.h"
#include "matrices.h"

class GLWindow : public IWindow {
protected:
	float m_nFOV;
	float m_nNear;
	float m_nFar;
	bool m_bDrawOrigin;
	vec2 m_vecMousePos;
	unsigned int mouseButtonState;
	bool keyboardState[256];
	mat4 matView;
public:
	GLWindow(const char* title, int width, int height);
	virtual ~GLWindow() { }

	void SetPerspective(float fov, float zNear, float zFar);
	void SetClearColor(float r, float g, float b);
	void SetDrawOrigin(bool value);

	virtual void OnInitialize();
	virtual void OnRender();
	virtual void OnResize(int width, int height);

	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseDown(int mouseCode);
	virtual void OnMouseUp(int mouseCode);

	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);

	void SetView(const mat4& view);
	mat4 GetView();

	vec2 GetMousePosition();
	bool MouseButonDown(int button);
	bool KeyDown(int key);
};

#endif
