#include "Rigidbody.h"
#include "Compare.h"
#include "FixedFunctionPrimitives.h"

void Rigidbody::ApplyForces() {
	forces = GRAVITY_CONST;
}

void  Rigidbody::AddRotationalImpulse(const vec3& point, const vec3& impulse) {
	vec3 centerOfMass = position;
	vec3 torque = Cross(point - centerOfMass, impulse);

	/*vec3 inertia = TensorVector();
	vec3 angAccel(
		inertia.x == 0.0f ? 0.0f : torque.x / inertia.x,
		inertia.y == 0.0f ? 0.0f : torque.y / inertia.y,
		inertia.z == 0.0f ? 0.0f : torque.z / inertia.z
	);*/

	vec3 angAccel = torque * InvTensor();

	angVel = angVel + angAccel;
}

void Rigidbody::AddLinearImpulse(const vec3& impulse) {
	velocity = velocity + impulse;
}

float Rigidbody::InvMass() {
	if (mass == 0.0f) {
		return 0.0f;
	}
	return 1.0f / mass;
}

void Rigidbody::SynchCollisionVolumes() {
	sphere.position = position;
	box.position = position;
	box.orientation = Rotation3x3(
		RAD2DEG(orientation.x), 
		RAD2DEG(orientation.y), 
		RAD2DEG(orientation.z)
	);
}

void Rigidbody::Render() {
	SynchCollisionVolumes();

	if (type == RIGIDBODY_TYPE_SPHERE) {
		::Render(sphere);
	}
	else if (type == RIGIDBODY_TYPE_BOX) {
		::Render(box);
	}
}



void Rigidbody::Update(float dt) {
	// Linear
	vec3 acceleration = forces * InvMass(); // A = F / M
	velocity = velocity + acceleration * dt;
	position = position + velocity * dt;

	if (type == RIGIDBODY_TYPE_BOX) {
		// Angular
		/*vec3 inertia = TensorVector();
		vec3 angAccel(
			inertia.x == 0.0f ? 0.0f : torques.x / inertia.x,
			inertia.y == 0.0f ? 0.0f : torques.y / inertia.y,
			inertia.z == 0.0f ? 0.0f : torques.z / inertia.z
		);*/
		vec3 angAccel = torques * InvTensor();
		angVel = angVel + angAccel * dt;
		orientation = orientation + angVel * dt;
	}

	SynchCollisionVolumes();
}

CollisionManifold FindCollisionFeatures(Rigidbody& ra, Rigidbody& rb) {
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

void ApplyImpulse(Rigidbody& A, Rigidbody& B, const CollisionManifold& M, int c) {
	// Linear impulse
	float invMass1 = A.InvMass();
	float invMass2 = B.InvMass();
	float invMassSum = invMass1 + invMass2;

	if (invMassSum == 0.0f) {
		return; // Both objects have infinate mass!
	}

	vec3 r1 = M.contacts[c] - A.position;
	vec3 r2 = M.contacts[c] - B.position;
	/*vec3 r1 = M.contacts[c] - A.position;
	vec3 r2 = M.contacts[c] - B.position;*/
	vec3 i1 = A.InvTensor();
	vec3 i2 = B.InvTensor();

	// Relative velocity
	vec3 relativeVel = (B.velocity + Cross(B.angVel, r2)) - (A.velocity + Cross(A.angVel, r1));
	//vec3 relativeVel = B.velocity - A.velocity;
	// Relative collision normal
	vec3 relativeNorm = M.normal;
	Normalize(relativeNorm);

	// Moving away from each other? Do nothing!
	if (Dot(relativeVel, relativeNorm) > 0.0f) {
		return;
	}

	float e = fminf(A.cor, B.cor);
	//vec3 vr = B.velocity - A.velocity;
	vec3 vr = (B.velocity + Cross(B.angVel, r2)) - (A.velocity + Cross(A.angVel, r1));
	vec3 n = M.normal;
	Normalize(n);

	float numerator = (-(1.0f + e) * Dot(vr, n));
	float d1 = Dot(n, n) +( invMassSum);
	vec3 d2 = Cross(Cross(r1, n) * i1, r1);
	vec3 d3 = Cross(Cross(r2, n) * i2, r2);
	float denominator = d1 + Dot(n, d2 + d3);

	float j = (denominator == 0.0f) ? 0.0f : numerator / denominator;

	vec3 impulse = relativeNorm * j;
	A.velocity = A.velocity - impulse *  invMass1;
	B.velocity = B.velocity + impulse *  invMass2;

	A.angVel = A.angVel - Cross(r1, impulse) *  i1;
	B.angVel = B.angVel + Cross(r2, impulse) *  i2;



	/// Friction
	float sf = sqrtf(A.staticFriction * A.staticFriction + B.staticFriction * B.staticFriction);
	float df = sqrtf(A.dynamicFriction * A.dynamicFriction + B.dynamicFriction * B.dynamicFriction);

	//relativeVel = B.velocity - A.velocity;
	relativeVel = (B.velocity + Cross(B.angVel, r2)) - (A.velocity + Cross(A.angVel, r1));
	vec3 t = relativeVel - relativeNorm * Dot(relativeVel, relativeNorm);
	if (CMP(MagnitudeSq(t), 0.0f)) {
		return;
	}
	Normalize(t);

	/*float jt = -Dot(relativeVel, t);
	jt /= (invMass1 + invMass2);
	if (M.contacts.size() > 0.0f) {
		jt /= (float)M.contacts.size();
	}*/

	numerator = -Dot(relativeVel, t);
	d1 = Dot(t, t) + (invMassSum);
	d2 = Cross(Cross(r1, t) * i1, r1);
	d3 = Cross(Cross(r2, t) * i2, r2);
	denominator = d1 + Dot(t, d2 + d3);

	float jt = (denominator == 0.0f) ? 0.0f : numerator / denominator;
	if (M.contacts.size() > 0.0f) {
		jt /= (float)M.contacts.size();
	}

	if (CMP(jt, 0.0f)) {
		return;
	}

	vec3 tangentImpuse;
	if (fabsf(jt) < j * sf) {
		tangentImpuse = t * jt;
	}
	else {
		tangentImpuse = t * -j * df;
	}

	A.velocity = A.velocity - tangentImpuse *  invMass1;
	B.velocity = B.velocity + tangentImpuse *  invMass2;

	A.angVel = A.angVel - Cross(r1, tangentImpuse) *  i1;
	B.angVel = B.angVel + Cross(r2, tangentImpuse) *  i2;
}