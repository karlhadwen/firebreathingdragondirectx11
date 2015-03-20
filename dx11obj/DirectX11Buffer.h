#ifndef DIRECTX11BUFFER_H
#define DIRECTX11BUFFER_H

#include "Engine.h"
#include "Buffer.h"

class DirectX11Buffer: public Buffer {
public:
	ID3D11Buffer *m_Buffer;

	DirectX11Buffer() {
		m_Buffer = NULL;
	}

	void Release() {
		m_Buffer->Release();
	}
};

template<class T>
class DirectX11ConstantBuffer: public DirectX11Buffer, public IUpdateable {
public:
	T m_State;

	DirectX11ConstantBuffer() {
		Create();
	}
	
	void Update() {
		Engine.m_Renderer.m_Context->UpdateSubresource(m_Buffer, 0, NULL, &m_State, 0, 0);
	}

	DirectX11ConstantBuffer& operator = (T& value) {
		m_State = value;
		Update();
		return *this;
	}

	void Create() {
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(T);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HRESULT hr = Engine.m_Renderer.m_Device->CreateBuffer(&bd, NULL, &m_Buffer);
		if (FAILED(hr))
			throw exception("create buffer failed");
	}
};

template<class T, int Slot>
class DirectX11PSConstantBuffer : public DirectX11ConstantBuffer<T>, public IRenderable {
public:
	DirectX11PSConstantBuffer& operator = (T& value) {
		m_State = value;
		Update();
		return *this;
	}

	inline void Render() {
		Engine.m_Renderer.m_Context->PSSetConstantBuffers(Slot, 1, &m_Buffer);
	}
};

template<class T, int Slot>
class DirectX11VSConstantBuffer : public DirectX11ConstantBuffer<T>, public IRenderable {
public:
	DirectX11VSConstantBuffer& operator = (T& value) {
		m_State = value;
		Update();
		return *this;
	}

	inline void Render() {
		Engine.m_Renderer.m_Context->VSSetConstantBuffers(Slot, 1, &m_Buffer);
	}
};

#endif