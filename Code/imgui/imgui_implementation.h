#ifndef _H_IMGUI_IMPLEMENTATION_
#define _H_IMGUI_IMPLEMENTATION_

#define WIN32_LEAN_AND_MEAN 
#define WIN32_EXTRA_LEAN
#include <windows.h>

bool ImGui_Implementation_Init(void* hwnd);
void ImGui_Implementation_Shutdown();
bool ImGui_Implementation_CreateDeviceObjects();
void ImGui_Implementation_InvalidateDeviceObjects();
void ImGui_Implementation_RenderDrawLists(struct ImDrawData* draw_data);
void ImGui_Implementation_NewFrame();
LRESULT ImGui_Implementation_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
