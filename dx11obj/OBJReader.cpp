#include "stdafx.h"
#include "OBJReader.h"
#include <fstream>
#include "FileSystem.h"

using namespace std;

// used for modification and enabling the detailed file. There's nothing too important in
// this method but it makes life a lot easier.
class LineParser {
public:
	const wstring& str;
	int index;

	wchar_t breakChars[16];
	int breakCharCount;

	LineParser(const wstring& line) : str(line) {
		breakCharCount = 0;
		index = 0;
		addBreakChar(L' ');
		addBreakChar(L'\t');
	}

	int getCountOf(wchar_t chr) {
		int result = 0;
		for (int i = 0; i < str.length();i++) {
			if (str[i] == chr) {
				result++;
			}
		}
		return result;
	}

	void addBreakChar(wchar_t chr) {
		breakChars[breakCharCount++] = chr;
	}

	bool isBreakCharacter(wchar_t chr) {
		for (int i = 0; i < breakCharCount;i++) {
			if (chr == breakChars[i]) {
				return true;
			}
		}
		return false;
	}

	void skipWhiteSpace() {
		for (int i = index; i < str.length(); i++) {
			if (!isBreakCharacter(str[i])) {
				index = i;
				break;
			}
		}
	}

	// shouldn't be longer than 32 so we can skip the line
	// quite nifty
	void skip() {
		wchar_t result[32];
		parse(result);
	}

	void parse(wchar_t* result) {
		int k = 0;
		bool notfinished = false;
		for (int i = index; i < str.length();i++, k++) {
			if (isBreakCharacter(str[i])) {
				result[k] = 0;
				index = i;
				notfinished = true;
				break;
			}
			result[k] = str[i];
		}

		if (notfinished) {
			skipWhiteSpace();
		} else {
			result[k] = 0;
			index = str.length();
		}
	}

	wstring parseString() {
		wchar_t result[32];
		parse(result);
		return wstring(result);
	}

	int ParseInt() {
		wchar_t result[32];
		parse(result);
		return _wtoi(result);
	}

	float parseFloat() {
		wchar_t result[32];
		parse(result);
		return std::wcstod(result, NULL);
	}
};

// similar concept to what's held in your project nigel, but i decided to clean it up

// parse the mtl file for the vertex, line by line
void OBJReader::ParseVertexLine(LineParser& ln) {
	ln.skip(); // skip command
	
	// a 3 element vector, i must be crazy! (java has a better implementation)
	// pop in x,y,z cords
	Vector3f v;
	v.x = ln.parseFloat();
	v.y = ln.parseFloat();
	v.z = ln.parseFloat();

	if (BoundingMax.x < v.x) BoundingMax.x = v.x;
	if (BoundingMax.y < v.y) BoundingMax.y = v.y;
	if (BoundingMax.z < v.z) BoundingMax.z = v.z;
	if (v.x < BoundingMin.x) BoundingMin.x = v.x;
	if (v.y < BoundingMin.y) BoundingMin.y = v.y;
	if (v.z < BoundingMin.z) BoundingMin.z = v.z;

	// store them in the vertices vector
	Vertices.push_back(v);
}

// parse the mtl file for the texture, line by line
void OBJReader::ParseTextureLine(LineParser& ln) {
	ln.skip(); // skip command

	Vector2f v;
	v.x = ln.parseFloat();
	v.y = 1.0f - ln.parseFloat();

	// store them in the texture coordinates vector
	TextureCoordinates.push_back(v);
}

// parse for normal lines
void OBJReader::ParseNormalLine(LineParser& ln) {
	ln.skip(); // skip command

	Vector3f v;
	v.x = ln.parseFloat();
	v.y = ln.parseFloat();
	v.z = ln.parseFloat();

	// store them in the normals vector
	Normals.push_back(v);
}

// parse the faces line (a little tougher than i expected to get faces to work)
// we're basically look for the "f" start
void OBJReader::ParseFaceLine(LineParser& ln) {
	ln.skip();

	ln.addBreakChar('/');

	int slashCount = ln.getCountOf('/');

	if (slashCount == 6) {
		if (ln.str.find(L"//") > -1) {
			throw exception("texture coordinates are missing");
		}

		int v1 = ln.ParseInt();
		int t1 = ln.ParseInt();
		int n1 = ln.ParseInt();

		int v2 = ln.ParseInt();
		int t2 = ln.ParseInt();
		int n2 = ln.ParseInt();

		int v3 = ln.ParseInt();
		int t3 = ln.ParseInt();
		int n3 = ln.ParseInt();

		OBJFace* result = new OBJFace();
		result->Material = curMtl;
		result->Vertices[0].Position = Vertices[v1-1];
		result->Vertices[0].TexUV = TextureCoordinates[t1-1];
		result->Vertices[0].Normal = Normals[n1-1];

		result->Vertices[1].Position = Vertices[v2-1];
		result->Vertices[1].TexUV = TextureCoordinates[t2-1];
		result->Vertices[1].Normal = Normals[n2-1];

		result->Vertices[2].Position = Vertices[v3-1];
		result->Vertices[2].TexUV = TextureCoordinates[t3-1];
		result->Vertices[2].Normal = Normals[n3-1];

		curGroup->Faces.push_back(result);
	} else {
		throw exception("Unsupported obj format");
	}
}

