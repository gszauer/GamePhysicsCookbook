#include "Scene.h"
#include <algorithm>
#include <list>

void Scene::AddModel(Model* model) {
	if (std::find(objects.begin(), objects.end(), model) != objects.end()) {
		// Duplicate object, don't add
		return;
	}
	objects.push_back(model);
}

void Scene::RemoveModel(Model* model) {
	objects.erase(std::remove(objects.begin(), objects.end(), model), objects.end());
}

void Scene::UpdateModel(Model* model) {
	// Stub
}

std::vector<Model*> Scene::FindChildren(const Model* model) {
	std::vector<Model*> result;

	for (int i = 0, size = objects.size(); i < size; ++i) {
		if (objects[i] == 0 || objects[i] == model) {
			continue;
		}

		Model* iterator = objects[i]->parent;
		if (iterator != 0) {
			if (iterator == model) {
				result.push_back(objects[i]);
				continue;
			}
			iterator = iterator->parent;
		}
	}

	return result;
}

Model* Scene::Raycast(const Ray& ray) {
	if (octree != 0) {
		// :: lets the compiler know to look outside class scope
		return ::Raycast(octree, ray);
	}

	Model* result = 0;
	float result_t = -1;

	for (int i = 0, size = objects.size(); i < size; ++i) {
		float t = ModelRay(*objects[i], ray);
		if (result == 0 && t >= 0) {
			result = objects[i];
			result_t = t;
		}
		else if (result != 0 && t < result_t) {
			result = objects[i];
			result_t = t;
		}
	}

	return result;
}

std::vector<Model*> Scene::Query(const Sphere& sphere) {
	if (octree != 0) {
		// :: lets the compiler know to look outside class scope
		return ::Query(octree, sphere);
	}

	std::vector<Model*> result;
	for (int i = 0, size = objects.size(); i < size; ++i) {
		OBB bounds = GetOBB(*objects[i]);
		if (SphereOBB(sphere, bounds)) {
			result.push_back(objects[i]);
		}
	}
	return result;
}

std::vector<Model*> Scene::Query(const AABB& aabb) {
	if (octree != 0) {
		// :: lets the compiler know to look outside class scope
		return ::Query(octree, aabb);
	}

	std::vector<Model*> result;
	for (int i = 0, size = objects.size(); i < size; ++i) {
		OBB bounds = GetOBB(*objects[i]);
		if (AABBOBB(aabb, bounds)) {
			result.push_back(objects[i]);
		}
	}
	return result;
}

void SplitTree(OctreeNode* node, int depth) {
	if (depth-- <= 0) { // Decrements depth
		return;
	}

	if (node->children == 0) {
		node->children = new OctreeNode[8];

		vec3 c = node->bounds.position;
		vec3 e = node->bounds.size *0.5f;

		node->children[0].bounds = AABB(c + vec3(-e.x, +e.y, -e.z), e);
		node->children[1].bounds = AABB(c + vec3(+e.x, +e.y, -e.z), e);
		node->children[2].bounds = AABB(c + vec3(-e.x, +e.y, +e.z), e);
		node->children[3].bounds = AABB(c + vec3(+e.x, +e.y, +e.z), e);
		node->children[4].bounds = AABB(c + vec3(-e.x, -e.y, -e.z), e);
		node->children[5].bounds = AABB(c + vec3(+e.x, -e.y, -e.z), e);
		node->children[6].bounds = AABB(c + vec3(-e.x, -e.y, +e.z), e);
		node->children[7].bounds = AABB(c + vec3(+e.x, -e.y, +e.z), e);
	}

	if (node->children != 0 && node->models.size() > 0) {
		for (int i = 0; i < 8; ++i) { // For each child
			for (int j = 0, size = node->models.size(); j < size; ++j) {
				OBB bounds = GetOBB(*node->models[j]);
				if (AABBOBB(node->children[i].bounds, bounds)) {
					node->children[i].models.push_back(node->models[j]);
				}
			}
		}
		node->models.clear();

		// Recurse
		for (int i = 0; i < 8; ++i) {
			SplitTree(&(node->children[i]), depth);
		}
	}
}

void Insert(OctreeNode* node, Model* model) {
	OBB bounds = GetOBB(*model);
	if (AABBOBB(node->bounds, bounds)) {
		if (node->children == 0) {
			node->models.push_back(model);
		}
		else {
			for (int i = 0; i < 8; ++i) {
				Insert(&(node->children[i]), model);
			}
		}
	}
}

