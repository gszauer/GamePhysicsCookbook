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

// TODO: REMOVE
#include "vectors.h"

void SampleApplication::OnInitialize() {
	GLWindow::OnInitialize();
	matView = LookAt(vec3(0.0f, 0.0f, -10.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));
	//glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glPointSize(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

float rot1 = 45.0f; // TODO, REMOVE
float rot2 = 45.0f;
void SampleApplication::OnRender() {
	glDisable(GL_LIGHTING);
	GLWindow::OnRender();

	OrientedRectangle r1(Point2D(1.0f, 1.0f), vec2(1.5f, 1.0f), rot1);
	glColor3f(0.0f, 0.0f, 1.0f);
	Render(r1);

	vec2 mousePos = GetMousePosition();
	mousePos.x /= (float)GetWidth();
	mousePos.y /= (float)GetHeight();
	mousePos.x *= 7.7f * 2.0f;
	mousePos.y *= 5.8f * 2.0f;
	mousePos.x -= 7.7f;
	mousePos.y -= 5.8f;
	mousePos.y *= -1.0f;

	
	OrientedRectangle r2(Point2D(mousePos.x, mousePos.y), vec2(1, 2), rot2);

	if (!OrientedRectangleOrientedRectangle(r1, r2)) {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	else {
		glColor3f(0.0f, 1.0f, 0.0f);
	}
	Render(r2);

	Rectangle2D r3(Point2D(), r1.halfExtents * 2.0f);
	OrientedRectangle r4(Point2D(mousePos.x, mousePos.y), vec2(1, 2), rot2);
	
	r4.rotation = r2.rotation - r1.rotation;
	vec2 rotVector = r2.position - r1.position;
	float theta = -DEG2RAD(r1.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	r4.position = rotVector + r1.halfExtents;

	glPushMatrix();
	glTranslatef(-3.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	Render(r3);
	glColor3f(0.0f, 1.0f, 1.0f);
	Render(r4);
	glPopMatrix();

	return;
	glEnable(GL_LIGHTING);

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
	FixedFunctionSphere(2, 0.5f);
	glPopMatrix();
	 
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
}

void SampleApplication::OnUpdate(float deltaTime) { 
	GLWindow::OnUpdate(deltaTime);

	if (MouseButonDown(MOUSE_LEFT)) {
		rot1 += 90.0f * deltaTime;
		while (rot1 > 360) {
			rot1 -= 360.0f;
		}
	}
	else if (MouseButonDown(MOUSE_RIGHT)) {
		rot2 += 90.0f * deltaTime;
		while (rot2 > 360) {
			rot2 -= 360.0f;
		}
	}

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
