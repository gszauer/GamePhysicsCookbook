#include "Geometry2D.h"
#include "matrices.h"
#include <cmath>
#include <cfloat>
#include <iostream>
#include "Compare.h"

#define CLAMP(number, minimum, maximum) \
	number = (number < minimum) ? minimum : \
		((number > maximum) ? maximum : number)

#define OVERLAP(minA, minB, maxA, maxB) \
	((minB <= maxA) && (minA <= maxB))

float Length(const Line2D& line) {
	return Magnitude(line.end - line.start);
}

float LengthSq(const Line2D& line) {
	return MagnitudeSq(line.end - line.start);
}

vec2 GetMin(const Rectangle2D& rect) {
	vec2 p1 = rect.origin;
	vec2 p2 = rect.origin + rect.size;

	return vec2(fminf(p1.x, p2.x), fminf(p1.y, p2.y));
}

vec2 GetMax(const Rectangle2D& rect) {
	vec2 p1 = rect.origin;
	vec2 p2 = rect.origin + rect.size;

	return vec2(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y));
}

Rectangle2D FromMinMax(const vec2& min, const vec2& max) {
	return Rectangle2D(min, max - min);
}

bool PointOnLine(const Point2D& point, const Line2D& line) {
	// Find the slope
	float M = (line.end.y - line.start.y) / (line.end.x - line.start.x);
	// Find the Y-Intercept
	float B = line.start.y - M * line.start.x;
	// Check line equation
	return CMP(point.y, M * point.x + B);
}

bool PointInCircle(const Point2D& point, const Circle& circle) {
	Line2D line(point, circle.position);
	if (LengthSq(line) < circle.radius * circle.radius) {
		return true;
	}
	return false;
}

bool PointInRectangle(const Point2D& point, const Rectangle2D& rectangle) {
	vec2 min = GetMin(rectangle);
	vec2 max = GetMax(rectangle);

	return	min.x <= point.x && min.y <= point.y &&
			point.x <= max.x && point.y <= max.y;
}

bool PointInOrientedRectangle(const Point2D& point, const OrientedRectangle& rectangle) {
	Rectangle2D localRectangle(Point2D(), rectangle.halfExtents * 2.0f);

	// First, we get a vector from the center of the rectangle
	// pointing to the point we want to test!
	vec2 rotVector = point - rectangle.position;

	// Second, we rotate that vector by the inverse of the rectangles
	// rotation (On the Z axis in 3D). We can either invert a matrix,
	// or construct a matrix from the negative rotation angle
	float theta = -DEG2RAD(rectangle.rotation);
	float zRotation2x2[] = { // Construct matrix
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	// Rotate vector
	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);

	// Now, the rotVector is in the local space of the world oriented rectangle.
	// But remember, it's a vector, not a point. Assume the rectangle is at 0,0
	// this means the center of the rotated rectangle is at 0,0. And rotVector points
	// to a point inside of this oriented box

	// Remember, the regular Rectangle class has 0,0 at bottom left! So, when we 
	// constructed the local space version of the oriented rectangle, we essentially
	// made an oriented rectangle with no orientation that was offset by half it's
	// size, so that bottom left is at origin. We have to apply this same offset
	// to the vector being rotated

	// Third we offset the point pointed to by the local roation vector by
	// half of the oriented bounding boxes extents. We do this because the
	// non oriented bounding box has its origin at bottom left, while the
	// oriented bounding box has its origin at center
	vec2 localPoint = rotVector + rectangle.halfExtents;

	// Finally, perform the local space point rectangle test!
	return PointInRectangle(localPoint, localRectangle);

	/* Alternate second step, Verbose
	vec3 pointToRotate(localPoint.x, localPoint.y, 0.0f);
	mat3 rotation = ZRotation3x3(-rectangle.rotation);
	vec3 result = MultiplyVector(pointToRotate, rotation);
	localPoint = vec2(result.x, result.y);*/

	// Alternate second step, With extras
	//localPoint = RotateVector(localPoint, -rectangle.rotation);
}

