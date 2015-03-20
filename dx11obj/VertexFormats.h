#ifndef VERTEXFORMATS_H
#define VERTEXFORMATS_H

#include "Math.h"

class VertexParticle {
public:
	Vector3f Position;
	float Size;
	float Alpha;
};

// texture vertexes
class VertexNormalTextured {
public:
	Vector3f Position;
	Vector3f Normal;
	Vector2f TexUV;
};

#endif