// groups of the mtl file are pulled in and then we assign a new group as in
// my case there were three active groups (one didn't need parsing)
void OBJReader::ParseGroupLine(LineParser& ln) {
	ln.skip();

	OBJGroup* ng = new OBJGroup();
	ng->Name = ln.parseString();
	curGroup = ng;
	Groups.push_back(ng);
}

// parsing mtl libraries line by line, and skip over each line. I told you
// lineparsing makes life easier
void OBJReader::ParseMtlLibLine(LineParser& ln) {
	ln.skip();
	wstring mtlPath = ln.parseString();
	LoadMtl(ObjFolder + mtlPath);
}

// reading in materials and parsing them line by line
void OBJReader::ParseUseMtlLine(LineParser& ln) {
	ln.skip();
	wstring mtlName = ln.parseString();
	curMtl = GetMaterial(mtlName);

	if (curMtl == NULL) {
		throw exception("Material not found???");
	}
}

// parse line and determine what we're using from the mtl file
// whether it's a face/vt/vn/g/mtllib/usemtl, just pulling the things
// out that make the mesh loader work
void OBJReader::ParseLine(const std::wstring& line) {
	LineParser lp(line);

	if (line.compare(0,2, L"v ") == 0) {
		ParseVertexLine(lp);
	} else if (line.compare(0, 3, L"vt ") == 0) {
		ParseTextureLine(lp);
	} else if (line.compare(0, 3, L"vn ") == 0) {
		ParseNormalLine(lp);
	} else if (line.compare(0, 2, L"f ") == 0) {
		ParseFaceLine(lp);
	} else if (line.compare(0, 2, L"g ") == 0) {
		ParseGroupLine(lp);
	} else if (line.compare(0, 7, L"mtllib ") == 0) {
		ParseMtlLibLine(lp);
	} else if (line.compare(0, 7, L"usemtl ") == 0) {
		ParseUseMtlLine(lp);
	}
}

// load in the obj!
void OBJReader::LoadObj(const std::wstring& path) {
	if (loaded) {
		throw exception("Already loaded");
	}

	loaded = true;
	std::wifstream          fileStream;
	std::wstring            line;

	fileStream.open(path);
	bool isOpen = fileStream.is_open();		// used for debugging only.

	if (!isOpen) {
		fileStream.close();
		throw exception("file not found");
	}

	ObjFolder = Path::GetDirectoryName(path) + L"\\";
	BoundingMin.x = 0.0f;
	BoundingMin.y = 0.0f;
	BoundingMin.z = 0.0f;

	while (std::getline(fileStream, line)) {
		ParseLine(line);
	}
}

// load in the mtl file!
void OBJReader::LoadMtl(const std::wstring& path) {
	std::wifstream          fileStream;
	std::wstring            line;

	fileStream.open(path);
	bool isOpen = fileStream.is_open();	// used for debugging only.

	while (std::getline(fileStream, line)) {
		ParseMtlLine(line);
	}

	curMtl = NULL;
}

// parse each mtl line
void OBJReader::ParseMtlLine(const std::wstring& line) {
	LineParser lp(line);

	if (line.compare(0, 7, L"newmtl ") == 0) {
		ParseNewMtlLine(lp);
	} else if (line.compare(0, 7, L"map_Kd ") == 0) {
		ParseMapKdLine(lp);
	}
}

// get the materials
OBJMaterial* OBJReader::GetMaterial(std::wstring& name) {
	for (int i = 0; i < Materials.size();i++) {
		OBJMaterial* cmtl = Materials[i];
		if (cmtl->Name.compare(name) == 0) {
			return cmtl;
		}
	}

	return NULL;
}

// parse each new mtl line (32 generally)
void OBJReader::ParseNewMtlLine(LineParser& ln) {
	ln.skip();

	OBJMaterial* mtl = new OBJMaterial();
	mtl->Name = ln.parseString();
	Materials.push_back(mtl);
	curMtl = mtl;
}

// parse the kd line
void OBJReader::ParseMapKdLine(LineParser& ln) {
	ln.skip();
	curMtl->DiffuseMap = ln.parseString();
}

