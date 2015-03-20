#ifndef DIRECTX11TEXTUREDSHADER_H
#define DIRECTX11TEXTUREDSHADER_H

#include "Shader.h"
#include "DirectX11Shader.h"
#include "DirectX11Buffer.h"
#include "DirectX11VertexBuffer.h"
#include "VertexFormats.h"

class DirectX11TexturedPixelShader: public DirectX11PixelShader {
public:
	static DirectX11TexturedPixelShader* Instance;

	struct LightData {
		XMFLOAT4 materialColour;
		XMVECTOR vecLight;			// must be 4, we only use the first 3 (discovered by Roy).
	};

	DirectX11PSConstantBuffer<LightData, 0> m_Light;
	DirectX11PixelShaderTextureSlot<0> m_Texture;
	DirectX11PixelShaderSamplerSlot<0> m_TextureSampler;

	DirectX11TexturedPixelShader() {
		CompileFromFile(L"TexturedPixelShader.hlsl", "main");

		LightData ld;
		ld.materialColour = XMFLOAT4(1, 1, 1, 1);
		ld.vecLight = XMVector3Normalize(XMVectorSet(500, -500, 500, 0));
		m_Light = ld;

		m_TextureSampler.m_Sampler = new DirectX11PixelShaderSampler();
		m_TextureSampler.m_Sampler->Create();
	}

	void Render() {
		this->DirectX11PixelShader::Render();
		m_Light.Render();
		m_Texture.Render();
		m_TextureSampler.Render();
	}
};

class DirectX11TexturedVertexShader : public DirectX11VertexShader {
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static DirectX11TexturedVertexShader* Instance;
	
	struct ProjectionData {
		XMMATRIX matProjection;
	};

	struct ViewData {
		XMMATRIX matWorld;
		XMMATRIX matWorldViewProjection;
	};

	DirectX11VSConstantBuffer<ProjectionData, 0> m_Projection;
	DirectX11VSConstantBuffer<ViewData, 1> m_View;

	DirectX11VertexBufferSlot<VertexNormalTextured, 0> m_Vertices;

	void CompileInputLayout();

	void Render() {
		this->DirectX11VertexShader::Render();

		m_Projection.Render();
		m_View.Render();

		Engine.m_Renderer.m_Context->IASetInputLayout(m_VertexLayout);
		m_Vertices.Render();

		Engine.m_Renderer.m_Context->IASetPrimitiveTopology(m_Topology);

		Engine.m_Renderer.m_Context->GSSetShader(NULL, NULL, 0);
		Engine.m_Renderer.m_Context->Draw(m_Vertices.m_Buffer->m_VertexCount, 0);
	}

	DirectX11TexturedVertexShader() {
		CompileFromFile(L"TexturedVertexShader.hlsl", "main");
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
};

#endif // DIRECTX11TEXTUREDSHADER_H
