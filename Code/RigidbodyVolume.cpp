#include "RigidbodyVolume.h"
#include "Compare.h"
#include "FixedFunctionPrimitives.h"

void RigidbodyVolume::ApplyForces() {
	forces = GRAVITY_CONST * mass;
}

#ifndef LINEAR_ONLY
void  RigidbodyVolume::AddRotationalImpulse(const vec3& point, const vec3& impulse) {
	vec3 centerOfMass = position;
	vec3 torque = Cross(point - centerOfMass, impulse);

	vec3 angAccel = MultiplyVector(torque, InvTensor());
	angVel = angVel + angAccel;
}
#endif

void RigidbodyVolume::AddLinearImpulse(const vec3& impulse) {
	velocity = velocity + impulse;
}

float RigidbodyVolume::InvMass() {
	if (mass == 0.0f) {
		return 0.0f;
	}
	return 1.0f / mass;
}

void RigidbodyVolume::SynchCollisionVolumes() {
	sphere.position = position;
	box.position = position;

#ifndef LINEAR_ONLY
	box.orientation = Rotation3x3(
		RAD2DEG(orientation.x),
		RAD2DEG(orientation.y),
		RAD2DEG(orientation.z)
	);
#endif
}

void RigidbodyVolume::Render() {
	SynchCollisionVolumes();

	if (type == RIGIDBODY_TYPE_SPHERE) {
		::Render(sphere);
	}
	else if (type == RIGIDBODY_TYPE_BOX) {
		::Render(box);
	}
}

