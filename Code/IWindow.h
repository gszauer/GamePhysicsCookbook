#ifndef _H_I_WINDOW_
#define _H_I_WINDOW_

class IWindow {
	friend void CleanupMemory(IWindow* window);
private:
	IWindow(const IWindow&) {}
	IWindow& operator=(const IWindow&) {}
#if ENABLE_USER_DATA
	void InitGenericIntegers();
#endif
protected:
	static IWindow* g_pSingleton;
	bool m_bQuitFlag;
	bool m_bFullscreenFlag;
	int m_nWidth;
	int m_nHeight;
	bool b_isVisible;

	char* m_szTitle;
	bool m_bTitleDirty;

#if ENABLE_USER_DATA
	int* m_vGenericIntegerValues;
	char** m_vGenericIntegerNames;
	int m_nGenericIntegerCount; // Active
	int m_nGenericIntegerLength; // Total
#endif

	int m_nTargetFPS;
	int m_nFixedFPS;
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
	virtual void OnFixedUpdate(float fixedDeltaTime) { } 
	virtual void OnRender() { }
	virtual void OnShutdown() { }

	virtual void OnResize(int width, int height) { } 
	
	virtual void OnMouseMove(int x, int y) { } 
	virtual void OnMouseDown(int mouseCode) { }
	virtual void OnMouseUp(int mouseCode) { }

	virtual void OnKeyDown(int keyCode) { }
	virtual void OnKeyUp(int keyCode) { }

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
	int GetFixedFPS();

	void MarkAsShown();
	bool WasWindowShown();

#if ENABLE_USER_DATA
	void SetInt(const char* name, int value);
	int GetInt(const char* name, int default);
	bool HasInt(const char* name);
#endif
};

void CleanupMemory(IWindow* window);
int KeyIndex(int keyCode);

#define MOUSE_LEFT		1
#define MOUSE_MIDDLE	2
#define MOUSE_RIGHT		3

