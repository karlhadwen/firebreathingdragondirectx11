#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Engine.h"
#include "DirectX11Buffer.h"

template <class T>
class DirectX11VertexBuffer : public DirectX11Buffer, public IRenderable {
public:
	int m_VertexCount;
	int m_FaceCount;
	std::vector<T> m_Vertices;

	void Reset() {
		m_Vertices.clear();
		m_VertexCount = 0;
		m_Buffer->Release();
		m_Buffer = NULL;
	}

	void AddVertex(T& v) {
		m_Vertices.push_back(v);
		m_VertexCount = m_Vertices.size();
	}

	void Set() {
		// Set vertex buffer
		UINT stride = sizeof(T);
		UINT offset = 0;
		Engine.m_Renderer.m_Context->IASetVertexBuffers(0, 1, &m_Buffer, &stride, &offset);

		// Set primitive topology
		Engine.m_Renderer.m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void Update() {
		Engine.m_Renderer.m_Context->UpdateSubresource(m_Buffer, 0, NULL, m_Vertices.data(), 0, 0);
	}

	void Render() {
		Set();
		Engine.m_Renderer.m_Context->Draw(m_VertexCount, 0);
	}

	void Create() {
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(T)* m_VertexCount;	// from sortOfMesh
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_Vertices.data();	// from sortOfMesh	

		HRESULT hr = Engine.m_Renderer.m_Device->CreateBuffer(&bd, &InitData, &m_Buffer);
		if (FAILED(hr))
			throw exception("Create vertex buffer failed");
	}

	void Clear() {
		m_VertexCount = 0;
		m_Vertices.clear();
	}
};

template <class T,int Slot>
class DirectX11VertexBufferSlot: public IRenderable {
public:
	DirectX11VertexBuffer<T>* m_Buffer;

	DirectX11VertexBufferSlot<T, Slot>& operator = (DirectX11VertexBuffer<T>& buffer) {
		m_Buffer = &buffer;
		return *this;
	}

	inline void Render() {
		UINT stride = sizeof(T);
		UINT offset = 0;
		Engine.m_Renderer.m_Context->IASetVertexBuffers(Slot, 1, &m_Buffer->m_Buffer, &stride, &offset);
	}
};

#endif