#include "stdafx.h"
#include "DirectX11Shader.h"

#include "Shader.h"
#include <d3dcompiler.h>
#include <d3dx11.h>
#include "Engine.h"

void DirectX11PixelShader::Compile(const string& source, const string& aFunction) {
	ID3DBlob* pPSBlob = NULL;
	CompileBlob(source, aFunction, "ps_4_0");
	HRESULT hr = S_OK;

	hr = Engine.m_Renderer.m_Device->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), NULL, &m_PixelShader);
	if (FAILED(hr))
		throw exception("create pixel shader failed");
}

void DirectX11PixelShader::Render() {
	Engine.m_Renderer.m_Context->PSSetShader(m_PixelShader, NULL, 0);
}

void DirectX11GeometryShader::Compile(const string& source, const string& aFunction) {
	ID3DBlob* pPSBlob = NULL;
	CompileBlob(source, aFunction, "gs_4_0");
	HRESULT hr = S_OK;

	hr = Engine.m_Renderer.m_Device->CreateGeometryShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), NULL, &m_GeometryShader);
	if (FAILED(hr))
		throw exception("create Geometry shader failed");
}

void DirectX11GeometryShader::Render() {
	Engine.m_Renderer.m_Context->GSSetShader(m_GeometryShader, NULL, 0);
}

void DirectX11VertexShader::Compile(const string& source, const string& aFunction) {
	CompileBlob(source, aFunction, "vs_4_0");
	HRESULT hr = S_OK;

	hr = Engine.m_Renderer.m_Device->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(hr)) {
		m_Blob->Release();
		throw exception("create vertex shader failed");
	}

	CompileInputLayout();
}

void DirectX11VertexShader::Render() {
	Engine.m_Renderer.m_Context->VSSetShader(m_VertexShader, NULL, 0);
}

void DirectX11Shader::CompileBlob(const string& source, const string& entry, const string& target) {
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompile(source.c_str(), source.length(), NULL, NULL, NULL, entry.c_str(), target.c_str(), dwShaderFlags, 0, &m_Blob, &pErrorBlob);
	if (FAILED(hr)) {
		WCHAR errorCharsW[200];
		if (pErrorBlob != NULL) {
			Tools::CharToWideCharString(errorCharsW, (char *)pErrorBlob->GetBufferPointer());
			MessageBox(0, errorCharsW, L"Error", 0);
		}
	}
	if (pErrorBlob) {
		WCHAR errorCharsW[200];
		Tools::CharToWideCharString(errorCharsW, (char *)pErrorBlob->GetBufferPointer());
		// Log this text?
		//MessageBox(0, errorCharsW, L"Error", 0);
	}
	if (pErrorBlob) pErrorBlob->Release();
}
