#ifndef DIRECTX11SHADER_H
#define DIRECTX11SHADER_H

#include "Shader.h"
#include "DirectX11Texture.h"

class DirectX11Shader: public Shader {
public:
	ID3DBlob* m_Blob;

	void CompileBlob(const string& source, const string& entry, const string& target);
	virtual void Release() = 0;
	virtual void Render() = 0;
	virtual void Compile(const string& source, const string& aFunction) = 0;
	virtual void CompileFromFile(const wstring& path, const string& aFunction) {
		std::string conents = File::ReadAllText(path);
		Compile(conents, aFunction);
	}
};

class DirectX11PixelShader : public DirectX11Shader {
public:
	ID3D11PixelShader* m_PixelShader;
	DirectX11PixelShader() {
		m_PixelShader = NULL;
	}

	void Release() {
		m_PixelShader->Release();
	}

	void Compile(const string& source, const string& aFunction);

	void Render();
};

class DirectX11GeometryShader : public DirectX11Shader {
public:
	ID3D11GeometryShader* m_GeometryShader;
	DirectX11GeometryShader() {
		m_GeometryShader = NULL;
	}

	void Release() {
		m_GeometryShader->Release();
	}

	void Compile(const string& source, const string& aFunction);

	void Render();
};

class DirectX11VertexShader : public DirectX11Shader {
public:
	ID3D11VertexShader* m_VertexShader;
	ID3D11InputLayout* m_VertexLayout;

	DirectX11VertexShader() {
		m_VertexShader = NULL;
		m_VertexLayout = NULL;
	}

	void Initialize() {

	}

	void Release() {
		m_VertexShader->Release();
	}

	void Render();

	virtual void CompileInputLayout() = 0;
	void Compile(const string& source, const string& aFunction);
};

template<int Slot>
class DirectX11PixelShaderTextureSlot: public IRenderable {
public:
	DirectX11Texture* m_Texture;

	DirectX11PixelShaderTextureSlot<Slot>& operator = (DirectX11Texture& texture) {
		m_Texture = &texture;
		return *this;
	}

	void Render() {
		Engine.m_Renderer.m_Context->PSSetShaderResources(Slot, 1, &m_Texture->m_ResourceView);
	}
};

class DirectX11PixelShaderSampler {
public:
	ID3D11SamplerState* m_Sampler;

	void Create() {
		// create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HRESULT hr = Engine.m_Renderer.m_Device->CreateSamplerState(&sampDesc, &m_Sampler);
		if (FAILED(hr))
			throw exception("create sampler state failed");
	}
};

template <int Slot>
class DirectX11PixelShaderSamplerSlot : public IRenderable {
public:
	DirectX11PixelShaderSampler* m_Sampler;

	void Render() {
		Engine.m_Renderer.m_Context->PSSetSamplers(Slot, 1, &m_Sampler->m_Sampler);
	}
};

#endif