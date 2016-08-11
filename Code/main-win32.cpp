// Undef this if you don't want to use GLEW. 
// Only GL 1.1 will be usable without it!
#define USE_GLEW

// If you don't want a console window define this

// Only change if you know what you are doing
#define MAIN_WIN_32_WINDOW_CLASS L"Win32GLSandbox"
#define ENABLE_RESIZE

// Should not need to edit below this line
// -------------------------------------------------- \\

#define WIN32_LEAN_AND_MEAN 
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>

#ifdef USE_GLEW
#include <GL\glew.h>
#include <GL\wglew.h>
#endif
#include <gl\GL.h>

#include <string>
#include <iostream>
#include <cstdio>
#include "IWindow.h"

std::wstring MAIN_WIN32_WINDOW_NAME;
RECT windowRect;
RECT clientRect;
RECT borderRect;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow);
bool CheckIfAlreadyRunning();
HGLRC OpenGLBindContext(HDC hdc);
void OpenGLUnbindContext(HWND hwnd, HDC hdc, HGLRC hglrc);
void UpdateFullscreen(IWindow* pWindowInstance, HWND hwnd, HDC hdc);
void SetDisplayMode(int width, int height, int bpp, int refreshRate);

#define NO_RESIZE_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#define NORMAL_STYLE (WS_VISIBLE | WS_OVERLAPPEDWINDOW)

static IWindow debugInstance; // TODO: Remove this
void win32DebugRender(HWND hwnd); // TODO: Also remove this

// TODO: How do i turn on CRT memleak detection?!?!?!

// https://www.opengl.org/archives/resources/code/samples/win32_tutorial/
// https://msdn.microsoft.com/en-us/library/bb384843.aspx?f=255&MSPPError=-2147217396
// https://gist.github.com/gszauer/5718416
// http://glew.sourceforge.net/basic.html

