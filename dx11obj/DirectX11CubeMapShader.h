#ifndef DIRECTX11CUBEMAPSHADER_H
#define DIRECTX11CUBEMAPSHADER_H

#include "Shader.h"
#include "DirectX11Shader.h"
#include "DirectX11Buffer.h"
#include "DirectX11VertexBuffer.h"
#include "VertexFormats.h"

class DirectX11CubeMapPixelShader: public DirectX11PixelShader {
public:
	static DirectX11CubeMapPixelShader* Instance;

	DirectX11PixelShaderTextureSlot<0> m_CubeMapTexture;
	DirectX11PixelShaderSamplerSlot<0> m_TextureSampler;

	DirectX11CubeMapPixelShader() {
		CompileFromFile(L"CubeMapPixelShader.hlsl", "main");

		m_TextureSampler.m_Sampler = new DirectX11PixelShaderSampler();
		m_TextureSampler.m_Sampler->Create();
	}

	void Render() {
		this->DirectX11PixelShader::Render();
		m_CubeMapTexture.Render();
		m_TextureSampler.Render();
	}
};

class DirectX11CubeMapVertexShader : public DirectX11VertexShader {
public:
	D3D_PRIMITIVE_TOPOLOGY m_Topology;

	static DirectX11CubeMapVertexShader* Instance;

	struct ViewData {
		XMMATRIX matWorld;
		XMMATRIX matWorldViewProjection;
	};

	DirectX11VSConstantBuffer<ViewData, 0> m_View;
	DirectX11VertexBufferSlot<VertexNormalTextured, 0> m_Vertices;

	void CompileInputLayout();

	void Render() {
		this->DirectX11VertexShader::Render();

		m_View.Render();

		Engine.m_Renderer.m_Context->IASetInputLayout(m_VertexLayout);
		m_Vertices.Render();

		Engine.m_Renderer.m_Context->IASetPrimitiveTopology(m_Topology);
		Engine.m_Renderer.m_Context->GSSetShader(NULL, NULL, 0);
		Engine.m_Renderer.m_Context->Draw(m_Vertices.m_Buffer->m_VertexCount, 0);
	}

	DirectX11CubeMapVertexShader() {
		CompileFromFile(L"CubeMapVertexShader.hlsl", "main");
		m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
};

#endif // DIRECTX11CUBEMAPSHADER_H
