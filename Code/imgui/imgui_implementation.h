#ifndef _H_IMGUI_IMPLEMENTATION_
#define _H_IMGUI_IMPLEMENTATION_

#define WIN32_LEAN_AND_MEAN 
#define WIN32_EXTRA_LEAN
#include <windows.h>

// Only the WinMain file should have access to this
// Implementation of this file will probably be per platform

bool ImGui_Implementation_Init(void* hwnd);
void ImGui_Implementation_Shutdown();
bool ImGui_Implementation_CreateDeviceObjects();
void ImGui_Implementation_InvalidateDeviceObjects();
void ImGui_Implementation_RenderDrawLists(struct ImDrawData* draw_data);
void ImGui_Implementation_NewFrame();
// Will need to wrap this function in a #define for the platform being used
void ImGui_Implementation_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool ImGui_Implementation_HasMouse();
bool ImGui_Implementation_HasKeyboard();

#endif
