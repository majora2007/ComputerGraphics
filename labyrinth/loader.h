#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_

#include "mesh.h"
#include <string>

class ObjLoader
{
public:
	static Mesh* load(const char* file);

private:
};

#endif