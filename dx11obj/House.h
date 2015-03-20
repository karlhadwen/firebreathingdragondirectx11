#ifndef HOUSE_H
#define HOUSE_H

#include "OBJEntity.h"

class House : public OBJEntity {
public:
	static OBJRenderer* globalHouse;

	House(float x, float y, float z) : OBJEntity(nullptr) {
		if (globalHouse == nullptr) {
			globalHouse = new OBJRenderer();
			globalHouse->Load(L"Media\\House\\house.obj");
		}

		m_Renderer = globalHouse;
		
		m_Position = XMVectorSet(x, y, z, 0.0f);
		Sphere* s = new Sphere();
		s->m_Position.Set(x, y, z);
		s->m_Radius = 50.0f;
		CollideShape = s;
	}

	void Update() {
		//m_Scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		//m_Position = Engine.m_Camera.Focus;
		//m_Position = Engine.m_Camera.Focus;
		this->Entity::Update();
	}
};

#endif // !HOUSE_H
