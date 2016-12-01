#include "CH14Demo.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"

void CH14Demo::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);
	leftWasDown = false;
	ray.origin = Point(0.0f, 3.0f, 0.0f);
	glPointSize(5.0f);
	obb.orientation = Rotation3x3(45.0f, 30.0f, 90.0f);
}

void CH14Demo::Render() {
	DemoBase::Render();

	RaycastResult result;

	if (Raycast(obb, ray, &result)) {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	else {
		glColor3f(0.0f, 1.0f, 0.0f);
	}
	::Render(obb);

	glColor3f(0.0f, 0.0f, 1.0f);
	::Render(ray);

	if (result.hit) {
		glColor3f(1.0f, 1.0f, 0.0f);
		::Render(result.point);
		glColor3f(0.0f, 1.0f, 1.0f);
		Line normal(result.point, result.point + result.normal);
		::Render(normal);
	}
}

void CH14Demo::Update(float dt) {
	DemoBase::Update(dt);

	if (mouseLeftDown && !leftWasDown) {
		ray = GetPickRay(mousePos, vec2(), size, camera.GetViewMatrix(), camera.GetProjectionMatrix());
	}

	leftWasDown = mouseLeftDown;
}