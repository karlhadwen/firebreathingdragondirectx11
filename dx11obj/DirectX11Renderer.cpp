#include "stdafx.h"
#include "DirectX11Renderer.h"
#include "DirectX11CubeMapShader.h"
#include "OBJReader.h"

LRESULT DiretX11Window::Procedure(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KEYDOWN:
		m_Keys[wParam] = true;
		break;

	case WM_KEYUP:
		m_Keys[wParam] = false;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return this->Window::Procedure(msg, wParam, lParam);
}


void DirectX11Renderer::Initialize(int width, int height) {
	HRESULT hr = S_OK;

	m_Window.Create(width, height, L"DirectX11 Engine - Karl Hadwen");

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_Window.m_Hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, m_DriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, &m_FeatureLevel, &m_Context);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		throw exception("device creation failed");

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		throw exception("retrieving backbuffer failed");

	hr = m_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();

	if (FAILED(hr))
		throw exception("creating render target failed");

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_Device->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);

	if (FAILED(hr))
		throw exception("creating depth stencil failed");

	// create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		throw exception("creating depth stencil view failed");

	m_Context->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	
	m_Viewport.m_Width = width;
	m_Viewport.m_Height = height;
	m_Viewport.Update();

	// setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_Context->RSSetViewports(1, &vp);
}

void DirectX11Renderer::Render() {
	// clear the back buffer
	m_Context->ClearRenderTargetView(g_pRenderTargetView, ClearColor.Values);

	// clear the depth buffer to 1.0 (max depth)
	m_Context->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	RenderList();

	// present our back buffer to our front buffer
	m_SwapChain->Present(0, 0);
}

void DirectX11Renderer::EnableBlending(D3D11_BLEND src, D3D11_BLEND dst)
{
	D3D11_BLEND_DESC blendDesc;

	ZeroMemory(&blendDesc, sizeof (D3D11_BLEND_DESC));


	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = src;
	blendDesc.RenderTarget[0].DestBlend = dst;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //0x0f;
	// set blending

	ID3D11BlendState* blendState;
	m_Device->CreateBlendState(&blendDesc, &blendState);

	float blendFactor[4] = { 1, 1, 1, 1 };
	m_Context->OMSetBlendState(blendState, blendFactor, 0xffffffff);
}

void DirectX11Renderer::DisableBlending()
{
	D3D11_BLEND_DESC blendDesc;

	ZeroMemory(&blendDesc, sizeof (D3D11_BLEND_DESC));


	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //0x0f;
	// set blending

	ID3D11BlendState* blendState;
	m_Device->CreateBlendState(&blendDesc, &blendState);

	float blendFactor[4] = { 1, 1, 1, 1 };
	m_Context->OMSetBlendState(blendState, blendFactor, 0xffffffff);
}

