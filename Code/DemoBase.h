#ifndef _H_DEMO_BASE_
#define _H_DEMO_BASE_

#include "Camera.h"

class DemoBase {
private: // Disable
	DemoBase(const DemoBase&);
	DemoBase& operator=(const DemoBase);
protected:
	bool show_help;
	bool mouseLeftDown;
	bool mouseRightDown;
	bool mouseMiddleDown;
	vec2 mouseDelta;
	vec2 mousePos;
	vec2 size;
	bool configHelpWindow;
public:
	OrbitCamera camera;

	DemoBase();
	inline virtual ~DemoBase() { }

	virtual void Initialize(int width, int height);
	virtual void Resize(int width, int height);
	virtual void Render();
	virtual void Update(float dt);
	virtual void ImGUI();
	inline virtual void Shutdown() { }

	void SetMouseState(bool left, bool middle, bool right, const vec2& delta, const vec2& mouse);
};

#endif 