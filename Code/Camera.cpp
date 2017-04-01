#include "Camera.h"
#include "GLWindow.h"
#include "Compare.h"

#include <iostream>

Camera::Camera() {
	m_nFov = 60.0f;
	m_nAspect = 1.3f;
	m_nNear = 0.01f;
	m_nFar = 1000.0f;
	m_nWidth = 1.0;
	m_nHeight = 1.0f;

	m_matWorld = mat4();
	m_matProj = Projection(m_nFov, m_nAspect, m_nNear, m_nFar);
	m_nProjectionMode = 0;
}

mat4 Camera::GetWorldMatrix() {
	return m_matWorld;
	/*mat3 r = Rotation3x3(rotation.x, rotation.y, rotation.z);

	return mat4(
		r._11, r._12, r._13, 0.0f,
		r._21, r._22, r._23, 0.0f,
		r._31, r._32, r._33, 0.0f,
		Dot(position, vec3(r._11, r._12, r._13)),
		Dot(position, vec3(r._21, r._22, r._23)),
		Dot(position, vec3(r._31, r._32, r._33)),
		1.0f
	);*/
}

bool Camera::IsOrthoNormal() {
	vec3 right = vec3(m_matWorld._11, m_matWorld._12, m_matWorld._13);
	vec3 up = vec3(m_matWorld._21, m_matWorld._22, m_matWorld._23);
	vec3 forward = vec3(m_matWorld._31, m_matWorld._32, m_matWorld._33);

	if (!CMP(Dot(right, right), 1.0f)) {
		return false; // X axis is not normal
	}

	if (!CMP(Dot(up, up), 1.0f)) {
		return false; // Y axis is not normal
	}

	if (!CMP(Dot(forward, forward), 1.0f)) {
		return false; // Z axis is not normal
	}

	if (!CMP(Dot(forward, up), 0.0f)) {
		return false; // Not perpendicular
	}

	if (!CMP(Dot(forward, right), 0.0f)) {
		return false; // Not perpendicular
	}

	if (!CMP(Dot(right, up), 0.0f)) {
		return false; // Not perpendicular
	}

	/*mat3 orientation = Cut(m_matWorld, 3, 3);
	mat3 transposed = Transpose(orientation);
	mat3 result = orientation * transposed;
	mat3 identity;
	
	for (int i = 0; i < 9; ++i) {
		if (!CMP(result.asArray[i], identity.asArray[i])) {
			return false;
		}
	}*/
	return true;
}

void Camera::OrthoNormalize() {
	vec3 right = vec3(m_matWorld._11, m_matWorld._12, m_matWorld._13);
	vec3 up = vec3(m_matWorld._21, m_matWorld._22, m_matWorld._23);
	vec3 forward = vec3(m_matWorld._31, m_matWorld._32, m_matWorld._33);

	vec3 f = Normalized(forward);
	vec3 r = Normalized(Cross(up, f));
	vec3 u = Cross(f, r);

	m_matWorld = mat4(
		r.x, r.y, r.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		f.x, f.y, f.z, 0.0f,
		m_matWorld._41, m_matWorld._42, m_matWorld._43, 1.0f
	);
}

mat4 Camera::GetViewMatrix() {
	//return Inverse(m_matWorld);

	if (!IsOrthoNormal()) {
		OrthoNormalize();
	}

	mat4 inverse = Transpose(m_matWorld);
	inverse._41 = inverse._14 = 0.0f;
	inverse._42 = inverse._24 = 0.0f;
	inverse._43 = inverse._34 = 0.0f;

	vec3 right = vec3(m_matWorld._11, m_matWorld._12, m_matWorld._13);
	vec3 up = vec3(m_matWorld._21, m_matWorld._22, m_matWorld._23);
	vec3 forward = vec3(m_matWorld._31, m_matWorld._32, m_matWorld._33);
	vec3 position = vec3(m_matWorld._41, m_matWorld._42, m_matWorld._43);

	inverse._41 = -Dot(right, position);
	inverse._42 = -Dot(up, position);
	inverse._43 = -Dot(forward, position);

	return inverse;
}

float Camera::GetAspect() {
	return m_nAspect;
}

mat4 Camera::GetProjectionMatrix() {
	return m_matProj;
}

void Camera::Resize(int width, int height) {
	m_nAspect = (float)width / (float)height;

	if (m_nProjectionMode == 0) { // Perspective
		m_matProj = Projection(m_nFov, m_nAspect, m_nNear, m_nFar);
	}
	else if (m_nProjectionMode == 1) { // Ortho
		m_nWidth = (float)width;
		m_nHeight = (float)height;

		float halfW = m_nWidth * 0.5f;
		float halfH = m_nHeight * 0.5f;

		m_matProj = Ortho(-halfW, halfW, halfH, -halfH, m_nNear, m_nFar);
	}
	// m_nProjectionMode == 2
		// User defined
}

bool Camera::IsOrthographic() {
	return m_nProjectionMode == 1;
}

bool Camera::IsPerspective() {
	return m_nProjectionMode == 0;
}

void Camera::Perspective(float fov, float aspect, float zNear, float zFar) {
	m_nFov = fov;
	m_nAspect = aspect;
	m_nNear = zNear;
	m_nFar = zFar;

	m_matProj = Projection(fov, aspect, zNear, zFar);
	m_nProjectionMode = 0;
}

void Camera::Orthographic(float width, float height, float zNear, float zFar) {
	m_nWidth = width;
	m_nHeight = height;
	m_nNear = zNear;
	m_nFar = zFar;

	float halfW = width * 0.5f;
	float halfH = height * 0.5f;

	m_matProj = Ortho(-halfW, halfW, halfH, -halfH, zNear, zFar);
	m_nProjectionMode = 1;
}