#define KEY_NONE              0
#define KEY_PAUSE             0x0013
#define KEY_SCROLL_LOCK       0x1014
#define KEY_PRINT             0x1061
#define KEY_SYSREQ            0x106A
#define KEY_BREAK             0x106B
#define KEY_ESCAPE            0x001B
#define KEY_BACKSPACE         0x0008
#define KEY_TAB               0x0009
#define KEY_BACK_TAB          0x0089
#define KEY_RETURN            0x000D
#define KEY_CAPS_LOCK         0x00E5
#define KEY_SHIFT             0x00E1
#define KEY_CTRL              0x00E3
#define KEY_ALT               0x00E9
#define KEY_MENU              0x1067
#define KEY_HYPER             0x10ED
#define KEY_INSERT            0x1063
#define KEY_HOME              0x1050
#define KEY_PG_UP             0x1055
#define KEY_DELETE            0x10FF
#define KEY_END               0x1057
#define KEY_PG_DOWN           0x1056
#define KEY_LEFT_ARROW        0x1051
#define KEY_RIGHT_ARROW       0x1053
#define KEY_UP_ARROW          0x1052
#define KEY_DOWN_ARROW        0x1054
#define KEY_NUM_LOCK          0x107F
#define KEY_KP_PLUS           0x10AB
#define KEY_KP_MINUS          0x10AD
#define KEY_KP_MULTIPLY       0x10AA
#define KEY_KP_DIVIDE         0x10AF
#define KEY_KP_ENTER          0x108D
#define KEY_KP_HOME           0x10B7
#define KEY_KP_UP             0x10B8
#define KEY_KP_PG_UP          0x10B9
#define KEY_KP_LEFT           0x10B4
#define KEY_KP_FIVE           0x10B5
#define KEY_KP_RIGHT          0x10B6
#define KEY_KP_END            0x10B1
#define KEY_KP_DOWN           0x10B2
#define KEY_KP_PG_DOWN        0x10B3
#define KEY_KP_INSERT         0x10B0
#define KEY_KP_DELETE         0x10AE
#define KEY_F1                0x00BE
#define KEY_F2                0x00BF
#define KEY_F3                0x00C0
#define KEY_F4                0x00C1
#define KEY_F5                0x00C2
#define KEY_F6                0x00C3
#define KEY_F7                0x00C4
#define KEY_F8                0x00C5
#define KEY_F9                0x00C6
#define KEY_F10               0x00C7
#define KEY_F11               0x00C8
#define KEY_F12               0x00C9
#define KEY_SPACE             ' '
#define KEY_EXCLAM            '!'
#define KEY_QUOTE             '"'
#define KEY_NUMBER            '#'
#define KEY_DOLLAR            '$'
#define KEY_PERCENT           '%'
#define KEY_CIRCUMFLEX        '^'
#define KEY_AMPERSAND         '&'
#define KEY_APOSTROPHE        '\''
#define KEY_LEFT_PARENTHESIS  '('
#define KEY_RIGHT_PARENTHESIS ')'
#define KEY_ASTERISK          '*'
#define KEY_PLUS              '+'
#define KEY_COMMA             ','
#define KEY_MINUS             '-'
#define KEY_PERIOD            '.'
#define KEY_SLASH             '/'
#define KEY_ZERO              '0'
#define KEY_ONE               '1'
#define KEY_TWO               '2'
#define KEY_THREE             '3'
#define KEY_FOUR              '4'
#define KEY_FIVE              '5'
#define KEY_SIX               '6'
#define KEY_SEVEN             '7'
#define KEY_EIGHT             '8'
#define KEY_NINE              '9'
#define KEY_0				  '0'
#define KEY_1                 '1'
#define KEY_2                 '2'
#define KEY_3				  '3'
#define KEY_4				  '4'
#define KEY_5				  '5'
#define KEY_6				  '6'
#define KEY_7				  '7'
#define KEY_8				  '8'
#define KEY_9				  '9'
#define KEY_COLON             ':'
#define KEY_SEMICOLON         ';'
#define KEY_LESS_THAN         '<'
#define KEY_EQUAL             '='
#define KEY_GREATER_THAN      '>'
#define KEY_QUESTION          '?'
#define KEY_AT                '@'
#define KEY_CAPITAL_A         'A'
#define KEY_CAPITAL_B         'B'
#define KEY_CAPITAL_C         'C'
#define KEY_CAPITAL_D         'D'
#define KEY_CAPITAL_E         'E'
#define KEY_CAPITAL_F         'F'
#define KEY_CAPITAL_G         'G'
#define KEY_CAPITAL_H         'H'
#define KEY_CAPITAL_I         'I'
#define KEY_CAPITAL_J         'J'
#define KEY_CAPITAL_K         'K'
#define KEY_CAPITAL_L         'L'
#define KEY_CAPITAL_M         'M'
#define KEY_CAPITAL_N         'N'
#define KEY_CAPITAL_O         'O'
#define KEY_CAPITAL_P         'P'
#define KEY_CAPITAL_Q         'Q'
#define KEY_CAPITAL_R         'R'
#define KEY_CAPITAL_S         'S'
#define KEY_CAPITAL_T         'T'
#define KEY_CAPITAL_U         'U'
#define KEY_CAPITAL_V         'V'
#define KEY_CAPITAL_W         'W'
#define KEY_CAPITAL_X         'X'
#define KEY_CAPITAL_Y         'Y'
#define KEY_CAPITAL_Z         'Z'
#define KEY_LEFT_BRACKET      '['
#define KEY_BACK_SLASH        '\\'
#define KEY_RIGHT_BRACKET     ']'
#define KEY_UNDERSCORE        '_'
#define KEY_GRAVE             '`'
#define KEY_A                 'a'
#define KEY_B                 'b'
#define KEY_C                 'c'
#define KEY_D                 'd'
#define KEY_E                 'e'
#define KEY_F                 'f'
#define KEY_G                 'g'
#define KEY_H                 'h'
#define KEY_I                 'i'
#define KEY_J                 'j'
#define KEY_K                 'k'
#define KEY_L                 'l'
#define KEY_M                 'm'
#define KEY_N                 'n'
#define KEY_O                 'o'
#define KEY_P                 'p'
#define KEY_Q                 'q'
#define KEY_R                 'r'
#define KEY_S                 's'
#define KEY_T                 't'
#define KEY_U                 'u'
#define KEY_V                 'v'
#define KEY_W                 'w'
#define KEY_X                 'x'
#define KEY_Y                 'y'
#define KEY_Z                 'z'
#define KEY_LEFT_BRACE        '{'
#define KEY_BAR               '|'
#define KEY_RIGHT_BRACE       '}'
#define KEY_TILDE             '~'
#define KEY_EURO              0x20AC
#define KEY_POUND             0x00A3
#define KEY_YEN               0x00A5
#define KEY_MIDDLE_DOT        0x0095
#define KEY_SEARCH            0xFFAA

#endif