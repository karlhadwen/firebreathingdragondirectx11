#ifndef DIRECTX11RENDERER_H
#define DIRECTX11RENDERER_H

#include "Renderer.h"
#include "Window.h"
#include "Color.h"
#include <D3DX11.h>

class DiretX11Window : public Window {
public:
	bool m_Keys[256];

	LRESULT Procedure(UINT msg, WPARAM wParam, LPARAM lParam);

};

class DirectX11Renderer: public IRenderer {
public:
	DiretX11Window m_Window;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_Context;
	IDXGISwapChain* m_SwapChain;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	D3D_DRIVER_TYPE m_DriverType;

	ID3D11RenderTargetView* g_pRenderTargetView;
	ID3D11Texture2D* g_pDepthStencil;
	ID3D11DepthStencilView* g_pDepthStencilView;

	Color4f ClearColor;

	DirectX11Renderer() {
		m_Device = NULL;
		m_Context = NULL;
		m_SwapChain = NULL;
		m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
		m_DriverType = D3D_DRIVER_TYPE_NULL;
		g_pRenderTargetView = NULL;
		g_pDepthStencil = NULL;
		g_pDepthStencilView = NULL;

		ClearColor.Set(0.0f, 0.125f, 0.3f, 1.0f);
	}

	void Release() {
		if (m_Context) m_Context->ClearState();

		if (g_pDepthStencil) g_pDepthStencil->Release();
		if (g_pDepthStencilView) g_pDepthStencilView->Release();
		if (g_pRenderTargetView) g_pRenderTargetView->Release();
		if (m_SwapChain) m_SwapChain->Release();
		if (m_Context) m_Context->Release();
		if (m_Device) m_Device->Release();
	}

	void EnableBlending(D3D11_BLEND src, D3D11_BLEND dst);
	void DisableBlending();

	void Initialize(int width, int height);
	void Render();
};

typedef DirectX11Renderer CurrentRenderer;

#endif