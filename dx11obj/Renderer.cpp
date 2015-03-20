#include "stdafx.h"
#include "Renderer.h"
#include "DirectX11Renderer.h"

// assume for unique renderables and that the beginning of the IRenderable vector isn't = to the end
void IRenderer::RenderList() {
	for (vector<IRenderable*>::iterator i = m_Renderables.begin(); i != m_Renderables.end(); ++i) {
		(*i)->Render();
	}
}

