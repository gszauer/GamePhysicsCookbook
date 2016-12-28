#ifndef _H_RIGIDBODY_
#define _H_RIGIDBODY_

#include <vector>
#include <string>
#include "Geometry3D.h"
#include "Compare.h"

#define RIGIDBODY_TYPE_BASE		0
#define RIGIDBODY_TYPE_PARTICLE	1
#define RIGIDBODY_TYPE_SPHERE	2
#define RIGIDBODY_TYPE_BOX		3

#define GRAVITY_CONST vec3(0.0f, -9.82f, 0.0f)

class Rigidbody {
public:
	vec3 position;
	vec3 velocity;
	// linear acceleration = F / M

	vec3 forces; // sumForces
	vec3 torques; // Sum torques

	vec3 orientation;
	vec3 angVel;

	//vec3 inertia;
	std::string debug;

	float mass;
	float cor; // Coefficient of restitution
	float staticFriction;
	float dynamicFriction;

	int type;
	OBB box;
	Sphere sphere;
public:
	inline mat4 InvTensor() {
		if (mass == 0.0f) {
			return mat4(0,0,0,0
			,0,0,0,0,
				0,0,0,0,
				0,0,0,0);
		}
		SynchCollisionVolumes();
		mat4 matR = FromMat3(box.orientation);
		mat4 matT = Translation(box.position);
		mat4 m_matWorld = matR * matT;
		vec3 size = box.size * 2.0f;

		float x2 = (size.x * size.x);
		float y2 = (size.y * size.y);
		float z2 = (size.z * size.z);
		float ix = (y2 + z2) * mass / 12.0f;
		float iy = (x2 + z2) * mass / 12.0f;
		float iz = (x2 + y2) * mass / 12.0f;


		mat4 m_boxInertia = mat4(
			ix, 0, 0, 0,
			0, iy, 0, 0,
			0, 0, iz, 0,
			0, 0, 0, 1);


		mat4 m_invInertia = Inverse(matR) * Inverse(m_boxInertia) * matR;
		return m_invInertia;
	}

	inline Rigidbody() :
		cor(0.5f), mass(1.0f),
		staticFriction(0.5f),
		dynamicFriction(0.3f),
		type(RIGIDBODY_TYPE_BASE) {
		angVel = vec3();
		debug = "none";
	}

	inline Rigidbody(int bodyType) :
		cor(0.5f), mass(1.0f),
		staticFriction(0.5f),
		dynamicFriction(0.3f),
		type(bodyType) {
		angVel = vec3();
		debug = "none";
	}

	virtual ~Rigidbody() { }

	virtual void Render();
	virtual void UpdatePoisition(float dt);
	virtual void UpdateVelocity(float dt);

	virtual void ApplyForces();
	float InvMass();
	void SynchCollisionVolumes();

	virtual void AddLinearImpulse(const vec3& impulse);
	virtual void AddRotationalImpulse(const vec3& point, const vec3& impulse);
	inline virtual void SolveConstraints(const std::vector<OBB>& constraints) { }
};

CollisionManifold FindCollisionFeatures(Rigidbody& ra, Rigidbody& rb);
void ApplyImpulse(Rigidbody& A, Rigidbody& B, const CollisionManifold& M, int c, float dt);

static vec3 operator*(const vec3& v, const mat4& m) {
	return MultiplyVector(v, m);
}

#endif