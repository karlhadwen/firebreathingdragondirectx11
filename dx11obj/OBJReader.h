#ifndef OBJREADER_H
#define OBJREADER_H

#include <vector>
#include <string>
#include "Math.h"

#include "DirectX11TexturedShader.h"
typedef VertexNormalTextured OBJVertex;

// name of the materil and where we're gonna use map
class OBJMaterial {
public:
	std::wstring Name;
	std::wstring DiffuseMap;
};

// name of the material and the vertices it contains x,y,z
class OBJFace {
public:
	OBJMaterial* Material;
	OBJVertex Vertices[3];
};

// name of the group and vector to hold the faces to that particuar texture group
class OBJGroup {
public:
	std::wstring Name;
	std::vector< OBJFace* > Faces;
};

// use this later
class LineParser;

// reader for the group and mtl that we'll eventually map
class OBJReader {
protected:
	OBJGroup* curGroup;
	OBJMaterial* curMtl;
	bool loaded;

public:
	OBJReader() {
		loaded = false;
	}
	std::wstring ObjFolder;

	// oh it's all happening here!
	std::vector< Vector3f > Vertices;
	std::vector< Vector3f > Normals;
	std::vector< Vector2f > TextureCoordinates;
	std::vector< OBJGroup* > Groups;
	std::vector< OBJMaterial* > Materials;
	Vector3f BoundingMax;
	Vector3f BoundingMin;

	void LoadMtl(const std::wstring& path);
	void LoadObj(const std::wstring& path);

	OBJMaterial* GetMaterial(std::wstring& name);

protected:
	void ParseLine(const std::wstring& line);
	void ParseVertexLine(LineParser& ln);
	void ParseTextureLine(LineParser& ln);
	void ParseNormalLine(LineParser& ln);
	void ParseFaceLine(LineParser& ln);
	void ParseGroupLine(LineParser& ln);
	void ParseMtlLibLine(LineParser& ln);
	void ParseUseMtlLine(LineParser& ln);

	void ParseMtlLine(const std::wstring& line);
	void ParseNewMtlLine(LineParser& ln);
	void ParseMapKdLine(LineParser& ln);
};

#endif