#ifndef LINEAR_ONLY
mat4 RigidbodyVolume::InvTensor() {
	if (mass == 0) {
		return mat4(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}
	float ix = 0.0f;
	float iy = 0.0f;
	float iz = 0.0f;
	float iw = 0.0f;

	if (mass != 0 && type == RIGIDBODY_TYPE_SPHERE) {
		float r2 = sphere.radius * sphere.radius;
		float fraction = (2.0f / 5.0f);

		ix = r2 * mass * fraction;
		iy = r2 * mass * fraction;
		iz = r2 * mass * fraction;
		iw = 1.0f;
	}
	else if (mass != 0 && type == RIGIDBODY_TYPE_BOX) {
		vec3 size = box.size * 2.0f;
		float fraction = (1.0f / 12.0f);

		float x2 = size.x * size.x;
		float y2 = size.y * size.y;
		float z2 = size.z * size.z;

		ix = (y2 + z2) * mass * fraction;
		iy = (x2 + z2) * mass * fraction;
		iz = (x2 + y2) * mass * fraction;
		iw = 1.0f;
	}

	return Inverse(mat4(
		ix, 0, 0, 0,
		0, iy, 0, 0,
		0, 0, iz, 0,
		0, 0, 0, iw));
}
#endif

void RigidbodyVolume::Update(float dt) {
	// Integrate velocity
	const float damping = 0.98f;

	vec3 acceleration = forces * InvMass();
	velocity = velocity + acceleration * dt;
	velocity = velocity * damping;

	if (fabsf(velocity.x) < 0.001f) {
		velocity.x = 0.0f;
	}
	if (fabsf(velocity.y) < 0.001f) {
		velocity.y = 0.0f;
	}
	if (fabsf(velocity.z) < 0.001f) {
		velocity.z = 0.0f;
	}

#ifndef LINEAR_ONLY
	if (type == RIGIDBODY_TYPE_BOX) {
		vec3 angAccel = MultiplyVector(torques, InvTensor());
		angVel = angVel + angAccel * dt;
		angVel = angVel *  damping;

		if (fabsf(angVel.x) < 0.001f) {
			angVel.x = 0.0f;
		}
		if (fabsf(angVel.y) < 0.001f) {
			angVel.y = 0.0f;
		}
		if (fabsf(angVel.z) < 0.001f) {
			angVel.z = 0.0f;
		}
	}
#endif

	// Integrate position
	position = position + velocity * dt;

#ifndef LINEAR_ONLY
	if (type == RIGIDBODY_TYPE_BOX) {
		orientation = orientation + angVel * dt;
	}
#endif

	SynchCollisionVolumes();
}

CollisionManifold FindCollisionFeatures(RigidbodyVolume& ra, RigidbodyVolume& rb) {
	CollisionManifold result;
	ResetCollisionManifold(&result);

	if (ra.type == RIGIDBODY_TYPE_SPHERE) {
		if (rb.type == RIGIDBODY_TYPE_SPHERE) {
			result = FindCollisionFeatures(ra.sphere, rb.sphere);
		}
		else if (rb.type == RIGIDBODY_TYPE_BOX) {
			result = FindCollisionFeatures(rb.box, ra.sphere);
			result.normal = result.normal * -1.0f;
		}
	}
	else if (ra.type == RIGIDBODY_TYPE_BOX) {
		if (rb.type == RIGIDBODY_TYPE_BOX) {
			result = FindCollisionFeatures(ra.box, rb.box);
		}
		else if (rb.type == RIGIDBODY_TYPE_SPHERE) {
			result = FindCollisionFeatures(ra.box, rb.sphere);
		}
	}


	return result;
}

void ApplyImpulse(RigidbodyVolume& A, RigidbodyVolume& B, const CollisionManifold& M, int c) {
	// Linear impulse
	float invMass1 = A.InvMass();
	float invMass2 = B.InvMass();
	float invMassSum = invMass1 + invMass2;

	if (invMassSum == 0.0f) {
		return; // Both objects have infinate mass!
	}

#ifndef LINEAR_ONLY
	vec3 r1 = M.contacts[c] - A.position;
	vec3 r2 = M.contacts[c] - B.position;
	mat4 i1 = A.InvTensor();
	mat4 i2 = B.InvTensor();
#endif

	// Relative velocity
#ifndef LINEAR_ONLY
	vec3 relativeVel = (B.velocity + Cross(B.angVel, r2)) - (A.velocity + Cross(A.angVel, r1));
#else
	vec3 relativeVel = B.velocity - A.velocity;
#endif
	// Relative collision normal
	vec3 relativeNorm = M.normal;
	Normalize(relativeNorm);

	// Moving away from each other? Do nothing!
	if (Dot(relativeVel, relativeNorm) > 0.0f) {
		return;
	}

	float e = fminf(A.cor, B.cor);

	float numerator = (-(1.0f + e) * Dot(relativeVel, relativeNorm));
	float d1 = invMassSum;
#ifndef LINEAR_ONLY
	vec3 d2 = Cross(MultiplyVector(Cross(r1, relativeNorm), i1), r1);
	vec3 d3 = Cross(MultiplyVector(Cross(r2, relativeNorm), i2), r2);
	float denominator = d1 + Dot(relativeNorm, d2 + d3);
#else
	float denominator = d1;
#endif

	float j = (denominator == 0.0f) ? 0.0f : numerator / denominator;
	if (M.contacts.size() > 0.0f && j != 0.0f) {
		j /= (float)M.contacts.size();
	}

	vec3 impulse = relativeNorm * j;
	A.velocity = A.velocity - impulse *  invMass1;
	B.velocity = B.velocity + impulse *  invMass2;

#ifndef LINEAR_ONLY
	A.angVel = A.angVel - MultiplyVector(Cross(r1, impulse), i1);
	B.angVel = B.angVel + MultiplyVector(Cross(r2, impulse), i2);
#endif

	// Friction
	vec3 t = relativeVel - (relativeNorm * Dot(relativeVel, relativeNorm));
	if (CMP(MagnitudeSq(t), 0.0f)) {
		return;
	}
	Normalize(t);

	numerator = -Dot(relativeVel, t);
	d1 = invMassSum;
#ifndef LINEAR_ONLY
	d2 = Cross(MultiplyVector(Cross(r1, t), i1), r1);
	d3 = Cross(MultiplyVector(Cross(r2, t), i2), r2);
	denominator = d1 + Dot(t, d2 + d3);
#else
	denominator = d1;
#endif

	float jt = (denominator == 0.0f) ? 0.0f : numerator / denominator;
	if (M.contacts.size() > 0.0f && jt != 0.0f) {
		jt /= (float)M.contacts.size();
	}

	if (CMP(jt, 0.0f)) {
		return;
	}

	vec3 tangentImpuse;
#ifdef DYNAMIC_FRICTION
	float sf = sqrtf(A.staticFriction * B.staticFriction);
	float df = sqrtf(A.dynamicFriction * B.dynamicFriction);
	if (fabsf(jt) < j * sf) {
		tangentImpuse = t * jt;
	}
	else {
		tangentImpuse = t * -j * df;
	}
#else
	float friction = sqrtf(A.friction * B.friction);
	if (jt > j * friction) {
		jt = j * friction;
	}
	else if (jt < -j * friction) {
		jt = -j * friction;
	}
	tangentImpuse = t * jt;
#endif

	A.velocity = A.velocity - tangentImpuse *  invMass1;
	B.velocity = B.velocity + tangentImpuse *  invMass2;

#ifndef LINEAR_ONLY
	A.angVel = A.angVel - MultiplyVector(Cross(r1, tangentImpuse), i1);
	B.angVel = B.angVel + MultiplyVector(Cross(r2, tangentImpuse), i2);
#endif
}