#ifndef _H_FIXED_FUNCTION_PRIMITIVES_
#define _H_FIXED_FUNCTION_PRIMITIVES_

void FixedFunctionSphere(int numDivisions, float radius);
void FixedFunctionCube(float extentsX, float extentsY, float extentsZ);
void FixedFunctionSubdivTetrahedron(float* a, float* b, float* c, int div, float r);

// http://www.povray.org/documentation/images/reference/mimxrtor.png
void FixedFunctionTorus(int majorSegments, int minorSegments, float majorRadius, float minorRadius);

// Conveniance function
void FixedFunctionTorus(float outerRaidus, float innerRadius);
void FixedFunctionSphere();
void FixedFunctionCube();

#endif
