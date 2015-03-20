#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"
#include "Interfaces.h"
#include "FileSystem.h"

class Shader: public IResource, public IRenderable {
public:
	virtual void Release() = 0;
	virtual void Render() = 0;
};

#endif