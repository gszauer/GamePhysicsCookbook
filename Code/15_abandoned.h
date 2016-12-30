typedef struct CollisionResult {
	bool colliding;
	vec3 normal;
	float depth;
	std::vector<vec3> contacts;
};

void ResetCollisionResult(CollisionResult* result);

bool LinePlaneIntersection(const Line& line, const Plane& plane, Point* hitPoint);

// Already defined!
/*Interval GetInterval(const AABB& aabb, const vec3& axis);
Interval GetInterval(const OBB& obb, const vec3& axis);
Interval GetInterval(const Triangle& triangle, const vec3& axis);*/

std::vector<Plane> GetPlanes(const AABB& aabb);
std::vector<Plane> GetPlanes(const OBB& obb);
std::vector<Plane> GetPlanes(const Triangle& triangle);

// These might not be needed!
std::vector<Point> GetVertices(const AABB& aabb);
std::vector<Point> GetVertices(const OBB& obb);
std::vector<Point> GetVertices(const Triangle& t);

std::vector<vec3> GetFaceNormals(const AABB& aabb);
std::vector<vec3> GetFaceNormals(const OBB& obb);
std::vector<vec3> GetFaceNormals(const Triangle& t);

std::vector<Line> GetEdges(const AABB& aabb);
std::vector<Line> GetEdges(const OBB& obb);
std::vector<Line> GetEdges(const Triangle& t);

static inline bool PointInside(const Point& p, const OBB& obb) {
	return PointInOBB(p, obb);
}

static inline bool PointInside(const Point& p, const AABB& aabb) {
	return PointInAABB(p, aabb);
}

static inline bool PointInside(const Point& p, const Triangle& triangle) {
	return PointInTriangle(p, triangle);
}

template <typename T, typename U>
static inline std::vector<Point> GetEdgeFacePoints(const T& o1, const U& o2) {
	std::vector<Point> result;

	std::vector<Plane> planes = GetPlanes(o1);
	std::vector<Line> edges = GetEdges(o2);

	for (int i = 0; i < planes.size(); ++i) {
		for (int j = 0; j < edges.size(); ++j) {
			Point intersection;
			if (LinePlaneIntersection(edges[j], planes[i], &intersection)) {
				if (PointInside(intersection, o1)) {
					result.push_back(intersection);
				}
			}
		}
	}

	return result;
}

template <typename T, typename U>
static inline float PenetrationLength(const T& o1, const U& o2, const vec3& axis) {
	Interval i1 = GetInterval(o1, Normalized(axis));
	Interval i2 = GetInterval(o2, Normalized(axis));

	if (!((i2.min <= i1.max) && (i1.min <= i2.max))) {
		return 0.0f; // No penerattion
	}

	float len1 = i1.max - i1.min;
	float len2 = i2.max - i2.min;
	float min = fminf(i1.min, i2.min);
	float max = fmaxf(i1.max, i2.max);
	float length = max - min;

	if (length > (len1 + len2)) {
		return 0.0f;
	}

	return (len1 + len2) - length;
}

template <typename T, typename U>
static inline CollisionResult SATIntersectionTest(vec3 center1, const T& o1, const U& o2) {
	CollisionResult result; // Will return result of intersection!
	ResetCollisionResult(&result);

	std::vector<vec3> o1Normals = GetFaceNormals(o1);
	std::vector<vec3> o2Normals = GetFaceNormals(o2);
	std::vector<Line> edges1 = GetEdges(o1);
	std::vector<Line> edges2 = GetEdges(o2);

	std::vector<vec3> axisOfSeperation;
	axisOfSeperation.reserve(o1Normals.size() + o2Normals.size() + (edges1.size() * edges2.size()));

	axisOfSeperation.insert(axisOfSeperation.end(), o1Normals.begin(), o1Normals.end());
	axisOfSeperation.insert(axisOfSeperation.end(), o2Normals.begin(), o2Normals.end());

	for (int i = 0; i < edges1.size(); ++i) {
		for (int j = 0; j < edges2.size(); ++j) {
			axisOfSeperation.push_back(
				Normalized(SatCrossEdge(
					edges1[i].start, edges1[i].end,
					edges2[j].start, edges2[j].end
				))
			);
		}
	}

	float minPenetration = FLT_MAX;
	vec3* hitNormal = 0;

	for (int i = 0; i < axisOfSeperation.size(); ++i) {
		float penertationLength = PenetrationLength<T, U>(o1, o2, axisOfSeperation[i]);
		if (penertationLength <= 0.0f) {
			ResetCollisionResult(&result);
			return result;
		}
		else if (penertationLength < minPenetration) {
			Interval a = GetInterval(o1, axisOfSeperation[i]);
			Interval b = GetInterval(o2, axisOfSeperation[i]);

			if (b.min < a.min) {
				axisOfSeperation[i] = axisOfSeperation[i] * -1.0f;
			}

			minPenetration = penertationLength;
			hitNormal = &axisOfSeperation[i];
		}
	}

	if (hitNormal == 0) {
		ResetCollisionResult(&result);
		return result;
	}

	vec3 axisOfPenertration = Normalized(*hitNormal);

	std::vector<Point> collision1 = GetEdgeFacePoints(o1, o2);
	std::vector<Point> collision2 = GetEdgeFacePoints(o2, o1);
	
	result.colliding = true;
	result.depth = minPenetration;
	result.normal = axisOfPenertration * -1.0f;

	result.contacts.insert(result.contacts.end(), collision1.begin(), collision1.end());
	result.contacts.insert(result.contacts.end(), collision2.begin(), collision2.end());

	Interval interval = GetInterval(o1, axisOfPenertration);
	float distance = (interval.max - interval.min)* 0.5f - minPenetration * 0.5f;
	vec3 pointOnPlane = center1 + axisOfPenertration * distance;
	for (int i = 0; i < result.contacts.size(); ++i) {
		result.contacts[i] = result.contacts[i] + (axisOfPenertration * Dot(axisOfPenertration, pointOnPlane - result.contacts[i]));
	}

	return result;
}

Interval GetInterval(const Sphere& sphere, const vec3& axis);
float PenetrationDepth(const OBB& obb, const Sphere& sphere, const vec3& axis, bool* outShouldFlip);