int main(int argc, const char** argv) {
	IWindow* pWindowInstance = IWindow::GetInstance();

	pWindowInstance->GetAndResetTitleDirtyFlag();
	std::string title(pWindowInstance->GetTitle());
	MAIN_WIN32_WINDOW_NAME = std::wstring(title.begin(), title.end());

	if (CheckIfAlreadyRunning()) {
		std::cout << "Error, only one instance of application is allowed to run\n";
	}
	else {
		WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	IWindow* pWindowInstance = IWindow::GetInstance();
	{ // In case we start with a non-console linker
		pWindowInstance->GetAndResetTitleDirtyFlag();
		std::string title(pWindowInstance->GetTitle());
		MAIN_WIN32_WINDOW_NAME = std::wstring(title.begin(), title.end());

		if (CheckIfAlreadyRunning()) {
			MessageBox(NULL, L"Only one instance of application is allowed to run", NULL, NULL);
			return FALSE;
		}
	}
	pWindowInstance->SetInt("glMajor", 2);
	pWindowInstance->SetInt("glMinor", 1);

	int width = pWindowInstance->GetWidth();
	int height = pWindowInstance->GetHeight();

	HINSTANCE hinstance = hInstance;

	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = MAIN_WIN_32_WINDOW_CLASS;
	RegisterClassEx(&wndclass);

	SetRect(&windowRect, (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2), 
						 (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2), 
						 (GetSystemMetrics(SM_CXSCREEN) / 2) + (width / 2), 
						 (GetSystemMetrics(SM_CYSCREEN) / 2) + (height / 2));
	AdjustWindowRectEx(&windowRect, 
#ifdef ENABLE_RESIZE
		NORMAL_STYLE
#else
		NO_RESIZE_STYLE
#endif
		, FALSE, 0);

	HWND hwnd = CreateWindowEx(0, MAIN_WIN_32_WINDOW_CLASS, MAIN_WIN32_WINDOW_NAME.c_str(),
#ifdef ENABLE_RESIZE
		NORMAL_STYLE
#else
		NO_RESIZE_STYLE
#endif
		, windowRect.left, windowRect.top,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 
		NULL, NULL, hInstance, szCmdLine);

	HDC hdc = GetDC(hwnd);
	HGLRC hglrc = OpenGLBindContext(hdc);

#ifdef USE_GLEW
	if (pWindowInstance->HasInt("glMajor")) {
		if (pWindowInstance->HasInt("glMinor")) {
			int major = pWindowInstance->GetInt("glMajor", 1);
			int minor = pWindowInstance->GetInt("glMinor", 1);

			std::cout << "Requesting OpenGL " << major << "." << minor << " context\n";

			GLenum err = glewInit();
			if (GLEW_OK != err) {
				std::cout << "Error initializing glew: " << glewGetErrorString(err) << "\n";
			}
			else {
				char version_string[256];
				memset(version_string, 0, sizeof(char) * 256);
				sprintf(version_string, "GL_VERSION_%d_%d", major, minor);

				if (!glewIsSupported(version_string)) {
					std::cout << "GLEW, Requested version is not supported: " << version_string << "\n";
				}
				else {
					int attribs[] =
					{
						WGL_CONTEXT_MAJOR_VERSION_ARB, major,
						WGL_CONTEXT_MINOR_VERSION_ARB, minor,
						WGL_CONTEXT_FLAGS_ARB, 0,
						0
					};

					// Needed to create 3X or higher context
					if (wglewIsSupported("WGL_ARB_create_context") == 1) {
						HGLRC glewrc = wglCreateContextAttribsARB(hdc, 0, attribs);

						wglMakeCurrent(NULL, NULL);
						wglDeleteContext(hglrc);

						wglMakeCurrent(hdc, glewrc);
						hglrc = glewrc;
					}
					else {
						std::cout << "WGL_ARB_create_context is not supported\n";
					}
				}
			}
		}
	}

	glGetError(); // Clear error flag
#endif
	int OpenGLVersion[2] = { 1, 1 };
#ifdef USE_GLEW
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	if (glGetError() == GL_INVALID_ENUM) { // Only works with 3X
#endif
		// Provide a 2x & below fallback!
		char major_string[2] = { glGetString(GL_VERSION)[0], '\0' };
		OpenGLVersion[0] = atoi(major_string);
		char minor_string[2] = { glGetString(GL_VERSION)[2], '\0' };
		OpenGLVersion[1] = atoi(minor_string);
#ifdef USE_GLEW

	}
	else {
		glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	}
#endif
	pWindowInstance->SetInt("glMajor", OpenGLVersion[0]);
	pWindowInstance->SetInt("glMinor", OpenGLVersion[1]);
	std::cout << "OpenGL context: " << OpenGLVersion[0] << ", " << OpenGLVersion[1] << "\n";
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
#ifdef USE_GLEW
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
#endif
	std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << "\n";

	pWindowInstance->OnInitialize();

	bool fullscreen = pWindowInstance->GetFullScreen();
	if (fullscreen) {
		UpdateFullscreen(pWindowInstance, hwnd, hdc);
	}
	else {
		pWindowInstance->OnResize(width, height);
	}
	
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	GetClientRect(hwnd, &clientRect);
	GetWindowRect(hwnd, &windowRect);
	GetWindowRect(hwnd, &borderRect);

	MSG msg;
	DWORD next_game_tick = GetTickCount();
	int sleep_time = 0;

	while (!pWindowInstance->GetQuitFlag()) {
		// If or while? Not sure if all messages should process at once or not
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Regulate FPS
		int SKIP_TICKS = 1000 / pWindowInstance->GetTargetFPS();
		next_game_tick += SKIP_TICKS;
		sleep_time = next_game_tick - GetTickCount();
		if (sleep_time >= 0) {
			Sleep(sleep_time);
		}

		// Potentially change title
		if (pWindowInstance->GetAndResetTitleDirtyFlag()) {
			std::string title(pWindowInstance->GetTitle());
			MAIN_WIN32_WINDOW_NAME = std::wstring(title.begin(), title.end());
			SetWindowText(hwnd, MAIN_WIN32_WINDOW_NAME.c_str());
		}

		// Potentially toggle full-screen
		if (fullscreen != pWindowInstance->GetFullScreen()) {
			UpdateFullscreen(pWindowInstance, hwnd, hdc);
			fullscreen = pWindowInstance->GetFullScreen();
		}

		if (!fullscreen) {
			int windowWidth = borderRect.right - borderRect.left;
			int windowHeight = borderRect.bottom - borderRect.top;
			int clientWidth = clientRect.right - clientRect.left;
			int clientHeight = clientRect.bottom - clientRect.top;
			int borderWidth = windowWidth - clientWidth;
			int borderHeight = windowHeight - clientHeight;

			if (clientWidth != pWindowInstance->GetWidth() || clientHeight != pWindowInstance->GetHeight()) {
				SetWindowPos(hwnd, 0, windowRect.left, windowRect.top, pWindowInstance->GetWidth() + borderWidth, pWindowInstance->GetHeight() + borderHeight, SWP_NOZORDER);
			}
		}

		pWindowInstance->OnUpdate(0.0f);
		pWindowInstance->OnRender();
		win32DebugRender(hwnd);
		SwapBuffers(hdc);
	}

	pWindowInstance->OnShutdown();
	OpenGLUnbindContext(hwnd, hdc, hglrc);

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	IWindow* pWindowInstance = IWindow::GetInstance();
	int width, height;

	switch (iMsg) {
	// Window Lifecycle events
	case WM_CREATE:
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		pWindowInstance->Close();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	// Handle resize event
	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		pWindowInstance->OnResize(width, height);

		GetClientRect(hwnd, &clientRect);
		GetWindowRect(hwnd, &borderRect);
		pWindowInstance->Resize(width, height);

		break;
	// Handle Mouse Events
	case WM_MOUSEMOVE:
		pWindowInstance->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONDOWN:
		pWindowInstance->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		pWindowInstance->OnMouseDown(MOUSE_LEFT);
		break;
	case WM_LBUTTONUP:
		pWindowInstance->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		pWindowInstance->OnMouseUp(MOUSE_LEFT);
		break;
	case WM_RBUTTONDOWN:
		pWindowInstance->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		pWindowInstance->OnMouseDown(MOUSE_RIGHT);
		break;
	case WM_RBUTTONUP:
		pWindowInstance->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		pWindowInstance->OnMouseUp(MOUSE_RIGHT);
		break;
	case WM_MBUTTONDOWN:
		pWindowInstance->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		pWindowInstance->OnMouseDown(MOUSE_MIDDLE);
		break;
	case WM_MBUTTONUP:
		pWindowInstance->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		pWindowInstance->OnMouseUp(MOUSE_MIDDLE);
		break;
	// Handle keyboard events
	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN) {
			//if ((HIWORD(lParam) & KF_ALTDOWN)) {
				if (pWindowInstance->GetFullScreen()) {
					pWindowInstance->SetFullScreen(false);
				}
				else {
					pWindowInstance->SetFullScreen(true);
				}
			//}
		}
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

bool CheckIfAlreadyRunning() {
	HWND hWnd = FindWindow(MAIN_WIN_32_WINDOW_CLASS, MAIN_WIN32_WINDOW_NAME.c_str());

	if (hWnd) {
		if (IsIconic(hWnd)) {
			ShowWindow(hWnd, SW_RESTORE);
		}
		SetForegroundWindow(hWnd);
		return true;
	}

	return false;
}

HGLRC OpenGLBindContext(HDC hdc) {
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	HGLRC context = wglCreateContext(hdc);
	wglMakeCurrent(hdc, context);
	return context;
}

void OpenGLUnbindContext(HWND hwnd, HDC hdc, HGLRC hglrc) {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	ReleaseDC(hwnd, hdc);
}

void win32DebugRender(HWND hwnd) {
	// Debug, reset projection
	RECT window = { 0 };
	GetClientRect(hwnd, &window);
	glViewport(0, 0, window.right - window.left, window.bottom - window.top);

	float fov = 62.0f;
	float aspect = (float)(window.right - window.left) / (float)(window.bottom - window.top);

	float top = 0.01f * float(tanf(fov * 3.14159265f / 360.0f));
	float bottom = -1.0f * top;
	float right = bottom * aspect;
	float left = top * aspect;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, 0.01f, 1000.0f);

	// Debug, render
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();
}

void UpdateFullscreen(IWindow* pWindowInstance, HWND hwnd, HDC hdc) {
	if (pWindowInstance->GetFullScreen()) {
		GetWindowRect(hwnd, &windowRect);
		SetDisplayMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), GetDeviceCaps(hdc, BITSPIXEL), GetDeviceCaps(hdc, VREFRESH));
		SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		// Not 100% sure why, but setting height to GetSystemMetrics(SM_CYSCREEN) causes flickering!
		// Adding 1 to it seems to solve it. Maybe something to do with running in a scaled WM???
		SetWindowPos(hwnd, 0, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) + 1, SWP_NOZORDER);
		ShowCursor(FALSE);
	}
	else {
		SetDisplayMode(0, 0, 0, 0);
		SetWindowLongPtr(hwnd, GWL_STYLE, 
#ifdef ENABLE_RESIZE
			NORMAL_STYLE
#else
			NO_RESIZE_STYLE
#endif	
		);
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		int iWindowWidth = windowRect.right - windowRect.left;
		int iWindowHeight = windowRect.bottom - windowRect.top;
		SetWindowPos(hwnd, 0, windowRect.left, windowRect.top, iWindowWidth, iWindowHeight, SWP_NOZORDER);
		ShowCursor(TRUE);
	}

	GetClientRect(hwnd, &clientRect);
	GetWindowRect(hwnd, &borderRect);
}

void SetDisplayMode(int width, int height, int bpp, int refreshRate) {
	if (width == 0 && height == 0 && bpp == 0 && refreshRate == 0) {
		ChangeDisplaySettings(NULL, 0);
		return;
	}

	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);

	int i = 0;
	while (EnumDisplaySettings(NULL, i++, &dm)) {
		if (dm.dmPelsWidth == width && dm.dmPelsHeight == height &&
			dm.dmBitsPerPel == bpp && dm.dmDisplayFrequency == refreshRate) {
			if (ChangeDisplaySettings(&dm, CDS_TEST) == DISP_CHANGE_SUCCESSFUL) {
				ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
				return;
			}
		}
	}
}