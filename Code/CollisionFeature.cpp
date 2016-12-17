#include "CollisionFeature.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>

void CollisionFeature::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	manipulator = manTranslation.asArray;
	manipulating = -1;
	transformWorld = false;

	obb[0].position = vec3(4, 2, 0);
	obb[1].position = vec3(-4, -2, 0);
	//obb[1].orientation = Rotation3x3(30.0f, 20.0f, 0.0f);
	
	glPointSize(5.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);
}

void CollisionFeature::ImGUI() {
	DemoBase::ImGUI();

	ImGui::Begin("SAT Test Demo", 0, ImGuiWindowFlags_NoResize);
	if (ImGui::RadioButton("Translate", manipulator == manTranslation.asArray)) {
		manipulator = manTranslation.asArray;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", manipulator == manRotation.asArray)) {
		manipulator = manRotation.asArray;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", manipulator == manScale.asArray)) {
		manipulator = manScale.asArray;
	}

	if (ImGui::RadioButton("Local", transformWorld == false)) {
		transformWorld = false;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("World", transformWorld == true)) {
		transformWorld = true;
	}
	ImGui::End();

	if (manipulating != -1) {
		ImGuizmo::MODE currentGizmoMode = transformWorld ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
		ImGuizmo::OPERATION currentGizmoOperation = ImGuizmo::ROTATE;
		
		if (manipulator == manTranslation.asArray) {
			currentGizmoOperation = ImGuizmo::TRANSLATE;
			ImGuizmo::Enable(true);
		}
		if (manipulator == manRotation.asArray) {
			currentGizmoOperation = ImGuizmo::ROTATE;
			ImGuizmo::Enable(manipulating == 1);
		}
		if (manipulator == manScale.asArray) {
			currentGizmoOperation = ImGuizmo::SCALE;
			ImGuizmo::Enable(manipulating == 0 || manipulating == 1);
		}

		ImGuizmo::Manipulate(
			camera.GetViewMatrix().asArray,
			camera.GetProjectionMatrix().asArray, 
			currentGizmoOperation, currentGizmoMode, manipulator);
		
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(manipulator, matrixTranslation, matrixRotation, matrixScale);

		if (manipulating == 0 || manipulating == 1) {
			if (manipulator == manTranslation.asArray) {
				obb[manipulating].position.x = matrixTranslation[0];
				obb[manipulating].position.y = matrixTranslation[1];
				obb[manipulating].position.z = matrixTranslation[2];
			}
			else if (manipulator == manScale.asArray) {
				obb[manipulating].size.x = matrixScale[0];
				obb[manipulating].size.y = matrixScale[1];
				obb[manipulating].size.z = matrixScale[2];
			}
			else if (manipulator == manRotation.asArray) {
				obb[manipulating].orientation = Cut(manRotation, 3, 3);
			}
		}
	}
}

void CollisionFeature::Render() {
	DemoBase::Render();

	static const float position[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	static const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const float ambient[] = { 0.0f, 0.0f, 0.5f, 0.0f };
	static const float diffuse[] = { 0.0f, 0.0f, 0.5f, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);

	CollisionResult r1 = CollisionFeatures(obb[0], obb[1]);

	if (!r1.colliding) {
		::Render(obb[0]);
	}
	if (!r1.colliding) {
		::Render(obb[1]);
	}

	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 0.0f, 1.0f);
	if (r1.colliding) {
		::Render(GetEdges(obb[0]));
	}
	if (r1.colliding) {
		::Render(GetEdges(obb[1]));
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	if (r1.colliding) {
		for (int i = 0; i < r1.contacts.size(); ++i) {
			::Render(r1.contacts[i]);
		}
	}
	glEnable(GL_LIGHTING);
}

void CollisionFeature::Update(float dt) {
	DemoBase::Update(dt);

	if (mouseLeftDown) {
		Ray screenRay = GetPickRay(mousePos, vec2(), size, camera.GetViewMatrix(), camera.GetProjectionMatrix());
		screenRay.NormalizeDirection();

		std::vector<RaycastResult> allCasts;
		allCasts.resize(2);
		Raycast(obb[0], screenRay, &allCasts[0]);
		Raycast(obb[1], screenRay, &allCasts[1]);

		RaycastResult raycastResult;
		ResetRaycastResult(&raycastResult);
		int wasManipulating = manipulating;

		for (int i = 0; i < allCasts.size(); ++i) {
			if (allCasts[i].hit) {
				if (!raycastResult.hit || allCasts[i].t < raycastResult.t) {
					raycastResult = allCasts[i];
					manipulating = i;
				}
			}
		}

		bool trans = manipulator == manTranslation.asArray;
		bool rot = manipulator == manRotation.asArray;
		bool scal = manipulator == manScale.asArray;

		if (!raycastResult.hit) {
			manipulating = -1;
		}
		else if (manipulating < 2 && wasManipulating != manipulating) {
			manTranslation = Translation(obb[manipulating].position.x, obb[manipulating].position.y, obb[manipulating].position.z);
			manRotation = FromMat3(obb[manipulating].orientation) * manTranslation;
			manTranslation = manRotation;
			manScale = Scale(obb[manipulating].size.x, obb[manipulating].size.y, obb[manipulating].size.z) * manTranslation;
		}
		

		if (trans) {
			manipulator = manTranslation.asArray;
		}
		else if (rot) {
			manipulator = manRotation.asArray;
		}
		else if (scal) {
			manipulator = manScale.asArray;
		}
	}
}