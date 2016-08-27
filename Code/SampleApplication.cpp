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
	matView = LookAt(vec3(0.0f, 0.0f, -10.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));
	//glDisable(GL_CULL_FACE);
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

void SampleApplication::OnRender() {
	glDisable(GL_LIGHTING);
	GLWindow::OnRender();

	glColor3f(1.0f, 1.0f, 0.0f);
	Rectangle2D r(Point2D(1, 0), vec2(2, 3));
	Render(r);

	Line2D ls[] = {
		Line2D(Point2D(0, 0), Point2D(5, 5)),
		//Line2D(Point2D(1, 0), Point2D(1, 5)),
		Line2D(Point2D(-4, -3), Point2D(-3, -4)),
		Line2D(Point2D(0, 0), Point2D(0, -5)),
		Line2D(Point2D(0.99f, 0), Point2D(0.99f, 5)),
	};

	for (int i = 0; i < 4; ++i) {
		if (LineRectangle(ls[i], r)) {
			glColor3f(0.0f, 0.0f, 1.0f);
		}
		else {
			glColor3f(1.0f, 0.0f, 1.0f);
		}
		Render(ls[i]);
	}

	return;

	Circle c1(Point2D(1.0f, 1.0f), 0.71f);

	glColor3f(1.0f, 1.0f, 0.0f);
	Render(c1);

	Line2D lines[] = {
		Line2D(Point2D(-2, -2), Point2D(2, 2)),
		Line2D(Point2D(-2, -2), Point2D(-2, -6)),
		Line2D(Point2D(0, 1), Point2D(1, 0)),
		Line2D(Point2D(5, 6), Point2D(6, 5))
	};
	for (int i = 0; i < 4; ++i) {
		if (LineCircle(lines[i], c1)) {
			glColor3f(0.0f, 1.0f, 1.0f);
		}
		else {
			glColor3f(1.0f, 0.0f, 1.0f);
		}
		Render(lines[i]);
	}

	return;
	/*glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 3.0f, 0.0f);
	glEnd();*/

	glColor3f(0.0f, 1.0f, 1.0f);
	OrientedRectangle rect(Point2D(3, 3), vec2(2, 1), 0.0);
	Render(rect);
	rect.rotation = 45.0f;
	Render(rect);

	glColor3f(1.0f, 0.0f, 1.0f);
	vec2 corner1(1, 2);
	vec2 corner2(1, 4);

	vec2 rc1 = rect.position + RotateVector(corner1 - rect.position, 45);
	vec2 rc2 = rect.position + RotateVector(corner2 - rect.position, 45);

	vec2 start = rc1;
	vec2 end = rc2;
	float percent = 0.5f;
	vec2 midPoint = (start + ((end - start) * percent));

	vec2 leftPoint = midPoint - rect.position;
	leftPoint = midPoint + Normalized(leftPoint) * 0.1f;
	vec2 rightPoint = midPoint - rect.position;
	rightPoint = midPoint - Normalized(rightPoint) * 0.1f;

	Render(rightPoint);
	vec2 localPoint = rightPoint - rect.position;
	//Render(localPoint);

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(rect.position.x + localPoint.x, rect.position.y + localPoint.y, 0.0f);
	glVertex3f(rect.position.x, rect.position.y, 0.0f);
	glEnd();


	float theta = -DEG2RAD(rect.rotation);
	float zRotation2x2[] = { // Construct matrix
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	// Rotate vector
	Multiply(localPoint.asArray, vec2(localPoint.x, localPoint.y).asArray, 1, 2, zRotation2x2, 2, 2);

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(rect.position.x + localPoint.x, rect.position.y + localPoint.y, 0.0f);
	glVertex3f(rect.position.x, rect.position.y, 0.0f);
	glEnd();


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
