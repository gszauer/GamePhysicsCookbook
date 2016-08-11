#ifndef _H_I_WINDOW_
#define _H_I_WINDOW_

#define MOUSE_LEFT		0
#define MOUSE_MIDDLE	1
#define MOUSE_RIGHT		2

class IWindow {
private:
	IWindow(const IWindow&) {}
	IWindow& operator=(const IWindow&) {}
	void InitGenericIntegers();
protected:
	static IWindow* g_pSingleton;
	bool m_bQuitFlag;
	bool m_bFullscreenFlag;
	int m_nWidth;
	int m_nHeight;

	char* m_szTitle;
	bool m_bTitleDirty;

	int* m_vGenericIntegerValues;
	char** m_vGenericIntegerNames;
	int m_nGenericIntegerCount; // Active
	int m_nGenericIntegerLength; // Total

	int m_nTargetFPS;
public:
	inline static IWindow* GetInstance() {
		return g_pSingleton;
	}

	IWindow();
	IWindow(const char* title);
	IWindow(const char* title, int width, int height);

	virtual ~IWindow();

	virtual void OnInitialize() { }
	virtual void OnUpdate(float deltaTime) { }
	virtual void OnRender() { }
	virtual void OnShutdown() { }

	virtual void OnResize(int width, int height) { } 
	
	virtual void OnMouseMove(int x, int y) { } 
	virtual void OnMouseDown(int mouseCode) { }
	virtual void OnMouseUp(int mouseCode) { }

	virtual void OnKeyDown(int keyCode) { } // TODO
	virtual void OnKeyUp(int keyCode) { } // TODO

	void SetTitle(const char* title);
	bool GetAndResetTitleDirtyFlag();
	const char* GetTitle();

	void SetFullScreen(bool value);
	bool GetFullScreen();
	
	void Resize(int width, int height);
	int GetWidth();
	int GetHeight();
	
	void Close();
	bool GetQuitFlag();

	void SetTargetFPS(int target);
	int GetTargetFPS();

	void SetInt(const char* name, int value);
	int GetInt(const char* name, int default);
	bool HasInt(const char* name);
};

#endif