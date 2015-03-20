#include "stdafx.h"
#include "DirectX11TexturedShader.h"

DirectX11TexturedPixelShader* DirectX11TexturedPixelShader::Instance;
DirectX11TexturedVertexShader* DirectX11TexturedVertexShader::Instance;

void DirectX11TexturedVertexShader::CompileInputLayout() {
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	HRESULT hr = Engine.m_Renderer.m_Device->CreateInputLayout(layout, numElements, m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), &m_VertexLayout);
	if (FAILED(hr))
		throw exception("Create input layout failed");
}
