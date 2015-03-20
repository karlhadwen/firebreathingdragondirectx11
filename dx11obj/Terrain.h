#ifndef TERRAIN_H
#define TERRAIN_H

#include "OBJEntity.h"

class Terrain : public OBJEntity {
public:
	Terrain() : OBJEntity(L"Media\\Terrain\\terrain.obj") {}
};

#endif