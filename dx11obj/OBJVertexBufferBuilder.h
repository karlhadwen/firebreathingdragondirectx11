#ifndef OBJVERTEXBUFFERBUILDER_H
#define OBJVERTEXBUFFERBUILDER_H

#include "Interfaces.h"
#include "VertexFormats.h"

template < class vertbuf, class texture>
class OBJRenderPart {
public:
	texture* m_Texture;
	vertbuf m_Buffer;
};

template < class vertBuf, class texture>
class OBJVertexBufferBuilder {
public:
	typedef OBJRenderPart< vertBuf, texture > OBJPart;

	std::vector<OBJPart*> m_Parts;

	void Build(OBJReader& reader) {
		for (int i = 0; i < reader.Materials.size(); i++) {
			OBJMaterial* curmat = reader.Materials[i];

			if (curmat->DiffuseMap.empty()) {
				continue;
			}

			std::wstring finalPath = reader.ObjFolder + curmat->DiffuseMap;
			OBJPart* curpart = new OBJPart();
			texture* curtex = new texture();

			curtex->Create(finalPath);
			curpart->m_Texture = curtex;

			// group together faces with same texture
			for (int k = 0; k < reader.Groups.size(); k++) {
				OBJGroup* curgr = reader.Groups[k];
				
				// make sure all materials match and bild up the vertices
				for (int j = 0; j < curgr->Faces.size(); j++) {
					OBJFace* curface = curgr->Faces[j];
					if (curface->Material == curmat) {
						for (int o = 0; o < 3; o++) {
							curpart->m_Buffer.AddVertex(curface->Vertices[o]);
						}
					}
				}
			}

			curpart->m_Buffer.Create();
			m_Parts.push_back(curpart);
		}
	}
};

#endif