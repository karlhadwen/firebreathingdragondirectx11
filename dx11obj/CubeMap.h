#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "OBJEntity.h"

class CubeMap : public OBJEntity {
public:
	XMMATRIX m_CorrectingMatrix;
	D3D11_DEPTH_STENCIL_DESC m_StencilDescription;
	ID3D11DepthStencilState* m_DepthStencil;

	CubeMap() : OBJEntity(L"Media\\Other\\cubemap.obj") {
		m_CorrectingMatrix = XMMatrixIdentity();
		m_CorrectingMatrix *= XMMatrixRotationX(XMConvertToRadians(90.0f)); // Rotation by 90 degrees around X-axis is necessary for it to align properly. Is the world rotated to its side? Maybe this is because of the orientation in 3DSMax

		ZeroMemory(&m_StencilDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));
		m_StencilDescription.DepthEnable = FALSE;
		Engine.m_Renderer.m_Device->CreateDepthStencilState(&m_StencilDescription, &m_DepthStencil);
	}

	// update the position to be always where the camera is
	void Update() {
		m_Position = Engine.m_Camera.Eye;
	}

	// render objs
	void Render() {
		m_CorrectingMatrix.m[3][0] = m_Position.m128_f32[0];
		m_CorrectingMatrix.m[3][1] = m_Position.m128_f32[1];
		m_CorrectingMatrix.m[3][2] = m_Position.m128_f32[2];
		DirectX11CubeMapVertexShader::ViewData vd;
		vd.matWorld = XMMatrixTranspose(m_CorrectingMatrix);
		vd.matWorldViewProjection = XMMatrixTranspose(m_CorrectingMatrix * Engine.m_Camera.View * Engine.m_Renderer.m_Viewport.m_Projection);

		auto curPart = m_Renderer->m_VB.m_Parts[0];
		DirectX11CubeMapPixelShader::Instance->m_CubeMapTexture = *curPart->m_Texture;
		DirectX11CubeMapPixelShader::Instance->Render();

		DirectX11CubeMapVertexShader::Instance->m_Vertices = curPart->m_Buffer;
		DirectX11CubeMapVertexShader::Instance->m_View = vd;

		// Make sure to render Cube Map with depth stencil turned off so that it doesn't write anything to it, then set the depth stencil back to the original
		ID3D11DepthStencilState* oldDepthStencil = NULL;
		UINT depthStencilRef = 0;
		Engine.m_Renderer.m_Context->OMGetDepthStencilState(&oldDepthStencil, &depthStencilRef);
		Engine.m_Renderer.m_Context->OMSetDepthStencilState(m_DepthStencil, 0);
		DirectX11CubeMapVertexShader::Instance->Render();
		Engine.m_Renderer.m_Context->OMSetDepthStencilState(oldDepthStencil, depthStencilRef);
	}
};

#endif