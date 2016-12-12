#include <iostream>
#include <cstring>
#include "IWindow.h"

IWindow* IWindow::g_pSingleton = 0;

#if ENABLE_USER_DATA
	#define I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH 5 
	#define I_WINDOW_CPP_GENERIC_EXPAND_BY 10
	#define I_WINDOW_CPP_DEFAULTS(w, h) \
		m_bQuitFlag(false), m_bFullscreenFlag(false), m_nWidth(w), m_nHeight(h), \
		m_szTitle(0), m_bTitleDirty(false), m_vGenericIntegerValues(0), \
		m_vGenericIntegerNames(0),m_nGenericIntegerCount(0), m_nGenericIntegerLength(0), \
		m_nTargetFPS(30), m_nFixedFPS(30), b_isVisible(false)
#else 
	#define I_WINDOW_CPP_DEFAULTS(w, h) \
		m_bQuitFlag(false), m_bFullscreenFlag(false), m_nWidth(w), m_nHeight(h), \
		m_szTitle(0), m_bTitleDirty(false), m_nTargetFPS(30), m_nFixedFPS(30), b_isVisible(false)
#endif

IWindow::IWindow() : I_WINDOW_CPP_DEFAULTS(800, 600) {
	if (g_pSingleton != 0) {
		std::cout << "Error, two instances of the window interface exist!\n";
		m_bQuitFlag = true;
	}

	SetTitle("Platform Window");
	m_bTitleDirty = true;

#if ENABLE_USER_DATA
	InitGenericIntegers();
#endif

	g_pSingleton = this;
};

IWindow::IWindow(const char* title) : I_WINDOW_CPP_DEFAULTS(800, 600) {
	if (g_pSingleton != 0) {
		std::cout << "Error, two instances of the window interface exist!\n";
		m_bQuitFlag = true;
	}

	SetTitle(title);
	m_bTitleDirty = true;

#if ENABLE_USER_DATA
	InitGenericIntegers();
#endif

	g_pSingleton = this;
}

IWindow::IWindow(const char* title, int width, int height) : I_WINDOW_CPP_DEFAULTS(width, height) {
	if (g_pSingleton != 0) {
		std::cout << "Error, two instances of the window interface exist!\n";
		m_bQuitFlag = true;
	}

	SetTitle(title);
	m_bTitleDirty = false;

#if ENABLE_USER_DATA
	InitGenericIntegers();
#endif

	g_pSingleton = this;
}

