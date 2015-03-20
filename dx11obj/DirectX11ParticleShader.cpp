#include "stdafx.h"
#include "DirectX11ParticleShader.h"

DirectX11ParticlePixelShader* DirectX11ParticlePixelShader::Instance;
DirectX11ParticleGeometryShader* DirectX11ParticleGeometryShader::Instance;
DirectX11ParticleVertexShader* DirectX11ParticleVertexShader::Instance;

void DirectX11ParticleVertexShader::CompileInputLayout() {
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	HRESULT hr = Engine.m_Renderer.m_Device->CreateInputLayout(layout, numElements, m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), &m_VertexLayout);
	if (FAILED(hr))
		throw exception("Create input layout failed");
}
