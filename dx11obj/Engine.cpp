#include "stdafx.h"
#include "Engine.h"
#include "DirectX11CubeMapShader.h"
#include "DirectX11TexturedShader.h"
#include "CubeMap.h"

EngineFacade Engine;

void EngineFacade::Run() {
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			m_Updater.Update();
			m_Renderer.Render();
		}
	}
}

EngineFacade::EngineFacade() {
	m_Updater.AddUpdatable(m_Time);
}
