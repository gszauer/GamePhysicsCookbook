#define WIN32_LEAN_AND_MEAN 
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>

#ifdef USE_GLEW
#include <GL\glew.h>
#include <GL\wglew.h>
#endif
#include <gl\GL.h>

#include "SampleApplication.h"
#include"FixedFunctionPrimitives.h"

void SampleApplication::OnInitialize() {
	GLWindow::OnInitialize();
	matView = LookAt(vec3(cameraPos.x, cameraPos.y, -10.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));
	glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);

	glPointSize(3.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);
	val[0] = 0.0f; val[1] = 1.0f; val[2] = 0.0f; val[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, val);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, val);
	val[0] = 1.0f; val[2] = 1.0f;
	glLightfv(GL_LIGHT0, GL_SPECULAR, val);

	cubeRotation.x = 30.0f;
	cubeRotation.y = 15.0f;
	cubeRotation.z = 5.0f;

	cubeScale = 0.2f;
	sphereScale = 2.0f;

	sphereRotation = cubeRotation;
}

void SampleApplication::OnRender() {
	GLWindow::OnRender();

	/*vec2 mousePos = GetMousePosition();
	mousePos.x /= (float)GetWidth();
	mousePos.y /= (float)GetHeight();
	mousePos.x *= 7.7f * 2.0f;
	mousePos.y *= 5.8f * 2.0f;
	mousePos.x -= 7.7f;
	mousePos.y -= 5.8f;
	mousePos.y *= -1.0f;*/


	/*
	// TODO: Is this the correct transform order?
	glPushMatrix();
	// Translate Last
	glTranslatef(1.0f, 0.5f, 1.0f);
	// Rotate Second
	glRotatef(cubeRotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(cubeRotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(cubeRotation.z, 0.0f, 0.0f, 1.0f);
	// Scale First
	glScalef(cubeScale, cubeScale, cubeScale);
	FixedFunctionCube(0.5f, 0.5f, 0.5f);
	glPopMatrix();
	*/

	// TODO: Same as above!
	glPushMatrix();
	// Translate Last
	glTranslatef(-1.0f, 0.5f, 1.0f);
	// Rotate Second
	glRotatef(sphereRotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(sphereRotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(sphereRotation.z, 0.0f, 0.0f, 1.0f);
	// Scale First
	glScalef(sphereScale, sphereScale, sphereScale);
	//FixedFunctionSphere(2, 0.5f); TODO: Uncomment
	FixedFunctionCone(); // TODO: Delete
	glPopMatrix();

	FixedFunctionCone();


	/*
	// TODO: Same as above!
	glPushMatrix();
	// Translate Last
	glTranslatef(1.0f, 0.5f, -1.0f);
	// Rotate Second
	glRotatef(sphereRotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(sphereRotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(sphereRotation.z, 0.0f, 0.0f, 1.0f);
	// Scale First
	glScalef(cubeScale, cubeScale, cubeScale);
	FixedFunctionTorus(0.75f, 0.5f);
	glPopMatrix();


	// TODO: Same as above!
	glPushMatrix();
	// Translate Last
	glTranslatef(-2.0f, 0.5f, -1.0f);
	// Rotate Second
	glRotatef(sphereRotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(sphereRotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(sphereRotation.z, 0.0f, 0.0f, 1.0f);
	// Scale First
	glScalef(cubeScale, cubeScale, cubeScale);
	FixedFunctionCylinder(15, 1.0f, 0.5f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -1.0f, 0.0f);
	FixedFunctionPlane();
	glPopMatrix();
	*/

	FixedFunctionOrigin();
}

void SampleApplication::OnUpdate(float deltaTime) {
	GLWindow::OnUpdate(deltaTime);

	if (KeyDown(KEY_DOWN_ARROW)) {
		cameraPos.y += -5.0f * deltaTime;
	}
	else if (KeyDown(KEY_UP_ARROW)) {
		cameraPos.y += 5.0f * deltaTime;
	}

	if (KeyDown(KEY_LEFT_ARROW)) {
		cameraPos.x += -5.0f * deltaTime;
	}
	else if (KeyDown(KEY_RIGHT_ARROW)) {
		cameraPos.x += 5.0f * deltaTime;
	}

	matView = LookAt(vec3(cameraPos.x, cameraPos.y, -10.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));

	cubeRotation.x += 90.0f * deltaTime;
	cubeRotation.y += 45.0f * deltaTime;
	cubeRotation.z += 25.0f * deltaTime;

	while (cubeRotation.x >= 360.0f) {
		cubeRotation.x -= 360.0f;
	}
	while (cubeRotation.y >= 360.0f) {
		cubeRotation.y -= 360.0f;
	}
	while (cubeRotation.z >= 360.0f) {
		cubeRotation.z -= 360.0f;
	}

	if (cubeScale < 1.0f) {
		cubeScale += 0.5f * deltaTime;

		if (cubeScale > 1.0f) {
			cubeScale = 1.0f;
		}
	}

	if (sphereScale > 1.0f) {
		sphereScale -= deltaTime;

		if (sphereScale < 1.0f) {
			sphereScale = 1.0f;
		}
	}

	sphereRotation.x += 25.0f * deltaTime;
	sphereRotation.y += 90.0f * deltaTime;
	sphereRotation.z += 45.0f * deltaTime;

	while (sphereRotation.x >= 360.0f) {
		sphereRotation.x -= 360.0f;
	}
	while (sphereRotation.y >= 360.0f) {
		sphereRotation.y -= 360.0f;
	}
	while (cubeRotation.z >= 360.0f) {
		sphereRotation.z -= 360.0f;
	}
}