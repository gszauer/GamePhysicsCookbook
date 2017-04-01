#ifndef _H_GEOMETRY_2D
#define _H_GEOMETRY_2D

#include "vectors.h"

#include <ostream>

typedef vec2 Point2D;

typedef struct Line2D {
	Point2D start;
	Point2D end;

	inline Line2D() {}
	inline Line2D(const Point2D& s, const Point2D& e) :
		start(s), end(e) {}
} Line2D;

typedef struct Circle {
	Point2D position;
	float radius;

	inline Circle() : radius(1.0f) {}
	inline Circle(const Point2D& p, float r):
		position(p), radius(r) {}
} Circle;

typedef struct Rectangle2D {
	Point2D origin;
	vec2 size;

	inline Rectangle2D() : size(1, 1) { }
	inline Rectangle2D(const Point2D& o, const vec2& s) :
		origin(o), size(s) { }
} Rectangle2D;

typedef struct OrientedRectangle {
	Point2D position;
	vec2 halfExtents;
	float rotation;

	inline OrientedRectangle() :
		halfExtents(1.0f, 1.0f), rotation(0.0f) { }
	inline OrientedRectangle(const Point2D& pos, const vec2& ext) :
		position(pos), halfExtents(ext), rotation(0.0f) { }
	inline OrientedRectangle(const Point2D& pos, const vec2& ext, float rot) :
		position(pos), halfExtents(ext), rotation(rot) { }
} OrientedRectangle;

typedef struct BoundingShape {
	int numCircles;
	Circle* circles;
	int numRectangles;
	Rectangle2D* rectangles;

	inline BoundingShape() :
		numCircles(0), circles(0),
		numRectangles(0), rectangles(0) { }
} BoundingShape;

typedef struct Interval2D {
	float min;
	float max;
} Interval2D;

float Length(const Line2D& line);
float LengthSq(const Line2D& line);
vec2 GetMin(const Rectangle2D& rect);
vec2 GetMax(const Rectangle2D& rect);
Rectangle2D FromMinMax(const vec2& min, const vec2& max);

bool PointOnLine(const Point2D& point, const Line2D& line);
bool PointInCircle(const Point2D& point, const Circle& circle);
bool PointInRectangle(const Point2D& point, const Rectangle2D& rectangle);
bool PointInOrientedRectangle(const Point2D& point, const OrientedRectangle& rectangle);

#define PointCircle(point, circle) \
	PointInCircle(point, circle)
#define CirclePoint(circle, point) \
	PointInCircle(point, circle)
#define PointRectangle(point, rect) \
	PointInRectangle(point, rect)
#define RectanglePoint(rect, point) \
	PointInRectangle(point, rect)
#define PointOrientedRectangle(point, rect) \
	PointInOrientedRectangle(point, rect)
#define OrientedRectanglePoint(rect, point) \
	PointInOrientedRectangle(point, rect)

bool LineCircle(const Line2D& line, const Circle& circle);
bool LineRectangle(const Line2D& line, const Rectangle2D& rectangle);
bool LineOrientedRectangle(const Line2D& line, const OrientedRectangle& rectangle);

#define PointLine(point, line) PointOnLine(point, line)
#define LinePoint(line, point) PointOnLine(point, line)
#define CircleLine(circle, line) LineCircle(line, circle)
#define RectangleLine(rectangle, line) \
	LineRectangle(line, rectangle);
#define OrientedRectangleLine(rectangle, line) \
	LineOrientedRectangle(line, rectangle);

std::ostream& operator<<(std::ostream& os, const Line2D& shape);
std::ostream& operator<<(std::ostream& os, const Circle& shape);
std::ostream& operator<<(std::ostream& os, const Rectangle2D& shape);
std::ostream& operator<<(std::ostream& os, const OrientedRectangle& shape);

bool CircleCircle(const Circle& circle1, const Circle& circle2);
bool CircleRectangle(const Circle& circle, const Rectangle2D& rect);
#define RectangleCircle(rectangle, circle) \
	CircleRectangle(circle, rectangle)
bool CircleOrientedRectangle(const Circle& circle, const OrientedRectangle& rect);
#define OrientedRectangleCircle(rectangle, circle) \
	CircleOrientedRectangle(circle, rectangle)
bool RectangleRectangle(const Rectangle2D& rect1, const Rectangle2D& rect2);

Interval2D GetInterval(const Rectangle2D& rect, const vec2& axis);
bool OverlapOnAxis(const Rectangle2D& rect1, const Rectangle2D& rect2, const vec2& axis);
bool RectangleRectangleSAT(const Rectangle2D& rect1, const Rectangle2D& rect2);

Interval2D GetInterval(const OrientedRectangle& rect, const vec2& axis);
bool OverlapOnAxis(const Rectangle2D& rect1, const OrientedRectangle& rect2, const vec2& axis);
bool RectangleOrientedRectangle(const Rectangle2D& rect1, const OrientedRectangle& rect2);
#define OrientedRectangleRectangle(oriented, regular) \
	RectangleOrientedRectangle(regular, oriented)

bool OverlapOnAxis(const OrientedRectangle& rect1, const OrientedRectangle& rect2, const vec2& axis);
bool OrientedRectangleOrientedRectangleSAT(const OrientedRectangle& rect1, const OrientedRectangle& rect2);
bool OrientedRectangleOrientedRectangle(const OrientedRectangle& rect1, const OrientedRectangle& rect2);

Circle ContainingCircle(Point2D* pointArray, int arrayCount);
Circle ContainingCircleAlt(Point2D* pointArray, int arrayCount);
Rectangle2D ContainingRectangle(Point2D* pointArray, int arrayCount);


bool PointInShape(const BoundingShape& shape, const Point2D& point);
bool LineShape(const Line2D& line, const BoundingShape& shape);
bool CircleShape(const Circle& circle, const BoundingShape& shape);
bool RectangleShape(const Rectangle2D& rectangle, const BoundingShape& shape);
bool OrientedRectangleShape(const OrientedRectangle& rect, const BoundingShape& shape);

#define ShapeLine(shape, line) \
	LineShape(line, shape)
#define ShapeCircle(shape, circle) \
	CircleShape(circle, shape)
#define ShapeRectangle(shape, rectangle) \
	RectangleShape(rectangle, shape)
#define ShapeOrientedRectangle(shape, rect) \
	OrientedRectangleShape(rect, shape)

#endif