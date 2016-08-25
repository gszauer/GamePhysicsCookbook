#ifndef _H_FIXED_FUNCTION_PRIMITIVES_
#define _H_FIXED_FUNCTION_PRIMITIVES_

void FixedFunctionSphere(int numDivisions, float radius);
void FixedFunctionCube(float extentsX, float extentsY, float extentsZ);
// http://www.povray.org/documentation/images/reference/mimxrtor.png
void FixedFunctionTorus(int majorSegments, int minorSegments, float majorRadius, float minorRadius);
void FixedFunctionCylinder(int slices, float height, float radius);


// Conveniance function
void FixedFunctionTorus(float outerRaidus, float innerRadius);
void FixedFunctionSphere();
void FixedFunctionCube();
void FixedFunctionCylinder(float height, float radius);

void FixedFunctionSubdivTetrahedron(float* a, float* b, float* c, int div, float r);

// TODO: Cone
// TODO Plane

#endif
