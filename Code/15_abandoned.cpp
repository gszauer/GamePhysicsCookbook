void ResetCollisionResult(CollisionResult* result) {
	if (result != 0) {
		result->colliding = false;
		result->normal = vec3(0, 0, 1);
		result->depth = 0.0f;
	}
}

std::vector<vec3> GetFaceNormals(const AABB& aabb) {
	std::vector<vec3> result;
	result.reserve(6);
	result.push_back(vec3( 1, 0, 0));
	result.push_back(vec3(-1, 0, 0));
	result.push_back(vec3(0,  1, 0));
	result.push_back(vec3(0, -1, 0));
	result.push_back(vec3(0, 0,  1));
	result.push_back(vec3(0, 0, -1));
	return result;
}

std::vector<vec3> GetFaceNormals(const OBB& obb) {
	const float* o = obb.orientation.asArray;
	std::vector<vec3> result;
	result.reserve(6);

	vec3 X(o[0], o[1], o[2]);
	vec3 Y(o[3], o[4], o[5]);
	vec3 Z(o[6], o[7], o[8]);
	Normalize(X);
	Normalize(Y);
	Normalize(Z);

	result.push_back( X);
	result.push_back(X * -1.0f);
	result.push_back(Y);
	result.push_back(Y * -1.0f);
	result.push_back(Z);
	result.push_back(Z * -1.0f);

	return result;
}

std::vector<vec3> GetFaceNormals(const Triangle& t) {
	std::vector<vec3> result;
	vec3 n = Normalized(Cross(t.b - t.a, t.c - t.a));
	result.push_back(n);
	return result;
}

bool LinePlaneIntersection(const Line& line, const Plane& plane, Point* hitPoint) {
	if (hitPoint != 0) {
		*hitPoint = line.start;
	}
	vec3 ab = line.end - line.start;

	float nA = Dot(plane.normal, line.start);
	float nAB = Dot(plane.normal, ab);

	if (CMP(nAB, 0)) {
		return false;
	}

	float t = (plane.distance - nA) / nAB;
	if (t >= 0.0f && t <= 1.0f) {
		*hitPoint = line.start + (line.end - line.start) * t;
		return true;
	}

	return false;
}

std::vector<Plane> GetPlanes(const AABB& aabb) {
	vec3 c = aabb.position; // center
	vec3 e = aabb.size;     // extents

	std::vector<Plane> result;
	result.resize(6);

	result[0] = Plane(vec3( 1,  0,  0),  c.x + e.x);
	result[1] = Plane(vec3(-1,  0,  0), -(c.x - e.x));
	result[2] = Plane(vec3( 0,  1,  0),  c.y + e.y);
	result[3] = Plane(vec3( 0, -1,  0), -(c.y - e.y));
	result[4] = Plane(vec3( 0,  0,  1),  c.z + e.z);
	result[5] = Plane(vec3( 0,  0, -1), -(c.z - e.z));

	return result;
}

std::vector<Plane> GetPlanes(const OBB& obb) {
	vec3 c = obb.position;	// OBB Center
	vec3 e = obb.size;		// OBB Extents
	const float* o = obb.orientation.asArray;
	vec3 a[] = {			// OBB Axis
		vec3(o[0], o[1], o[2]),
		vec3(o[3], o[4], o[5]),
		vec3(o[6], o[7], o[8]),
	};
	Normalize(a[0]);
	Normalize(a[1]);
	Normalize(a[2]);

	std::vector<Plane> result;
	result.resize(6);

	result[0] = Plane(a[0]        ,  Dot(a[0], (c + a[0] * e.x)));
	result[1] = Plane(a[0] * -1.0f, -Dot(a[0], (c - a[0] * e.x)));
	result[2] = Plane(a[1]        ,  Dot(a[1], (c + a[1] * e.y)));
	result[3] = Plane(a[1] * -1.0f, -Dot(a[1], (c - a[1] * e.y)));
	result[4] = Plane(a[2]        ,  Dot(a[2], (c + a[2] * e.z)));
	result[5] = Plane(a[2] * -1.0f, -Dot(a[2], (c - a[2] * e.z)));

	return result;
}

std::vector<Plane> GetPlanes(const Triangle& triangle) {
	std::vector<Plane> result;
	Plane plane = FromTriangle(triangle);
	result.push_back(plane);
	return result;
}

