#define WIN32_LEAN_AND_MEAN 
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <cstdlib>

#include "glad/glad.h"

#include "SampleApplication.h"
#include"FixedFunctionPrimitives.h"

static SampleApplication debugInstance("Sample Application", 800, 600);

// TODO: REMOVE
#include <iostream>
Mesh meshObject;
Model modelObject;
AABB ground;
Scene* scene;
AABB testPoints[] = {
	/*OBB(Point(0, 0, 0), vec3(0.5, 0.5, 0.5)),
	OBB(Point(2, 2, 2), vec3(1, 1, 1)),
	OBB(Point(1, 1, 1), vec3(0.1, 0.1, 0.1)),
	OBB(Point(1, 1, 1), vec3(4, 4, 4)),*/
	
	/*OBB(Point(0, 0, 0), vec3(0.5, 0.5, 0.5), Rotation3x3(0.0f, 0.0f, 45.0f)),
	OBB(Point(2, 2, 2), vec3(1, 1, 1), Rotation3x3(45.0f, 0.0f, 45.0f)),
	OBB(Point(1, 1, 1), vec3(0.1, 0.1, 0.1), Rotation3x3(45.0f, 0.0f, 0.0f)),*/

	AABB(Point(0, 0, 0), vec3(0.5f, 0.5f, 0.5f)),
	AABB(Point(2, 2, 2), vec3(1, 1, 1)),
	AABB(Point(1, 1, 1), vec3(0.1, 0.1, 0.1)),
	AABB(Point(1, 1, 1), vec3(4, 4, 4)),
};
int testSize = 4;
// END TODO

float SampleApplication::random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;
	
	float range = max - min;
	return (random*range) + min;
}

void SampleApplication::OnInitialize() {
	GLWindow::OnInitialize();
	m_prevMousePos = vec2(0.0f, 0.0f);

	camera.Perspective(60.0f, (float)m_nWidth / (float)m_nHeight, 0.01f, 1000.0f);

	cameraDist = -10.0f;
	//camera.LookAt(vec3(cameraPos.x, cameraPos.y, cameraDist), vec3(), vec3(0.0f, 1.0f, 0.0f));
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);

	glPointSize(3.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);
	val[0] = 0.0f; val[1] = 1.0f; val[2] = 0.0f; val[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, val);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, val);
	val[0] = 1.0f; val[2] = 1.0f;
	glLightfv(GL_LIGHT0, GL_SPECULAR, val);


	
	LoadMesh("Assets/suzane.mdl", &meshObject);
	modelObject.SetContent(&meshObject);
	modelObject.position = vec3(0, 0, 0.0);
	modelObject.rotation = vec3(0.0f, 0.0f, 0.0f);
	ground = FromMinMax(vec3(-5, -2, -5), vec3(5, -1, 5));

	scene = new Scene();
	scene->Accelerate(vec3(), 500);
}

void SampleApplication::OnShutdown() {
	delete scene;
	FreeMesh(&meshObject);
	//FreeBVHNode(meshObject.accelerator);
	//delete meshObject.accelerator;
}

void SampleApplication::OnResize(int width, int height) {
	GLWindow::OnResize(width, height);

	mat4 projection = camera.GetProjectionMatrix();
	mat4 view = camera.GetViewMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.asArray);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(view.asArray);
}

void SampleApplication::OnRender() {
	GLWindow::OnRender();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.GetViewMatrix().asArray);

	glColor3f(0.0f, 0.0f, 1.0f);

	Camera c;
	c.Perspective(60.0f, 1.3f, 1.0f, 2.0f);
	c.SetWorld(Inverse(LookAt(vec3(2.0f, 2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))));
	Frustum f = c.GetFrustum();

	Render(f);
	RenderNormals(f);

	for (int i = 0; i < testSize; ++i) {
		if (Intersects(f, testPoints[i])) {
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else {
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		Render(testPoints[i]);
	}

	FixedFunctionOrigin(true, false);
}

void SampleApplication::OnUpdate(float deltaTime) {
	GLWindow::OnUpdate(deltaTime);
	
	bool leftDown = MouseButonDown(MOUSE_LEFT);
	bool middleDown = MouseButonDown(MOUSE_MIDDLE);
	bool rightDown = MouseButonDown(MOUSE_RIGHT);

	vec2 mousePos = GetMousePosition();
	vec2 mouseDelta = mousePos - m_prevMousePos;
	mouseDelta.x /= (float)GetWidth();
	mouseDelta.y /= (float)GetHeight();
	
	if (rightDown) { // Rotation logic
		camera.Rotate(mouseDelta, deltaTime);
	}
	else if (middleDown) { // Zoom logic
		camera.Zoom(mouseDelta.y, deltaTime);
	}
	else if (leftDown) { // Move logic
		camera.Pan(mouseDelta, deltaTime);
	}

	camera.Update(deltaTime);
	m_prevMousePos = mousePos;
}