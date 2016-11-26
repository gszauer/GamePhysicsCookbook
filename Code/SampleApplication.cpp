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

bool doRaycast = false;
Ray cast;
vec2 castMouse;
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

	float aspect = (float)m_nWidth / (float)m_nHeight;
	//camera.Perspective(60.0f, (float)m_nWidth / (float)m_nHeight, 0.01f, 1000.0f);
	camera.Orthographic(10 * aspect, 10, 0.01f, 1000.0f);

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
	AccelerateMesh(meshObject);
	
	ground = FromMinMax(vec3(-5, -2, -5), vec3(5, -1, 5));

	scene = new Scene();
	scene->AddModel(&modelObject);
	scene->Accelerate(vec3(), 500);
}

void SampleApplication::OnShutdown() {
	delete scene;
	FreeMesh(&meshObject);
	FreeBVHNode(meshObject.accelerator);
	delete meshObject.accelerator;
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


	

	
	Camera c;
	//c.SetWorld(Inverse(LookAt(vec3(2.0f, 2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))));
	c.SetWorld(Inverse(LookAt(vec3(0, 0, 0), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f))));
	
	//glColor3f(0.0f, 0.0f, 1.0f);
	//c.Orthographic(5.0f, 5.0f, 0.5f, 3.0f);
	//Render(c.GetFrustum());

	glColor3f(0.0f, 1.0f, 0.0f);
	c.Perspective(60.0f, 1.3f, 0.5f, 3.0f);
	Render(c.GetFrustum());

	RenderNormals(c.GetFrustum());
	//Render(Point(2, 2, 2));

	FixedFunctionOrigin(true, false);
}

void SampleApplication::OnUpdate(float deltaTime) {
	GLWindow::OnUpdate(deltaTime);
	

	if (KeyDown(KEY_ONE)) {
		camera.SetWorld(Inverse(LookAt(vec3(0, 0, 15), vec3(0, 0, 0), vec3(0, 1, 0))));
	}
	if (KeyDown(KEY_TWO)) {
		camera.SetWorld(Inverse(LookAt(vec3(0, 15, 0), vec3(0, 0, 0), vec3(0, 0, 1))));
	}
	if (KeyDown(KEY_THREE)) {
		camera.SetWorld(Inverse(LookAt(vec3(15, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0))));
	}

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
		doRaycast = true;
		castMouse.x = mousePos.x;
		castMouse.y = mousePos.y;

		camera.Pan(mouseDelta, deltaTime);
	}

	//camera.Update(deltaTime);
	m_prevMousePos = mousePos;
}