#ifndef RENDERER_H
#define RENDERER_H

#include "Interfaces.h"
#include "Viewport.h"

class IRenderer: public IResource {
protected:
	void RenderList();

public:
	Viewport m_Viewport;
	vector<IRenderable*> m_Renderables;

	void AddRenderable(IRenderable& item) {
		m_Renderables.push_back(&item);
	}

	void RemoveRenderable(IRenderable& item) {
		removeFromArray(m_Renderables, &item);
	}

	 virtual void Initialize(int width, int height) = 0;

	 virtual void Render() = 0;

	 virtual void Release() = 0;
};

#endif