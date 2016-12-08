#ifndef _H_CAMERA_
#define _H_CAMERA_

#include "matrices.h"
#include "Geometry3D.h"

class Camera {
protected:
	float m_nFov;
	float m_nAspect;
	float m_nNear;
	float m_nFar;
	float m_nWidth;
	float m_nHeight;

	mat4 m_matWorld; // World Transform
	// View Transform = Inverse(World Transform)
	mat4 m_matProj;
	int m_nProjectionMode; // 0 - Perspective, 1 - Ortho, 2 - User
public:
	Camera();
	// Default copy constructor / assignment operator will do!
	inline virtual ~Camera() { }

	mat4 GetWorldMatrix();
	mat4 GetViewMatrix(); // Inverse of world!
	mat4 GetProjectionMatrix();

	float GetAspect();
	bool IsOrthographic();
	bool IsPerspective();

	bool IsOrthoNormal();
	void OrthoNormalize();

	void Resize(int width, int height);
	
	void Perspective(float fov, float aspect, float zNear, float zFar);
	void Orthographic(float width, float height, float zNear, float zFar);
	
	void SetProjection(const mat4& projection);
	void SetWorld(const mat4& view);

	Frustum GetFrustum();
};

Camera CreatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
Camera CreateOrthographic(float width, float height, float nearPlane, float farPlane);

class OrbitCamera : public Camera {
protected:
	vec3 target;
	vec2 panSpeed;

	float zoomDistance;
	vec2 zoomDistanceLimit; // x = min, y = max;
	float zoomSpeed;

	vec2 rotationSpeed;
	vec2 yRotationLimit; // x = min, y = max
	vec2 currentRotation;
public:
	OrbitCamera();
	inline virtual ~OrbitCamera() { }

	void Rotate(const vec2& deltaRot, float deltaTime);
	void Zoom(float deltaZoom, float deltaTime);
	void Pan(const vec2& delataPan, float deltaTime);

	void Update(float dt);
	float ClampAngle(float angle, float min, float max);

	void SetTarget(const vec3& newTarget);
	void SetZoom(float zoom);
	void SetRotation(const vec2& rotation);
	void PrintDebug();
};

#endif 