void IWindow::SetTitle(const char* title) {
	if (m_szTitle != 0) {
		free(m_szTitle);
	}

	int len = strlen(title);
	m_szTitle = (char*)malloc(sizeof(char) * len + 1);
	strcpy(m_szTitle, title);
	m_szTitle[len] = '\0';

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

void CleanupMemory(IWindow* window) {
	if (window->m_szTitle != 0) {
		free(window->m_szTitle);
	}
	window->m_szTitle = 0;

#if ENABLE_USER_DATA
	if (window->m_vGenericIntegerValues != 0) {
		free(window->m_vGenericIntegerValues);
	}
	window->m_vGenericIntegerValues = 0;

	if (window->m_vGenericIntegerNames != 0) {
		for (int i = 0; i < window->m_nGenericIntegerCount; ++i) {
			free(window->m_vGenericIntegerNames[i]);
			window->m_vGenericIntegerNames[i] = 0;
		}
		free(window->m_vGenericIntegerNames);
	}
	window->m_vGenericIntegerNames = 0;
#endif
}

IWindow::~IWindow() {
	CleanupMemory(this);
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

#if ENABLE_USER_DATA
void IWindow::InitGenericIntegers() {
	m_nGenericIntegerCount = 0;
	m_nGenericIntegerLength = I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH;

	m_vGenericIntegerValues = (int*)malloc(sizeof(int) * I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH);
	memset(m_vGenericIntegerValues, 0, sizeof(int) * I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH);
	
	m_vGenericIntegerNames = (char**)malloc(sizeof(char*) * I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH);
	memset(m_vGenericIntegerNames, 0, sizeof(char*) * I_WINDOW_CPP_GENERIC_DEFAULT_LENGTH);
}

void IWindow::SetInt(const char* name, int value) {
	if (m_vGenericIntegerNames == 0) {
		InitGenericIntegers();
	}

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
		for (int i = 0; i < m_nGenericIntegerCount; ++i) {
			free(oldIntegerNames[i]);
		}
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
	if (m_vGenericIntegerNames == 0) {
		return default;
	}

	for (int i = 0; i < m_nGenericIntegerCount; ++i) {
		if (strcmp(m_vGenericIntegerNames[i], name) == 0) {
			return m_vGenericIntegerValues[i];
		}
	}
	return default;
}

bool IWindow::HasInt(const char* name) {
	if (m_vGenericIntegerNames == 0) {
		return false;
	}

	for (int i = 0; i < m_nGenericIntegerCount; ++i) {
		if (strcmp(m_vGenericIntegerNames[i], name) == 0) {
			return true;
		}
	}
	return false;
}
#endif

void IWindow::SetTargetFPS(int target) {
	m_nTargetFPS = target;
	if (m_nTargetFPS < 15) {
		m_nTargetFPS = 15;
	}
	if (m_nTargetFPS > 120) {
		m_nTargetFPS = 120;
	}
}

int IWindow::GetFixedFPS() {
	return m_nFixedFPS;
}

void IWindow::MarkAsShown() {
	b_isVisible = true;
}

bool IWindow::WasWindowShown() {
	return b_isVisible;
}

int IWindow::GetTargetFPS() {
	return m_nTargetFPS;
}

int KeyIndex(int keyCode) {
	switch (keyCode) {
		case KEY_NONE:              return 0;
		case KEY_PAUSE:             return 1;
		case KEY_SCROLL_LOCK:       return 2;
		case KEY_PRINT:             return 3;
		case KEY_SYSREQ:            return 4;
		case KEY_BREAK:             return 5;
		case KEY_ESCAPE:            return 6;
		case KEY_BACKSPACE:         return 7;
		case KEY_TAB:               return 8;
		case KEY_BACK_TAB:          return 9;
		case KEY_RETURN:            return 10;
		case KEY_CAPS_LOCK:         return 11;
		case KEY_SHIFT:             return 12;
		case KEY_CTRL:              return 13;
		case KEY_ALT:               return 14;
		case KEY_MENU:              return 15;
		case KEY_HYPER:             return 16;
		case KEY_INSERT:            return 17;
		case KEY_HOME:              return 18;
		case KEY_PG_UP:             return 19;
		case KEY_DELETE:            return 20;
		case KEY_END:               return 21;
		case KEY_PG_DOWN:           return 22;
		case KEY_LEFT_ARROW:        return 23;
		case KEY_RIGHT_ARROW:       return 24;
		case KEY_UP_ARROW:          return 25;
		case KEY_DOWN_ARROW:        return 26;
		case KEY_NUM_LOCK:          return 27;
		case KEY_KP_PLUS:           return 28;
		case KEY_KP_MINUS:          return 29;
		case KEY_KP_MULTIPLY:       return 30;
		case KEY_KP_DIVIDE:         return 31;
		case KEY_KP_ENTER:          return 32;
		case KEY_KP_HOME:           return 33;
		case KEY_KP_UP:             return 34;
		case KEY_KP_PG_UP:          return 35;
		case KEY_KP_LEFT:           return 36;
		case KEY_KP_FIVE:           return 37;
		case KEY_KP_RIGHT:          return 38;
		case KEY_KP_END:            return 39;
		case KEY_KP_DOWN:           return 40;
		case KEY_KP_PG_DOWN:        return 41;
		case KEY_KP_INSERT:         return 42;
		case KEY_KP_DELETE:         return 43;
		case KEY_F1:                return 44;
		case KEY_F2:                return 45;
		case KEY_F3:                return 46;
		case KEY_F4:                return 47;
		case KEY_F5:                return 48;
		case KEY_F6:                return 49;
		case KEY_F7:                return 50;
		case KEY_F8:                return 51;
		case KEY_F9:                return 52;
		case KEY_F10:               return 53;
		case KEY_F11:               return 54;
		case KEY_F12:               return 55;
		case KEY_SPACE:             return 56;
		case KEY_EXCLAM:            return 57;
		case KEY_QUOTE:             return 58;
		case KEY_NUMBER:            return 59;
		case KEY_DOLLAR:            return 60;
		case KEY_PERCENT:           return 61;
		case KEY_CIRCUMFLEX:        return 62;
		case KEY_AMPERSAND:         return 63;
		case KEY_APOSTROPHE:        return 64;
		case KEY_LEFT_PARENTHESIS:  return 65;
		case KEY_RIGHT_PARENTHESIS: return 66;
		case KEY_ASTERISK:          return 67;
		case KEY_PLUS:              return 68;
		case KEY_COMMA:             return 69;
		case KEY_MINUS:             return 70;
		case KEY_PERIOD:            return 71;
		case KEY_SLASH:             return 72;
		case KEY_ZERO:              return 73;
		case KEY_ONE:               return 74;
		case KEY_TWO:               return 75;
		case KEY_THREE:             return 76;
		case KEY_FOUR:              return 77;
		case KEY_FIVE:              return 78;
		case KEY_SIX:               return 79;
		case KEY_SEVEN:             return 80;
		case KEY_EIGHT:             return 81;
		case KEY_NINE:              return 82;
		case KEY_COLON:             return 83;
		case KEY_SEMICOLON:         return 84;
		case KEY_LESS_THAN:         return 85;
		case KEY_EQUAL:             return 86;
		case KEY_GREATER_THAN:      return 87;
		case KEY_QUESTION:          return 88;
		case KEY_AT:                return 89;
		case KEY_CAPITAL_A:         return 90;
		case KEY_CAPITAL_B:         return 91;
		case KEY_CAPITAL_C:         return 92;
		case KEY_CAPITAL_D:         return 93;
		case KEY_CAPITAL_E:         return 94;
		case KEY_CAPITAL_F:         return 95;
		case KEY_CAPITAL_G:         return 96;
		case KEY_CAPITAL_H:         return 97;
		case KEY_CAPITAL_I:         return 98;
		case KEY_CAPITAL_J:         return 99;
		case KEY_CAPITAL_K:         return 100;
		case KEY_CAPITAL_L:         return 101;
		case KEY_CAPITAL_M:         return 102;
		case KEY_CAPITAL_N:         return 103;
		case KEY_CAPITAL_O:         return 104;
		case KEY_CAPITAL_P:         return 105;
		case KEY_CAPITAL_Q:         return 106;
		case KEY_CAPITAL_R:         return 107;
		case KEY_CAPITAL_S:         return 108;
		case KEY_CAPITAL_T:         return 109;
		case KEY_CAPITAL_U:         return 110;
		case KEY_CAPITAL_V:         return 111;
		case KEY_CAPITAL_W:         return 112;
		case KEY_CAPITAL_X:         return 113;
		case KEY_CAPITAL_Y:         return 114;
		case KEY_CAPITAL_Z:         return 115;
		case KEY_LEFT_BRACKET:      return 116;
		case KEY_BACK_SLASH:        return 117;
		case KEY_RIGHT_BRACKET:     return 118;
		case KEY_UNDERSCORE:        return 119;
		case KEY_GRAVE:             return 120;
		case KEY_A:                 return 121;
		case KEY_B:                 return 122;
		case KEY_C:                 return 123;
		case KEY_D:                 return 124;
		case KEY_E:                 return 125;
		case KEY_F:                 return 126;
		case KEY_G:                 return 127;
		case KEY_H:                 return 128;
		case KEY_I:                 return 129;
		case KEY_J:                 return 130;
		case KEY_K:                 return 131;
		case KEY_L:                 return 132;
		case KEY_M:                 return 133;
		case KEY_N:                 return 134;
		case KEY_O:                 return 135;
		case KEY_P:                 return 136;
		case KEY_Q:                 return 137;
		case KEY_R:                 return 138;
		case KEY_S:                 return 139;
		case KEY_T:                 return 140;
		case KEY_U:                 return 141;
		case KEY_V:                 return 142;
		case KEY_W:                 return 143;
		case KEY_X:                 return 144;
		case KEY_Y:                 return 145;
		case KEY_Z:                 return 146;
		case KEY_LEFT_BRACE:        return 147;
		case KEY_BAR:               return 148;
		case KEY_RIGHT_BRACE:       return 149;
		case KEY_TILDE:             return 150;
		case KEY_EURO:              return 151;
		case KEY_POUND:             return 152;
		case KEY_YEN:               return 153;
		case KEY_MIDDLE_DOT:        return 154;
		case KEY_SEARCH:            return 155;
	}
	return 0;
}