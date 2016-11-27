#ifndef _H_OBJ_LOADER_
#define _H_OBJ_LOADER_

#include "Geometry3D.h"

bool LoadMesh(const char* szFilePath, Mesh* mesh);
void FreeMesh(Mesh* mesh);

#endif 