std::vector<Point> GetVertices(const AABB& aabb) {
	std::vector<vec3> v;
	v.resize(8);
	v[0].x = v[1].x = v[2].x = v[3].x = GetMin(aabb).x;
	v[4].x = v[5].x = v[6].x = v[7].x = GetMax(aabb).x;
	v[2].y = v[3].y = v[6].y = v[7].y = GetMin(aabb).y;
	v[0].y = v[1].y = v[4].y = v[5].y = GetMax(aabb).y;
	v[1].z = v[3].z = v[5].z = v[7].z = GetMin(aabb).z;
	v[0].z = v[2].z = v[4].z = v[6].z = GetMax(aabb).z;
	return v;
}

std::vector<Point> GetVertices(const OBB& obb) {
	std::vector<vec3> v;
	v.resize(8);

	vec3 C = obb.position;	// OBB Center
	vec3 E = obb.size;		// OBB Extents
	const float* o = obb.orientation.asArray;
	vec3 A[] = {			// OBB Axis
		vec3(o[0], o[1], o[2]),
		vec3(o[3], o[4], o[5]),
		vec3(o[6], o[7], o[8]),
	};

	v[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
	v[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
	v[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2];
	v[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
	v[4] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
	v[5] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
	v[6] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
	v[7] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2];

	return v;
}

std::vector<Point> GetVertices(const Triangle& t) {
	std::vector<vec3> v;
	v.reserve(3);
	v.push_back(t.a);
	v.push_back(t.b);
	v.push_back(t.c);
	return v;
}

std::vector<Line> GetEdges(const AABB& aabb) {
	std::vector<Line> result;
	result.reserve(12);
	std::vector<Point> v = GetVertices(aabb);

	int i[][2] = { // Indices of edges
		{0, 1}, {0, 2}, {1, 3}, {3, 2}, {5, 4}, {5, 7},
		{4, 6}, {6, 7}, {1, 5}, {0, 4}, {3, 7}, {2, 6}
	};

	for (int j = 0; j < 12; ++j) {
		result.push_back(Line(
			v[i[j][0]], v[i[j][1]]
		));
	}

	return result;
}

std::vector<Line> GetEdges(const OBB& obb) {
	std::vector<Line> result;
	result.reserve(12);
	std::vector<Point> v = GetVertices(obb);

	int index[][2] = { // Indices of edges
		{ 6, 1 },{ 6, 3 },{ 6, 4 },{ 2, 7 },{ 2, 5 },{ 2, 0 },
		{ 0, 1 },{ 0, 3 },{ 7, 1 },{ 7, 4 },{ 4, 5 },{ 5, 3 }
	};

	for (int j = 0; j < 12; ++j) {
		result.push_back(Line(
			v[index[j][0]], v[index[j][1]]
		));
	}

	return result;
}

std::vector<Line> GetEdges(const Triangle& t) {
	std::vector<Line> result;
	result.reserve(3);
	result.push_back(Line(t.a, t.b));
	result.push_back(Line(t.b, t.c));
	result.push_back(Line(t.c, t.a));
	return result;
}

Interval GetInterval(const Sphere& sphere, const vec3& axis) {
	Interval result;
	float v1 = Dot(axis, sphere.position - axis * sphere.radius);
	float v2 = Dot(axis, sphere.position + axis * sphere.radius);
	result.min = fminf(v1, v2);
	result.max = fmaxf(v1, v2);
	return result;
}

float PenetrationDepth(const OBB& obb, const Sphere& sphere, const vec3& axis, bool* outShouldFlip) {
	Interval i1 = GetInterval(obb, Normalized(axis));
	Interval i2 = GetInterval(sphere, Normalized(axis));

	if (!((i2.min <= i1.max) && (i1.min <= i2.max))) {
		return 0.0f; // No penerattion
	}

	float len1 = i1.max - i1.min;
	float len2 = i2.max - i2.min;
	float min = fminf(i1.min, i2.min);
	float max = fmaxf(i1.max, i2.max);
	float length = max - min;

	if (outShouldFlip != 0) {
		*outShouldFlip = (i2.min < i1.min);
	}

	return (len1 + len2) - length;
}