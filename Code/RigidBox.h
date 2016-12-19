#ifndef _H_RIGID_BOX_
#define _H_RIGID_BOX_

#include "Rigidbody.h";

// Step one, move and rotate the box:
// http://gafferongames.com/virtual-go/how-the-go-stone-moves/
// Step two, take the inertia tensor into account
//	http://gafferongames.com/virtual-go/rotation-and-inertia-tensors/

// Step three, figure out how to do collision impulse
// This might help....
//----------------------------------------------------------
// 
// j =			                           -(1+Cor)(relv.norm)
//	     -----------------------------------------------------------------------------------
//	     norm.norm(1/Mass0 + 1/Mass1) + (sqr(r0 x norm) / Inertia0) + (sqr(r1 x norm) / Inertia1)
//
//----------------------------------------------------------
// This is where linear j comes from: https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
// The function void ResolveCollision( Object A, Object B ) explains it!

/* OpenTabs
http://gafferongames.com/game-physics/physics-in-3d/
http://gafferongames.com/virtual-go/how-the-go-stone-moves/
http://gafferongames.com/virtual-go/rotation-and-inertia-tensors/
http://www.xbdev.net/physics/RigidBodyImpulseCubes/

Searching for: c++ rigidbody collision impulse
*/

// TODO: Implementation specific!
#include "FixedFunctionPrimitives.h"

class RigidBox : public Rigidbody {
protected:
public:
	OBB box;

	inline RigidBox() {
		position = vec3();
		velocity = vec3();
		mass = 1;
		forces = vec3();

		type = RIGIDBODY_TYPE_BOX;
		//box.size = Random(vec3(0.1f, 0.1f, 0.1f), vec3(0.2f, 0.2f, 0.2f));
	}

	inline virtual CollisionManifest IsColliding(Rigidbody& rb) {
		if (rb.GetType() != type) {
			CollisionManifest result;
			ResetCollisionManifest(&result);
			return result;
		}
		RigidBox* other = (RigidBox*)&rb;
		box.position = position;
		other->box.position = other->position;

		return FindCollisionFeatures(box, other->box);
	}

	inline void Render() {
		box.position = position;
		::Render(box);
	}

	inline float Random(float min, float max) {
		if (max < min) {
			float t = min;
			min = max;
			max = t;
		}

		float random = ((float)rand()) / (float)RAND_MAX;

		float range = max - min;
		return (random*range) + min;
	}

	inline vec3 Random(vec3 min, vec3 max) {
		vec3 result;
		result.x = Random(min.x, max.x);
		result.y = Random(min.y, max.y);
		result.z = Random(min.z, max.z);
		return result;
	}
};

#endif
