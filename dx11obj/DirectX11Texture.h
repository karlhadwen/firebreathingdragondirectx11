#ifndef DIRECTX11TEXTURE_H
#define DIRECTX11TEXTURE_H

#include "Interfaces.h"
#include "Engine.h"

class DirectX11Texture : public IResource {
public:
	ID3D11ShaderResourceView* m_ResourceView;

	void Release() {
		m_ResourceView->Release();
	}

	void Create(const std::wstring& path) {
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(Engine.m_Renderer.m_Device, path.c_str(), NULL, NULL, &m_ResourceView, NULL);
		if (FAILED(hr))
			throw exception("Create texture failed");
	}
};

#endif
