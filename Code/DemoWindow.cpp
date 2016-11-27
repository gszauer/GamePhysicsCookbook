#include "DemoWindow.h"
#include "imgui/imgui.h"

// This is the global instance of the IWindow singleton!
static DemoWindow g_WindowInstance("Physics Sandbox", 800, 600);

void DemoWindow::OnInitialize() {
	GLWindow::OnInitialize();

	m_prevMousePos = vec2(0, 0);
	m_selectedDemo = -1;
	m_pDemo = 0;
	imgui_init = true;
}

void DemoWindow::OnResize(int width, int height) {
	GLWindow::OnResize(width, height);
}

void DemoWindow::OnRender() {
	GLWindow::OnRender();

	if (m_pDemo != 0) {
		m_pDemo->Render();
	}
}

void DemoWindow::OnUpdate(float deltaTime) {
	GLWindow::OnUpdate(deltaTime);

	if (imgui_init) {
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		imgui_init = false;
	}
	ImGui::SetNextWindowSize(ImVec2(370, 75));
	ImGui::Begin("Physics Demo", 0, ImGuiWindowFlags_NoResize);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (m_selectedDemo == 0) {
		if (ImGui::Button("Stop Chapter 14")) {
			m_selectedDemo = -1;
			StopDemo();
		}
	}
	else {
		if (ImGui::Button(" Run Chapter 14")) {
			m_selectedDemo = 0;
			Start14();
		}
	}
	ImGui::SameLine();
	if (m_selectedDemo == 1) {
		if (ImGui::Button("Stop Chapter 15")) {
			m_selectedDemo = -1;
			StopDemo();
		}
	}
	else {
		if (ImGui::Button(" Run Chapter 15")) {
			m_selectedDemo = 1;
			Start15();
		}
	}
	ImGui::SameLine();
	if (m_selectedDemo == 2) {
		if (ImGui::Button("Stop Chapter 16")) {
			m_selectedDemo = -1;
			StopDemo();
		}
	}
	else {
		if (ImGui::Button(" Run Chapter 16")) {
			m_selectedDemo = 2;
			Start16();
		}
	}

	ImGui::End();

	if (m_pDemo != 0) {
		m_pDemo->ImGUI();
		m_pDemo->Update(deltaTime);
	}
}

void DemoWindow::OnShutdown() {
	GLWindow::OnShutdown();
	StopDemo();
}

void DemoWindow::StopDemo() {
	if (m_pDemo != 0) {
		m_pDemo->Shutdown();
	}
	m_pDemo = 0;
}

void DemoWindow::Start14() {
	StopDemo();
	// TODO
}

void DemoWindow::Start15() {
	StopDemo();
	// TODO
}

void DemoWindow::Start16() {
	StopDemo();
	// TODO
}