void Camera::SetProjection(const mat4& projection) {
	m_matProj = projection;
	m_nProjectionMode = 2;
}

void Camera::SetWorld(const mat4& view) {
	m_matWorld = view;
}

Camera CreatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
	Camera result;
	result.Perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
	return result;
}

Camera CreateOrthographic(float width, float height, float nearPlane, float farPlane) {
	Camera result;
	result.Orthographic(width, height, nearPlane, farPlane);
	return result;
}

OrbitCamera::OrbitCamera() {
	target = vec3(0, 0, 0);
	zoomDistance = 10.0f;
	zoomSpeed = 200.0f;
	rotationSpeed = vec2(250.0f, 120.0f);
	yRotationLimit = vec2(-20.0f, 80.0f);
	zoomDistanceLimit = vec2(3.0f, 15.0f);
	currentRotation = vec2(0, 0);
	panSpeed = vec2(180.0f, 180.0f);
}

void OrbitCamera::Rotate(const vec2& deltaRot, float deltaTime) {
	currentRotation.x += deltaRot.x * rotationSpeed.x * zoomDistance* deltaTime;
	currentRotation.y += deltaRot.y * rotationSpeed.y * zoomDistance * deltaTime;

	currentRotation.x = ClampAngle(currentRotation.x, -360, 360);
	currentRotation.y = ClampAngle(currentRotation.y, yRotationLimit.x, yRotationLimit.y);
}

void OrbitCamera::Zoom(float deltaZoom, float deltaTime) {
	zoomDistance = zoomDistance + deltaZoom  * zoomSpeed * deltaTime;
	if (zoomDistance < zoomDistanceLimit.x) {
		zoomDistance = zoomDistanceLimit.x;
	}
	if (zoomDistance > zoomDistanceLimit.y) {
		zoomDistance = zoomDistanceLimit.y;
	}
}

void OrbitCamera::Pan(const vec2& delataPan, float deltaTime) {
	vec3 right(m_matWorld._11, m_matWorld._12, m_matWorld._13);

	// Pan X axis in local space
	target = target - (right * (delataPan.x * panSpeed.x * deltaTime));
	// Pan Y Axis in global space
	target = target + (vec3(0, 1, 0) * (delataPan.y * panSpeed.y * deltaTime));

	// Reset zoom to allow infinate zooming after a motion
	// This part of the code is not in the book!
	float midZoom = zoomDistanceLimit.x + (zoomDistanceLimit.y - zoomDistanceLimit.x) * 0.5f;
	zoomDistance = midZoom - zoomDistance;
	vec3 rotation = vec3(currentRotation.y, currentRotation.x, 0);
	mat3 orient = Rotation3x3(rotation.x, rotation.y, rotation.z);
	vec3 dir = MultiplyVector( vec3(0.0, 0.0, -zoomDistance), orient);
	target = target - dir;
	zoomDistance = midZoom;
}

void OrbitCamera::Update(float dt) {
	vec3 rotation = vec3(currentRotation.y, currentRotation.x, 0);
	mat3 orient = Rotation3x3(rotation.x, rotation.y, rotation.z);
	vec3 dir = MultiplyVector( vec3(0.0, 0.0, -zoomDistance), orient);
	vec3 position = /*rotation * vec3(0.0, 0.0, -distance)*/dir + target;
	m_matWorld = FastInverse(LookAt(position, target, vec3(0, 1, 0)));
}

float OrbitCamera::ClampAngle(float angle, float min, float max) {
	while (angle < -360) {
		angle += 360;
	}
	while (angle > 360) {
		angle -= 360;
	}
	if (angle < min) {
		angle = min;
	}
	if (angle > max) {
		angle = max;
	}
	return angle;
}

Frustum Camera::GetFrustum() {
	Frustum result;

	mat4 vp = GetViewMatrix() * GetProjectionMatrix();

	vec3 col1(vp._11, vp._21, vp._31);//, vp._41 };
	vec3 col2(vp._12, vp._22, vp._32);//, vp._42 };
	vec3 col3(vp._13, vp._23, vp._33);//, vp._43 };
	vec3 col4(vp._14, vp._24, vp._34);//, vp._44 };
					
	// Find plane magnitudes
	result.left.normal	= col4 + col1;
	result.right.normal = col4 - col1;
	result.bottom.normal= col4 + col2;
	result.top.normal	= col4 - col2;
	result._near.normal	= /*col4 +*/ col3;
	result._far.normal	= col4 - col3;

	// Find plane distances
	result.left.distance	= vp._44 + vp._41;
	result.right.distance	= vp._44 - vp._41;
	result.bottom.distance	= vp._44 + vp._42;
	result.top.distance		= vp._44 - vp._42;
	result._near.distance	= /*vp._44 +*/ vp._43;
	result._far.distance		= vp._44 - vp._43;

	// Normalize all 6 planes
	for (int i = 0; i < 6; ++i) {
		float mag = 1.0f / Magnitude(result.planes[i].normal);
		Normalize(result.planes[i].normal);
		result.planes[i].distance *= mag;
	}

	return result;		   
}

void OrbitCamera::PrintDebug() {
	std::cout << "Target: (" << target.x << ", " << target.y << ", " << target.z << ")\n";
	std::cout << "Zoom distance: " << zoomDistance << "\n";
	std::cout << "Rotation: (" << currentRotation.x << ", " << currentRotation.y << ")\n";
}

void OrbitCamera::SetTarget(const vec3& newTarget) {
	target = newTarget;
}

void OrbitCamera::SetZoom(float zoom) {
	zoomDistance = zoom;
}

void OrbitCamera::SetRotation(const vec2& rotation) {
	currentRotation = rotation;
}