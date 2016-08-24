#ifndef _H_FIXED_FUNCTION_PRIMITIVES_
#define _H_FIXED_FUNCTION_PRIMITIVES_

void FixedFunctionSphere(int numDivisions, float radius);
void FixedFunctionCube(float extentsX, float extentsY, float extentsZ);
void FixedFunctionSubdivTetrahedron(float* a, float* b, float* c, int div, float r);

#endif
