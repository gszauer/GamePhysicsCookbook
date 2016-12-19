#include "DemoBase.h"
#include "imgui/imgui.h"
#include "FixedFunctionPrimitives.h"

DemoBase::DemoBase() {
	show_help = false;
	mouseLeftDown = false;
	mouseRightDown = false;
	mouseMiddleDown = false;
	mouseDelta = vec2(0.0f, 0.0f);
	mousePos = vec2(0.0f, 0.0f);
	size = vec2(0.0f, 0.0f);
	configHelpWindow = true;
}

void DemoBase::Initialize(int width, int height) {
	Resize(width, height);
}

void DemoBase::Resize(int width, int height) {
	camera.Perspective(60.0f, (float)width / (float)height, 0.01f, 1000.0f);
	size = vec2(width, height);
}

void DemoBase::Update(float dt) {
	if (mouseRightDown) {
		camera.Rotate(mouseDelta, dt);
	}
	else if (mouseMiddleDown) {
		camera.Zoom(mouseDelta.y, dt);
	}
	else if (mouseLeftDown) {
		camera.Pan(mouseDelta, dt);
	}

	// Update the camera position
	camera.Update(dt);
}

void DemoBase::ImGUI() {
	if (show_help) {
		if (configHelpWindow) {
			configHelpWindow = false;

			ImGui::SetNextWindowPos(ImVec2(400, 90));
			ImGui::SetNextWindowSize(ImVec2(370, 100));
		}
		ImGui::Begin("How to use", &show_help, ImGuiWindowFlags_NoResize);
		ImGui::Text("A 3 button mouse is needed to navigate the scene");
		ImGui::Text("Press the left mouse and move the mouse to pan");
		ImGui::Text("Press the middle mouse and move the mouse to zoom");
		ImGui::Text("Press the right mouse and move the mouse to rotate");
		ImGui::End();
	}
}

void DemoBase::Render() {
	FixedFunctionOrigin();
}

void DemoBase::SetMouseState(bool left, bool middle, bool right, const vec2& delta, const vec2& mouse) {
	mouseLeftDown = left;
	mouseMiddleDown = middle;
	mouseRightDown = right;
	mouseDelta = delta;
	mousePos = mouse;
}