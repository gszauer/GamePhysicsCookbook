#include <iostream>
#include <cstring>
#include "IWindow.h"

IWindow* IWindow::g_pSingleton = 0;

// TODO: Change this to 10!
#define I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH 2 
#define I_WINDOW_CPP_GENERIC_EXPAND_BY 10

#define I_WINDOW_CPP_DEFAULTS(w, h) \
	m_bQuitFlag(false), m_bFullscreenFlag(false), m_nWidth(w), m_nHeight(h), \
	m_szTitle(0), m_bTitleDirty(false), m_vGenericIntegerValues(0), \
	m_vGenericIntegerNames(0),m_nGenericIntegerCount(0), m_nGenericIntegerLength(0), \
	m_nTargetFPS(30)

IWindow::IWindow() : I_WINDOW_CPP_DEFAULTS(800, 600) {
	if (g_pSingleton != 0) {
		std::cout << "Error, two instances of the window interface exist!\n";
		m_bQuitFlag = true;
	}

	SetTitle("Platform Window");
	m_bTitleDirty = true;

	InitGenericIntegers();

	g_pSingleton = this;
};

IWindow::IWindow(const char* title) : I_WINDOW_CPP_DEFAULTS(800, 600) {
	if (g_pSingleton != 0) {
		std::cout << "Error, two instances of the window interface exist!\n";
		m_bQuitFlag = true;
	}

	SetTitle(title);
	m_bTitleDirty = true;

	InitGenericIntegers();

	g_pSingleton = this;
}

IWindow::IWindow(const char* title, int width, int height) : I_WINDOW_CPP_DEFAULTS(width, height) {
	if (g_pSingleton != 0) {
		std::cout << "Error, two instances of the window interface exist!\n";
		m_bQuitFlag = true;
	}

	SetTitle(title);
	m_bTitleDirty = false;
	InitGenericIntegers();

	g_pSingleton = this;
}

void IWindow::SetTitle(const char* title) {
	if (m_szTitle != 0) {
		free(m_szTitle);
	}

	int len = strlen(title) + 1;
	m_szTitle = (char*)malloc(sizeof(char) * len);
	strcpy(m_szTitle, title);
	m_szTitle[len - 1] = '\0';

	m_bTitleDirty = true;
}

bool IWindow::GetAndResetTitleDirtyFlag() {
	bool value = m_bTitleDirty;
	m_bTitleDirty = false;
	return value;
}

const char* IWindow::GetTitle() {
	return m_szTitle;
}

IWindow::~IWindow() {
	free(m_szTitle);
	free(m_vGenericIntegerValues);
	for (int i = 0; i < m_nGenericIntegerCount; ++i) {
		free(m_vGenericIntegerNames[i]);
	}
	free(m_vGenericIntegerNames);
}

void IWindow::Close() {
	m_bQuitFlag = true;
}

bool IWindow::GetQuitFlag() {
	return m_bQuitFlag;
}

int IWindow::GetWidth() {
	return m_nWidth;
}

int IWindow::GetHeight() {
	return m_nHeight;
}

void IWindow::Resize(int width, int height) {
	m_nWidth = width;
	m_nHeight = height;
}

void IWindow::SetFullScreen(bool value) {
	m_bFullscreenFlag = value;
}

bool IWindow::GetFullScreen() {
	return m_bFullscreenFlag;
}

void IWindow::InitGenericIntegers() {
	m_nGenericIntegerCount = 0;
	m_nGenericIntegerLength = I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH;

	m_vGenericIntegerValues = (int*)malloc(sizeof(int) * I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH);
	memset(m_vGenericIntegerValues, 0, sizeof(int) * I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH);
	
	m_vGenericIntegerNames = (char**)malloc(sizeof(char*) * I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH);
	memset(m_vGenericIntegerNames, 0, sizeof(char*) * I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH);
}

void IWindow::SetInt(const char* name, int value) {
	// Update
	for (int i = 0; i < m_nGenericIntegerCount; ++i) {
		if (strcmp(m_vGenericIntegerNames[i], name) == 0) {
			m_vGenericIntegerValues[i] = value;
			return;
		}
	}

	// Expand
	if (m_nGenericIntegerCount == m_nGenericIntegerLength) {
		int newSize = m_nGenericIntegerLength + I_WINDOW_CPP_GENERIC_EXPAND_BY;

		int* oldIntegerValues = m_vGenericIntegerValues;
		char** oldIntegerNames = m_vGenericIntegerNames;

		m_vGenericIntegerValues = (int*)malloc(sizeof(int) * newSize);
		memset(m_vGenericIntegerValues, 0, sizeof(int) * newSize);
		memcpy(m_vGenericIntegerValues, oldIntegerValues, sizeof(int) * m_nGenericIntegerLength);

		m_vGenericIntegerNames = (char**)malloc(sizeof(char*) * newSize);
		memset(m_vGenericIntegerNames, 0, sizeof(char*) * newSize);
		memcpy(m_vGenericIntegerNames, oldIntegerNames, sizeof(char*) * m_nGenericIntegerLength);

		free(oldIntegerValues);
		free(oldIntegerNames);
		m_nGenericIntegerLength += I_WINDOW_CPP_GENERIC_EXPAND_BY;
	}

	// Insert
	int length = strlen(name) + 1;
	m_vGenericIntegerValues[m_nGenericIntegerCount] = value;
	m_vGenericIntegerNames[m_nGenericIntegerCount] = (char*)malloc(sizeof(char) * length);
	strcpy(m_vGenericIntegerNames[m_nGenericIntegerCount], name);
	m_vGenericIntegerNames[m_nGenericIntegerCount][length - 1] = '\0';
	m_nGenericIntegerCount += 1;
}

int IWindow::GetInt(const char* name, int default) {
	for (int i = 0; i < m_nGenericIntegerCount; ++i) {
		if (strcmp(m_vGenericIntegerNames[i], name) == 0) {
			return m_vGenericIntegerValues[i];
		}
	}
	return default;
}

bool IWindow::HasInt(const char* name) {
	for (int i = 0; i < m_nGenericIntegerCount; ++i) {
		if (strcmp(m_vGenericIntegerNames[i], name) == 0) {
			return true;
		}
	}
	return false;
}

void IWindow::SetTargetFPS(int target) {
	m_nTargetFPS = target;
	if (m_nTargetFPS < 15) {
		m_nTargetFPS = 15;
	}
	if (m_nTargetFPS > 120) {
		m_nTargetFPS = 120;
	}
}
int IWindow::GetTargetFPS() {
	return m_nTargetFPS;
}