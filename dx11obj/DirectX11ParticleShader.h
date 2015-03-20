#ifndef DIRECTX11PARTICLESHADER_H
#define DIRECTX11PARTICLESHADER_H

#include "Shader.h"
#include "DirectX11Shader.h"
#include "DirectX11Buffer.h"
#include "DirectX11VertexBuffer.h"
#include "DirectX11IndexBuffer.h"
#include "VertexFormats.h"

class DirectX11ParticlePixelShader: public DirectX11PixelShader {
public:
	static DirectX11ParticlePixelShader* Instance;

	DirectX11PixelShaderTextureSlot<0> m_ParticleTexture;
	DirectX11PixelShaderSamplerSlot<0> m_TextureSampler;

	DirectX11ParticlePixelShader() {
		CompileFromFile(L"ParticlePixelShader.hlsl", "main");

		m_TextureSampler.m_Sampler = new DirectX11PixelShaderSampler();
		m_TextureSampler.m_Sampler->Create();
	}

	void Render() {
		this->DirectX11PixelShader::Render();
		m_ParticleTexture.Render();
		m_TextureSampler.Render();
	}
};

class DirectX11ParticleGeometryShader: public DirectX11GeometryShader {
public:
	static DirectX11ParticleGeometryShader* Instance;

	DirectX11ParticleGeometryShader() {
		CompileFromFile(L"ParticleGeometryShader.hlsl", "main");
	}

	void Render() {
		this->DirectX11GeometryShader::Render();
	}
};

class DirectX11ParticleVertexShader : public DirectX11VertexShader {
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static DirectX11ParticleVertexShader* Instance;

	struct ViewData {
		XMMATRIX matWorldViewProjection;
	};
	DirectX11VSConstantBuffer<ViewData, 0> m_WorldViewProjection;

	DirectX11VertexBufferSlot<VertexParticle, 0> m_Vertices;
	DirectX11IndexBufferSlot m_Indexes;

	void CompileInputLayout();

	void Render() {
		this->DirectX11VertexShader::Render();

		m_WorldViewProjection.Render();

		Engine.m_Renderer.m_Context->IASetInputLayout(m_VertexLayout);
		m_Vertices.Render();

		Engine.m_Renderer.m_Context->IASetPrimitiveTopology(m_Topology);
		m_Indexes.Render();
		Engine.m_Renderer.m_Context->DrawIndexed(m_Indexes.m_Buffer->m_IndexCount, 0, 0);
	}

	DirectX11ParticleVertexShader() {
		CompileFromFile(L"ParticleVertexShader.hlsl", "main");
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	}
};

#endif // DIRECTX11CUBEMAPSHADER_H
