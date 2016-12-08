#include "RaycastDemo.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include <iostream>

void DebugRaycastResult(RaycastResult* result) {
	if (result == 0) {
		return;
	}

	std::cout << "Hit: " << result->hit << "\n";
	std::cout << "t: " << result->t << "\n";
	std::cout << "Point: " << result->point.x << ", " << result->point.y << ", " << result->point.z << "\n";
	std::cout << "Normal: " << result->normal.x << ", " << result->normal.y << ", " << result->normal.z << "\n";
}

void RaycastDemo::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);

	ResetRaycastResult(&raycastResult);

	sphere.position = vec3(-4, 2, 0);
	aabb.position = vec3(4, 2, 0);
	obb.position = vec3(-4, -2, 0);
	obb.orientation = Rotation3x3(30.0f, 20.0f, 0.0f);
	plane = FromTriangle(Triangle(
		Point(4, -1, 0),
		Point(3, -3, 0),
		Point(3, -3, 1)
	));
	triangle.a = vec3(0, 3, 0);
	triangle.b = vec3(-1, 1, 0);
	triangle.c = vec3(1, 1, 0);

	glPointSize(5.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);
}

void RaycastDemo::Render() {
	DemoBase::Render();

	static const float position[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	static const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const float ambient[] = { 0.0f, 0.0f, 0.5f, 0.0f };
	static const float diffuse[] = { 0.0f, 0.0f, 0.5f, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);

	::Render(sphere);
	::Render(aabb);
	::Render(obb);
	::Render(plane);
	::Render(triangle);

	glDisable(GL_LIGHTING);
	if (mouseLeftDown) {
		screenRay = GetPickRay(mousePos, vec2(), size, camera.GetViewMatrix(), camera.GetProjectionMatrix());
		screenRay.NormalizeDirection();

		Raycast(sphere, screenRay, 0);
		Raycast(aabb, screenRay, 0);
		Raycast(obb, screenRay, 0);
		Raycast(plane, screenRay, 0);
		Raycast(triangle, screenRay, 0);

		RaycastResult allCasts[5];
		Raycast(sphere, screenRay, &allCasts[0]);
		Raycast(aabb, screenRay, &allCasts[1]);
		Raycast(obb, screenRay, &allCasts[2]);
		Raycast(plane, screenRay, &allCasts[3]);
		Raycast(triangle, screenRay, &allCasts[4]);

		raycastResult = allCasts[0];
		for (int i = 0; i < 5; ++i) {
			if (!allCasts[i].hit) {
				if (allCasts[i].t >= 0) {
					DebugRaycastResult(&allCasts[i]);
				}
			}

			if (allCasts[i].hit) {
				if (!raycastResult.hit) {
					raycastResult = allCasts[i];
				}
				else if (allCasts[i].t < raycastResult.t) {
					raycastResult = allCasts[i];
				}
			}
		}
	}

	if (raycastResult.hit) {
		glColor3f(1.0f, 0.0f, 0.0f);
		::Render(raycastResult.point);

		glColor3f(0.0f, 1.0f, 0.0f);
		Line normal(raycastResult.point, raycastResult.point + raycastResult.normal);
		::Render(normal);
	}
	glColor3f(0.0f, 0.0f, 1.0f);
	::Render(screenRay);
	glEnable(GL_LIGHTING);
}

void RaycastDemo::Update(float dt) {
	DemoBase::Update(dt);
}