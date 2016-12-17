#include "GenericSAT.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>

void GenericSAT::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	manipulator = manTranslation.asArray;
	manipulating = -1;
	transformWorld = false;

	//sphere.position = vec3(-4, 2, 0);
	aabb.position = vec3(4, 2, 0);
	
	obb.position = vec3(-4, -2, 0);
	//obb.orientation = Rotation3x3(30.0f, 20.0f, 0.0f);
	
	triangle.a = vec3(-1, 1, 0);
	triangle.b = vec3(0, 3, 0);
	triangle.c = vec3(1, 1, 0);

	vec3 centroid = Centroid(triangle);
	centroidOffset.a = centroid - triangle.a;
	centroidOffset.b = centroid - triangle.b;
	centroidOffset.c = centroid - triangle.c;

	glPointSize(5.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);
}

void GenericSAT::ImGUI() {
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

		if (manipulating == 0) {
			if (manipulator == manTranslation.asArray) {
				aabb.position.x = matrixTranslation[0];
				aabb.position.y = matrixTranslation[1];
				aabb.position.z = matrixTranslation[2];
			}
			else if (manipulator == manScale.asArray) {
				aabb.size.x = matrixScale[0];
				aabb.size.y = matrixScale[1];
				aabb.size.z = matrixScale[2];
			}
		}
		else if (manipulating == 1) {
			if (manipulator == manTranslation.asArray) {
				obb.position.x = matrixTranslation[0];
				obb.position.y = matrixTranslation[1];
				obb.position.z = matrixTranslation[2];
			}
			else if (manipulator == manScale.asArray) {
				obb.size.x = matrixScale[0];
				obb.size.y = matrixScale[1];
				obb.size.z = matrixScale[2];
			}
			else if (manipulator == manRotation.asArray) {
				obb.orientation = Cut(manRotation, 3, 3);
			}
		}
		else if (manipulating == 2) {
			if (manipulator == manTranslation.asArray) {
				vec3 centroid = Centroid(triangle);
				
				centroid.x = matrixTranslation[0];
				centroid.y = matrixTranslation[1];
				centroid.z = matrixTranslation[2];
				
				triangle.a = centroid - centroidOffset.a;
				triangle.b = centroid - centroidOffset.b;
				triangle.c = centroid - centroidOffset.c;
			}
		}
	}


}

void GenericSAT::Render() {
	DemoBase::Render();

	static const float position[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	static const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const float ambient[] = { 0.0f, 0.0f, 0.5f, 0.0f };
	static const float diffuse[] = { 0.0f, 0.0f, 0.5f, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);

	/*CollisionResult r1 = SATIntersectionTest<AABB, OBB>(aabb.position, aabb, obb);
	CollisionResult r2 = SATIntersectionTest<AABB, Triangle>(aabb.position, aabb, triangle);
	CollisionResult r3 = SATIntersectionTest<OBB, Triangle>(obb.position, obb, triangle);

	//::Render(sphere);
	if (!r1.colliding && !r2.colliding) {
		::Render(aabb);
	}
	if (!r1.colliding & !r3.colliding) {
		::Render(obb);
	}
	if (!r2.colliding & !r3.colliding) {
		::Render(triangle);
	}*/

	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 0.0f, 1.0f);
	/*if (r1.colliding || r2.colliding) {
		::Render(GetEdges(aabb));
	}
	if (r2.colliding || r3.colliding) {
		::Render(GetEdges(triangle));
	}
	if (r1.colliding || r3.colliding) {
		::Render(GetEdges(obb));
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	if (r1.colliding) {
		for (int i = 0; i < r1.contacts.size(); ++i) {
			::Render(r1.contacts[i]);
		}
	}*/
	glEnable(GL_LIGHTING);
}

void GenericSAT::Update(float dt) {
	DemoBase::Update(dt);

	if (mouseLeftDown) {
		Ray screenRay = GetPickRay(mousePos, vec2(), size, camera.GetViewMatrix(), camera.GetProjectionMatrix());
		screenRay.NormalizeDirection();

		RaycastResult allCasts[3];
		//Raycast(sphere, screenRay, &allCasts[0]);
		Raycast(aabb, screenRay, &allCasts[0]);
		Raycast(obb, screenRay, &allCasts[1]);
		Raycast(triangle, screenRay, &allCasts[2]);

		RaycastResult raycastResult;
		ResetRaycastResult(&raycastResult);

		int wasManipulating = manipulating;

		for (int i = 0; i < 3; ++i) {
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
		else if (manipulating == 0 && wasManipulating != 0) {
			manTranslation = Translation(aabb.position.x, aabb.position.y, aabb.position.z);
			manRotation = mat4() * manTranslation;
			manScale = Scale(aabb.size.x, aabb.size.y, aabb.size.z) * manTranslation;
		}
		else if (manipulating == 1 && wasManipulating != 1) {
			manTranslation = Translation(obb.position.x, obb.position.y, obb.position.z);
			manRotation = FromMat3(obb.orientation) * manTranslation;
			manTranslation = manRotation;
			manScale = Scale(obb.size.x, obb.size.y, obb.size.z) * manTranslation;
		}
		else if (manipulating == 2 && wasManipulating != 2) {
			manTranslation = Translation(Centroid(triangle));
			manRotation = manTranslation;
			manScale = manTranslation;
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