std::ostream& operator<<(std::ostream& os, const Line2D& shape) {
	os << "start: (" << shape.start.x << ", " << shape.start.y << "), end: (" << shape.end.x << ", " << shape.end.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Circle& shape) {
	os << "position: (" << shape.position.x << ", " << shape.position.y << "), radius: " << shape.radius;
	return os;
}
std::ostream& operator<<(std::ostream& os, const Rectangle2D& shape) {
	vec2 min = GetMin(shape);
	vec2 max = GetMax(shape);

	os << "min: (" << min.x << ", " << min.y << "), max: (" << max.x << ", " << max.y << ")";
	return os;
}
std::ostream& operator<<(std::ostream& os, const OrientedRectangle& shape) {
	os << "position: (" << shape.position.x << ", " << shape.position.y << "), half size: (";
	os << shape.halfExtents.x << ", " << shape.halfExtents.y << "), rotation: " << shape.rotation;
	return os;
}

bool LineCircle(const Line2D& line, const Circle& circle) {
	// Turn line into vector
	vec2 ab = line.end - line.start;

	// Project point (circle position) onto ab (line segment), computing the 
	// paramaterized position d(t) = a + t * (b - a)
	float t = Dot(circle.position - line.start, ab) / Dot(ab, ab);

	// Clamp T to a 0-1 range. If t was < 0 or > 1
	// then the closest point was outside the segment!
	if (t < 0.0f || t > 1.0f) {
		return false;
	}
	// We do not need to clamp t
	//t = (t < 0.0f) ? 0.0f : ((t > 1.0f)? 1.0f : t);

	// Find the closest point on the line segment
	Point2D closestPoint = line.start + ab * t;

	Line2D circleToClosest(circle.position, closestPoint);
	return LengthSq(circleToClosest) < circle.radius * circle.radius;
}

#if 1
bool LineRectangle(const Line2D& line, const Rectangle2D& rect) {
	if (PointInRectangle(line.start, rect) || PointInRectangle(line.end, rect)) {
		return true;
	}
	
	vec2 norm = Normalized(line.end - line.start);
	norm.x = (norm.x != 0) ? 1.0f / norm.x : 0;
	norm.y = (norm.y != 0) ? 1.0f / norm.y : 0;
	vec2 min = (GetMin(rect) - line.start) * norm;
	vec2 max = (GetMax(rect) - line.start) * norm;

	float tmin = fmaxf(fminf(min.x, max.x), fminf(min.y, max.y));
	float tmax = fminf(fmaxf(min.x, max.x), fmaxf(min.y, max.y));
	if (tmax < 0 || tmin > tmax) {
		return false;
	}
	float t = (tmin < 0.0f) ? tmax : tmin;
	return t > 0.0f && t * t < LengthSq(line);
}
#else // Same code, just a bit easyer to read!
bool LineRectangle(const Line2D& line, const Rectangle2D& rect) {
	// If either point of the line is in the rectangle, we have collision!
	if (PointInRectangle(line.start, rect) || PointInRectangle(line.end, rect)) {
		return true;
	}

	// Now we do a raycast
	vec2 min = GetMin(rect);
	vec2 max = GetMax(rect);
	vec2 norm = Normalized(line.end - line.start);

	float t1 = (min.x - line.start.x) / norm.x;
	float t2 = (max.x - line.start.x) / norm.x;
	float t3 = (min.y - line.start.y) / norm.y;
	float t4 = (max.y - line.start.y) / norm.y;

	float tmin = fmaxf(fminf(t1, t2), fminf(t3, t4));
	float tmax = fminf(fmaxf(t1, t2), fmaxf(t3, t4));

	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	// So the line segment is not intersecting
	if (tmax < 0) {
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) {
		return false;
	}

	float t = tmin;

	if (tmin < 0.0f) {
		// Intersection point is:
		// line.start + norm * tmax
		t = tmax;
	}

	// Intersection point is:
	// line.start + norm * tmin
	return t > 0.0f && t * t < LengthSq(line);

	

	/* This tests a line (not segment) against a Rectangle

	// First get the corners of the box
	Point2D c[] = { // corners
		rect.origin,
		rect.origin + rect.size,
		Point2D(rect.origin.x + rect.size.x, rect.origin.y),
		Point2D(rect.origin.x, rect.origin.y + rect.size.y)
	};

	// These variables are just to keep the determinant caluclations readable
	float xDelta = line.end.x - line.start.x;
	float yDelta = line.end.y - line.start.y;
	float sx = line.start.x;
	float sy = line.start.y;

	// Find the determinant of
	// | x2 - x1 |  x3 - x1 |
	// | y2 - y1 |  y3 - y1 |
	// for every corner. The result of the determinant for that matrix
	// will be positive if the point is on one side, negative if its
	// on the other side and 0 if the point is on the line.
	float d1 = (c[0].x - sx) * (yDelta) - (c[0].y - sy) * (xDelta);
	float d2 = (c[1].x - sx) * (yDelta) - (c[1].y - sy) * (xDelta);
	float d3 = (c[2].x - sx) * (yDelta) - (c[2].y - sy) * (xDelta);
	float d4 = (c[3].x - sx) * (yDelta) - (c[3].y - sy) * (xDelta);

	// If all points are on the same side of the line, we have a collision
	// We take advantage of the fact that multiplying two positive numbers
	// is always positive and multiplying two negative numbers is always
	// negative. The only way multiplying two determinant results is negative
	// is if two of the numbers where on the same side.
	return (!d1 * d2 <= 0 || d2 * d3 <= 0 || d3 * d4 <= 0); */
}
#endif

