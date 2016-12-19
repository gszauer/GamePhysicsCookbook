#ifndef _H_RIGID_SPHERE_
#define _H_RIGID_SPHERE_

#include "Rigidbody.h";

// TODO: Implementation specific!
#include "FixedFunctionPrimitives.h"
#include "RigidBox.h"

class RigidSphere : public Rigidbody {
protected:
	vec3 oldPosition;
	float radius;
public:

	inline RigidSphere() {
		position = vec3();
		velocity = vec3();
		mass = 1;
		forces = vec3();

		radius = 1;
		type = RIGIDBODY_TYPE_SPHERE;
	}

	inline virtual CollisionManifest IsColliding(Rigidbody& rb) {
		if (rb.GetType() == RIGIDBODY_TYPE_BOX) {
			RigidBox* other = (RigidBox*)&rb;

			Sphere s1(position, radius);
			OBB obb;
			obb.position = other->position;
			obb.size = other->box.size;
			obb.orientation = other->box.orientation;

			CollisionManifest cm = FindCollisionFeatures(obb, s1);
			return cm;
		}
		if (rb.GetType() == RIGIDBODY_TYPE_SPHERE) {
			RigidSphere* other = (RigidSphere*)&rb;
			Sphere s1(position, radius);
			Sphere s2(other->position, other->radius);
			return FindCollisionFeatures(s1, s2);
		}

		CollisionManifest result;
		ResetCollisionManifest(&result);
		return result;
	}

	inline void Render() {
		::Render(Sphere(position, radius));
	}
};

#endif