void Remove(OctreeNode* node, Model* model) {
	if (node->children == 0) {
		std::vector<Model*>::iterator it = std::find(node->models.begin(), node->models.end(), model);
		if (it != node->models.end()) {
			node->models.erase(it);
		}
	}
	else {
		for (int i = 0; i < 8; ++i) {
			Remove(&(node->children[i]), model);
		}
	}
}

void Update(OctreeNode* node, Model* model) {
	Remove(node, model);
	Insert(node, model);
}

Model* FindClosest(const std::vector<Model*>& set, const Ray& ray) {
	if (set.size() == 0) {
		return 0;
	}

	Model* closest = 0;
	float closest_t = -1;

	for (int i = 0, size = set.size(); i < size; ++i) {
		float this_t = ModelRay(*set[i], ray);

		if (this_t < 0) {
			continue;
		}

		if (closest_t < 0 || this_t < closest_t) {
			closest_t = this_t;
			closest = set[i];
		}
	}

	return closest;
}

Model* Raycast(OctreeNode* node, const Ray& ray) {
	RaycastResult raycast;
	Raycast(node->bounds, ray, &raycast);
	float t = raycast.t;

	if (t >= 0) {
		if (node->children == 0) {
			return FindClosest(node->models, ray);
		}
		else {
			std::vector<Model*> results;
			for (int i = 0; i < 8; ++i) {
				Model* result = Raycast(&(node->children[i]), ray);
				if (result != 0) {
					results.push_back(result);
				}
			}
			return FindClosest(results, ray);
		}
	}

	return 0;
}

std::vector<Model*> Query(OctreeNode* node, const Sphere& sphere) {
	std::vector<Model*> result;

	if (SphereAABB(sphere, node->bounds)) {
		if (node->children == 0) {
			for (int i = 0, size = node->models.size(); i < size; ++i) {
				OBB bounds = GetOBB(*(node->models[i]));
				if (SphereOBB(sphere, bounds)) {
					result.push_back(node->models[i]);
				}
			}
		}
		else {
			for (int i = 0; i < 8; ++i) {
				std::vector<Model*> child = Query(&(node->children[i]), sphere);
				if (child.size() > 0) {
					result.insert(result.end(), child.begin(), child.end());
				}
			}
		}
	}

	return result;
}

std::vector<Model*> Query(OctreeNode* node, const AABB& aabb) {
	std::vector<Model*> result;

	if (AABBAABB(aabb, node->bounds)) {
		if (node->children == 0) {
			for (int i = 0, size = node->models.size(); i < size; ++i) {
				OBB bounds = GetOBB(*(node->models[i]));
				if (AABBOBB(aabb, bounds)) {
					result.push_back(node->models[i]);
				}
			}
		}
		else {
			for (int i = 0; i < 8; ++i) {
				std::vector<Model*> child = Query(&(node->children[i]), aabb);
				if (child.size() > 0) {
					result.insert(result.end(), child.begin(), child.end());
				}
			}
		}
	}

	return result;
}

bool Scene::Accelerate(const vec3& position, float size) {
	if (octree != 0) {
		return false;
	}

	vec3 min(position.x - size, position.y - size, position.z - size);
	vec3 max(position.x + size, position.y + size, position.z + size);

	// Construct tree root
	octree = new OctreeNode();
	octree->bounds = FromMinMax(min, max);
	octree->children = 0;
	for (int i = 0, size = objects.size(); i < size; ++i) {
		octree->models.push_back(objects[i]);
	}

	// Create tree
	SplitTree(octree, 5);
	return true;
}

std::vector<Model*> Scene::Cull(const Frustum& f) {
	std::vector<Model*> result;

	for (int i = 0, size = objects.size(); i < size; ++i) {
		objects[i]->flag = false;
	}

	if (octree == 0) {
		for (int i = 0, size = objects.size(); i < size; ++i) {
			OBB bounds = GetOBB(*(objects[i]));
			if (Intersects(f, bounds)) {
				result.push_back(objects[i]);
			}
		}
	}
	else {
		std::list<OctreeNode*> nodes;
		nodes.push_back(octree);

		while (nodes.size() > 0) {
			OctreeNode* active = *nodes.begin();
			nodes.pop_front();

			// Has child nodes
			if (active->children != 0) {
				for (int i = 0; i < 8; ++i) {
					AABB bounds = active->children[i].bounds;
					if (Intersects(f, bounds)) {
						nodes.push_back(&active->children[i]);
					}
				}
			}
			else { // Is leaf node
				for (int i = 0, size = active->models.size(); i < size; ++i) {
					if (!active->models[i]->flag) {
						OBB bounds = GetOBB(*(active->models[i]));
						if (Intersects(f, bounds)) {
							active->models[i]->flag = true;
							result.push_back(active->models[i]);
						}
					}
				}
			}
		}
	}

	return result;
}