bool LineOrientedRectangle(const Line2D& line, const OrientedRectangle& rectangle) {
	float theta = -DEG2RAD(rectangle.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Line2D localLine;

	vec2 rotVector = line.start - rectangle.position;
	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	localLine.start  = rotVector + rectangle.halfExtents;
	
	rotVector = line.end - rectangle.position;
	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	localLine.end = rotVector + rectangle.halfExtents;
	
	Rectangle2D localRectangle(Point2D(), rectangle.halfExtents * 2.0f);
	return LineRectangle(localLine, localRectangle);
}

bool CircleCircle(const Circle& circle1, const Circle& circle2) {
	Line2D line(circle1.position, circle2.position);
	float radiiSum = circle1.radius + circle2.radius;
	return LengthSq(line) <= radiiSum * radiiSum;
}

bool CircleRectangle(const Circle& circle, const Rectangle2D& rect) {
	Point2D closestPoint = circle.position;
	vec2 min = GetMin(rect);
	vec2 max = GetMax(rect);
	closestPoint.x = (closestPoint.x < min.x) ? min.x :
		(closestPoint.x > max.x) ? max.x : closestPoint.x;
	closestPoint.y = (closestPoint.y < min.y) ? min.y :
		(closestPoint.y > max.y) ? max.y : closestPoint.y;

	Line2D line(circle.position, closestPoint);
	return LengthSq(line) <= circle.radius * circle.radius;
}

bool CircleOrientedRectangle(const Circle& circle, const OrientedRectangle& rect) {
	vec2 rotVector = circle.position - rect.position;
	float theta = -DEG2RAD(rect.rotation);
	float zRotation2x2[] = { 
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	
	Circle localCircle(rotVector + rect.halfExtents, circle.radius);
	Rectangle2D localRectangle(Point2D(), rect.halfExtents * 2.0f);

	return CircleRectangle(localCircle, localRectangle);
}

bool RectangleRectangle(const Rectangle2D& rect1, const Rectangle2D& rect2) {
	vec2 aMin = GetMin(rect1);
	vec2 aMax = GetMax(rect1);
	vec2 bMin = GetMin(rect2);
	vec2 bMax = GetMax(rect2);

	bool xOverlap = ((bMin.x <= aMax.x) && (aMin.x <= bMax.x));
	bool yOverlap = ((bMin.y <= aMax.y) && (aMin.y <= bMax.y));

	return xOverlap && yOverlap;
}

Interval2D GetInterval(const Rectangle2D& rect, const vec2& axis) {
	vec2 min = GetMin(rect);
	vec2 max = GetMax(rect);

	vec2 verts[] = { // Get all vertices of rect
		vec2(min.x, min.y),
		vec2(min.x, max.y),
		vec2(max.x, max.y),
		vec2(max.x, min.y)
	};

	// Set interval first projected vertex
	Interval2D result;
	result.min = Dot(axis, verts[0]);
	result.max = result.min;

	// For all other verts
	for (int i = 1; i < 4; ++i) {
		// Project vertex
		float projection = Dot(axis, verts[i]);
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;
	}

	return result;
}

bool OverlapOnAxis(const Rectangle2D& rect1, const Rectangle2D& rect2, const vec2& axis) {
	Interval2D a = GetInterval(rect1, axis);
	Interval2D b = GetInterval(rect2, axis);
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool RectangleRectangleSAT(const Rectangle2D& rect1, const Rectangle2D& rect2) {
	vec2 axisToTest[] = {
		vec2(1, 0),
		vec2(0, 1)
	};

	for (int i = 0; i < 2; ++i) {
		if (!OverlapOnAxis(rect1, rect2, axisToTest[i])) {
			// Intervals don't overlap, seperating axis found
			return false; // No collision has taken place
		}
	}

	// All intervals overlapped, seperating axis not found
	return true; // We have a collision
}

Interval2D GetInterval(const OrientedRectangle& rect, const vec2& axis) {
	Rectangle2D nonOrientedRect = Rectangle2D(Point2D(rect.position - rect.halfExtents), rect.halfExtents * 2.0f);
	vec2 min = GetMin(nonOrientedRect); 
	vec2 max = GetMax(nonOrientedRect);
	vec2 verts[] = { min, max, vec2(min.x, max.y), vec2(max.x, min.y) };

	float theta = DEG2RAD(rect.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };

	for (int i = 0; i < 4; ++i) {
		vec2 rotVector = verts[i] - rect.position;
		Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
		verts[i] = rotVector + rect.position;
	}

	// Set interval first projected vertex
	Interval2D result;
	result.min = result.max = Dot(axis, verts[0]);

	// For all other verts
	for (int i = 1; i < 4; ++i) {
		// Project vertex
		float projection = Dot(axis, verts[i]);
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;
	}

	return result;
}


bool OverlapOnAxis(const Rectangle2D& rect1, const OrientedRectangle& rect2, const vec2& axis) {
	Interval2D a = GetInterval(rect1, axis);
	Interval2D b = GetInterval(rect2, axis);
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool RectangleOrientedRectangle(const Rectangle2D& rect1, const OrientedRectangle& rect2) {
	vec2 axisToTest[]{
		vec2(1, 0),
		vec2(0, 1),
		Normalized(vec2(rect2.halfExtents.x, 0)),
		Normalized(vec2(0, rect2.halfExtents.y))
	};

	float theta = DEG2RAD(rect2.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Multiply(axisToTest[2].asArray, Normalized(vec2(rect2.halfExtents.x, 0)).asArray, 1, 2, zRotation2x2, 2, 2);
	Multiply(axisToTest[3].asArray, Normalized(vec2(0, rect2.halfExtents.y)).asArray, 1, 2, zRotation2x2, 2, 2);

	for (int i = 0; i < 4; ++i) {
		if (!OverlapOnAxis(rect1, rect2, axisToTest[i])) {
			// Intervals don't overlap, seperating axis found
			return false; // No collision has taken place
		}
	}

	// All intervals overlapped, seperating axis not found
	return true; // We have a collision
}

bool OverlapOnAxis(const OrientedRectangle& rect1, const OrientedRectangle& rect2, const vec2& axis) {
	Interval2D a = GetInterval(rect1, axis);
	Interval2D b = GetInterval(rect2, axis);
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool OrientedRectangleOrientedRectangleSAT(const OrientedRectangle& rect1, const OrientedRectangle& rect2) {
	vec2 axisToTest[]{
		vec2(1, 0),
		vec2(0, 1),
		vec2(), // TEMP
		vec2(), // TEMP
		vec2(), // TEMP
		vec2() // TEMP
	};

	// Collision axis of rect 2
	float theta = DEG2RAD(rect2.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Multiply(axisToTest[2].asArray, Normalized(vec2(rect2.halfExtents.x, 0)).asArray, 1, 2, zRotation2x2, 2, 2);
	Multiply(axisToTest[3].asArray, Normalized(vec2(0, rect2.halfExtents.y)).asArray, 1, 2, zRotation2x2, 2, 2);

	// Collision axis of rect 1
	theta = DEG2RAD(rect1.rotation);
	zRotation2x2[0] = cosf(theta);
	zRotation2x2[1] = sinf(theta);
	zRotation2x2[2] = -sinf(theta);
	zRotation2x2[3] = cosf(theta);
	Multiply(axisToTest[4].asArray, Normalized(vec2(rect1.halfExtents.x, 0)).asArray, 1, 2, zRotation2x2, 2, 2);
	Multiply(axisToTest[5].asArray, Normalized(vec2(0, rect1.halfExtents.y)).asArray, 1, 2, zRotation2x2, 2, 2);

	for (int i = 0; i < 6; ++i) {
		if (!OverlapOnAxis(rect1, rect2, axisToTest[i])) {
			// Intervals don't overlap, seperating axis found
			return false; // No collision has taken place
		}
	}

	// All intervals overlapped, seperating axis not found
	return true; // We have a collision
}

bool OrientedRectangleOrientedRectangle(const OrientedRectangle& r1, const OrientedRectangle& r2) {
	//return OrientedRectangleOrientedRectangleSAT(rect1, rect2);
	Rectangle2D localRect1(Point2D(), r1.halfExtents * 2.0f);
	OrientedRectangle localRect2(r2.position, r2.halfExtents, r2.rotation);

	localRect2.rotation = r2.rotation - r1.rotation;
	vec2 rotVector = r2.position - r1.position;
	float theta = -DEG2RAD(r1.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta) };
	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	localRect2.position = rotVector + r1.halfExtents;

	return RectangleOrientedRectangle(localRect1, localRect2);
}

Circle ContainingCircle(Point2D* pointArray, int arrayCount) {
	Point2D center; 
	for (int i = 0; i < arrayCount; ++i) {
		center = center + pointArray[i];
	}
	center = center * (1.0f / (float)arrayCount);

	Circle result(center, 1.0f);

	// Find the squared radius
	result.radius = MagnitudeSq(center - pointArray[0]);
	for (int i = 1; i < arrayCount; ++i) {
		float distance = MagnitudeSq(center - pointArray[i]);
		if (distance > result.radius) {
			result.radius = distance;
		}
	}

	// This has been squared until now
	result.radius = sqrtf(result.radius);
	return result;
}

Circle ContainingCircleAlt(Point2D* pointArray, int arrayCount) {
	vec2 min = pointArray[0];
	vec2 max = pointArray[0];

	for (int i = 0; i < arrayCount; ++i) {
		min.x = pointArray[i].x < min.x ? pointArray[i].x : min.x;
		min.y = pointArray[i].y < min.y ? pointArray[i].y : min.y;
		max.x = pointArray[i].x > max.x ? pointArray[i].x : max.x;
		max.y = pointArray[i].y > max.y ? pointArray[i].y : max.y;
	}

	return Circle((min + max) * 0.5f, Magnitude(max - min) * 0.5f);
}

Rectangle2D ContainingRectangle(Point2D* pointArray, int arrayCount) {
	vec2 min = pointArray[0];
	vec2 max = pointArray[0];

	for (int i = 0; i < arrayCount; ++i) {
		min.x = pointArray[i].x < min.x ? pointArray[i].x : min.x;
		min.y = pointArray[i].y < min.y ? pointArray[i].y : min.y;
		max.x = pointArray[i].x > max.x ? pointArray[i].x : max.x;
		max.y = pointArray[i].y > max.y ? pointArray[i].y : max.y;
	}

	return FromMinMax(min, max);
}




bool PointInShape(const BoundingShape& shape, const Point2D& point) {
	for (int i = 0; i < shape.numCircles; ++i) {
		if (PointInCircle(point, shape.circles[i])) {
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) {
		if (PointInRectangle(point, shape.rectangles[i])) {
			return true;
		}
	}
	return false;
}

bool LineShape(const Line2D& line, const BoundingShape& shape) {
	for (int i = 0; i < shape.numCircles; ++i) {
		if (LineCircle(line, shape.circles[i])) {
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) {
		if (LineRectangle(line, shape.rectangles[i])) {
			return true;
		}
	}
	return false;
}

bool CircleShape(const Circle& circle, const BoundingShape& shape) {
	for (int i = 0; i < shape.numCircles; ++i) {
		if (CircleCircle(circle, shape.circles[i])) {
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) {
		if (CircleRectangle(circle, shape.rectangles[i])) {
			return true;
		}
	}
	return false;
}

bool RectangleShape(const Rectangle2D& rectangle, const BoundingShape& shape) {
	for (int i = 0; i < shape.numCircles; ++i) {
		if (RectangleCircle(rectangle, shape.circles[i])) {
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) {
		if (RectangleRectangle(rectangle, shape.rectangles[i])) {
			return true;
		}
	}
	return false;
}

bool OrientedRectangleShape(const OrientedRectangle& rect, const BoundingShape& shape) {
	for (int i = 0; i < shape.numCircles; ++i) {
		if (OrientedRectangleCircle(rect, shape.circles[i])) {
			return true;
		}
	}
	for (int i = 0; i < shape.numRectangles; ++i) {
		if (OrientedRectangleRectangle(rect, shape.rectangles[i])) {
			return true;
		}
	}
	return false;
}