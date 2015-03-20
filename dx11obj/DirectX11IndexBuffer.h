#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "Engine.h"
#include "DirectX11Buffer.h"

class DirectX11IndexBuffer : public DirectX11Buffer, public IRenderable {
public:
	int m_IndexCount;
	int m_FaceCount;
	std::vector<UINT16> m_Indexes;

	void Reset() {
		m_Indexes.clear();
		m_IndexCount = 0;
		m_Buffer->Release();
		m_Buffer = NULL;
	}

	inline void AddIndex(UINT16 i) {
		m_Indexes.push_back(i);
		m_IndexCount = m_Indexes.size();
	}

	void Update() {
		Engine.m_Renderer.m_Context->UpdateSubresource(m_Buffer, 0, NULL, m_Indexes.data(), 0, 0);
	}

	void Render() {
		// Set index buffer
		UINT stride = sizeof(UINT16);
		UINT offset = 0;
		Engine.m_Renderer.m_Context->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R16_UINT, offset);

		Engine.m_Renderer.m_Context->DrawIndexed(m_IndexCount, 0, 0);
	}

	void Create() {
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(UINT16)* m_IndexCount;	// from sortOfMesh
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_Indexes.data();	// from sortOfMesh	

		HRESULT hr = Engine.m_Renderer.m_Device->CreateBuffer(&bd, &InitData, &m_Buffer);
		if (FAILED(hr))
			throw exception("Create index buffer failed");
	}

	void Clear() {
		m_IndexCount = 0;
		m_Indexes.clear();
	}
};

class DirectX11IndexBufferSlot: public IRenderable {
public:
	DirectX11IndexBuffer* m_Buffer;

	DirectX11IndexBufferSlot& operator = (DirectX11IndexBuffer& buffer) {
		m_Buffer = &buffer;
		return *this;
	}

	inline void Render() {
		UINT offset = 0;
		Engine.m_Renderer.m_Context->IASetIndexBuffer(m_Buffer->m_Buffer, DXGI_FORMAT_R16_UINT, offset);
	}
};

#endif