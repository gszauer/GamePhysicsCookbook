#ifndef _H_FIXED_FUNCTION_PRIMITIVES_
#define _H_FIXED_FUNCTION_PRIMITIVES_

#include "C:\Code\PhysicsBookDraftCode\Physics\Geometry2D.h"

void Render(const Point2D& point); 
void Render(const Line2D& line);
void Render(const Circle& circle);
void Render(const Rectangle2D& rect); 
void Render(const OrientedRectangle& rect);

void FixedFunctionSphere(int numDivisions, float radius);
void FixedFunctionCube(float extentsX, float extentsY, float extentsZ);
// http://www.povray.org/documentation/images/reference/mimxrtor.png
void FixedFunctionTorus(int majorSegments, int minorSegments, float majorRadius, float minorRadius);
void FixedFunctionCylinder(int slices, float height, float radius);
void FixedFunctionPlane(float size, int subdivs);

// Conveniance function
void FixedFunctionTorus(float outerRaidus, float innerRadius);
void FixedFunctionCylinder(float height, float radius);
void FixedFunctionCylinder(float height);
void FixedFunctionPlane(float size);
void FixedFunctionTorus();
void FixedFunctionSphere();
void FixedFunctionCube();
void FixedFunctionCylinder();
void FixedFunctionPlane();

// Helper functions
void FixedFunctionSubdivTetrahedron(float* a, float* b, float* c, int div, float r);

// TODO: Cone

#endif
