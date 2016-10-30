#ifndef _H_GL_WINDOW_
#define _H_GL_WINDOW_

#include "IWindow.h"
#include "matrices.h"

class GLWindow : public IWindow {
protected:
	float m_nFOV;
	float m_nNear;
	float m_nFar;
	float m_nLeft;
	float m_nRight;
	float m_nTop;
	float m_nBottom;
	vec2 m_vecMousePos;
	unsigned int mouseButtonState;
	bool keyboardState[256];
	mat4 matView;
	mat4 matProj;
	int matMode; // 0 - Perspective, 1 - Ortho, 2 - User
public:
	GLWindow(const char* title, int width, int height);
	virtual ~GLWindow() { }

	void SetPerspective(float fov, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetClearColor(float r, float g, float b);

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
	void SetProjection(const mat4& proj);
	mat4 GetProjection();

	vec2 GetMousePosition();
	bool MouseButonDown(int button);
	bool KeyDown(int key);
};

#endif
