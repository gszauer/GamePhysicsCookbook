#ifndef _H_FIXED_FUNCTION_PRIMITIVES_
#define _H_FIXED_FUNCTION_PRIMITIVES_

#include "Geometry2D.h"
#include "Geometry3D.h"
#include "ObjLoader.h"
#include "Scene.h"
#include "matrices.h"

// 2D Geometry
void Render(const Point2D& point); 
void Render(const Line2D& line);
void Render(const Circle& circle);
void Render(const Rectangle2D& rect); 
void Render(const OrientedRectangle& rect);

// 3D Geometry
void Render(const Point& point);
void Render(const Line& line);
void Render(const Ray& ray);
void Render(const Sphere& sphere);
void Render(const AABB& aabb);
void RenderWithQuads(const AABB& aabb);
void Render(const OBB& obb);
void Render(const Plane& plane);
void Render(const Plane& plane, float scale);
void Render(const Triangle& triangle);
void Render(const Triangle& triangle, bool oneSided);
void Render(const Frustum& frustum);
void RenderNormals(const Frustum& frustum);
void Render(const CollisionManifold& manifold);

void Render(const Mesh& mesh);
void Render(const BVHNode& bvh);
void Render(const Model& model);

void Render(const std::vector<Line>& edges);

// Generic Primitives (All pointing up on Y axis, must be rotated)
void FixedFunctionSphere(int numDivisions, float radius);
void FixedFunctionCube(float extentsX, float extentsY, float extentsZ);
void FixedFunctionTorus(int majorSegments, int minorSegments, float majorRadius, float minorRadius);
void FixedFunctionCylinder(int slices, float height, float radius);
void FixedFunctionPlane(float size, int subdivs);
void FixedFunctionOrigin(bool depthTest, bool twoSided);
void FixedFunctionCone(int subdiv, float height, float radius);
void FixedFunctionCubeQuads(float extentsX, float extentsY, float extentsZ);

// Conveniance function
void FixedFunctionTorus(float outerRaidus, float innerRadius);
void FixedFunctionCylinder(float height, float radius);
void FixedFunctionCylinder(float height);
void FixedFunctionPlane(float size);
void FixedFunctionOrigin(bool depthTest);
void FixedFunctionCone(float height, float radius);
void FixedFunctionCone();
void FixedFunctionOrigin();
void FixedFunctionTorus();
void FixedFunctionSphere();
void FixedFunctionCube();
void FixedFunctionCylinder();
void FixedFunctionPlane();

// Helper functions
void FixedFunctionSubdivTetrahedron(float* a, float* b, float* c, int div, float r);
void FixedFunctionSubdivCone(float *v1, float *v2, int subdiv, float height, float radius);

#endif
