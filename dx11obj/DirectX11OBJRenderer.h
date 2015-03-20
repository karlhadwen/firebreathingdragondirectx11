#ifndef DIRECTX11OBJRENDERER_H
#define DIRECTX11OBJRENDERER_H

#include "DirectX11TexturedShader.h"
#include "OBJReader.h"
#include "OBJVertexBufferBuilder.h"
#include "DirectX11Texture.h"
#include "DirectX11VertexBuffer.h"

class OBJRenderer {
public:
	OBJVertexBufferBuilder< DirectX11VertexBuffer< VertexNormalTextured>, DirectX11Texture > m_VB;
	Vector3f m_BoundingMax;
	Vector3f m_BoundingMin;

	void Load(const std::wstring& path) {
		OBJReader reader;
		reader.LoadObj(path);

		m_VB.Build(reader);
		m_BoundingMax = reader.BoundingMax;
		m_BoundingMin = reader.BoundingMin;
	}

	void Render(XMMATRIX& objMat) {
		DirectX11TexturedVertexShader* m_VS = DirectX11TexturedVertexShader::Instance;
		DirectX11TexturedPixelShader* m_PS = DirectX11TexturedPixelShader::Instance;

		DirectX11TexturedVertexShader::ProjectionData pd;
		pd.matProjection = XMMatrixTranspose(Engine.m_Renderer.m_Viewport.m_Projection);

		//XMMATRIX matCubeWorld = XMMatrixIdentity();

		XMMATRIX matWVP = objMat * Engine.m_Camera.View * Engine.m_Renderer.m_Viewport.m_Projection;

		DirectX11TexturedVertexShader::ViewData vd;

		XMMATRIX cpy = XMMATRIX(objMat);

		vd.matWorld = XMMatrixTranspose(cpy);
		vd.matWorldViewProjection = XMMatrixTranspose(matWVP);

		for (int i = 0; i < m_VB.m_Parts.size(); i++) {
			auto curPart = m_VB.m_Parts[i];
			m_PS->m_Texture = *curPart->m_Texture;
			m_PS->Render();

			m_VS->m_Vertices = curPart->m_Buffer;
			m_VS->m_Projection = pd;
			m_VS->m_View = vd;
			m_VS->Render();
		}
	}
};

#endif