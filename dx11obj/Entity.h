#ifndef ENTITY_H
#define ENTITY_H

#include "Renderer.h"
#include "Updater.h"
#include "Shader.h"
#include "Math.h"

class Entity: public IRenderable, public IUpdateable {
public:
	Entity* m_Parent;
	vector< Entity* > m_Childs;

	Vector4f m_Position; // position vector
	Vector4f m_Rotation; // rotation quaternion
	Vector4f m_Scale; // scaler vector

	Matrix4x4f m_Matrix;

	ICollidable* CollideShape;

	Entity() {
		m_Position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_Rotation = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
		m_Scale = XMVectorSet( 1.0f, 1.0f, 1.0f, 1.0f );
		m_Parent = NULL;
		Update();
		CollideShape = NULL;
	}

	virtual ~Entity() {}

	virtual void OnCollide(Entity* other) {}

	void Translate( float x, float y, float z ) {
		m_Position = XMVectorSet(x,y,z, 0.0f);
	}

	void Rotate(float pitch, float yaw, float roll) {
		m_Rotation = XMQuaternionRotationRollPitchYaw(pitch,yaw,roll);
	}

	void Scale(float x, float y, float z) {
		m_Scale = XMVectorSet(x, y, z, 0.0f);
	}

	void UpdateChilds() {
		for (int i = 0; i < m_Childs.size(); i++) {
			m_Childs[i]->Update();
		}
	}

	void RenderChilds() {
		for (int i = 0; i < m_Childs.size(); i++) {
			m_Childs[i]->Render();
		}
	}

	void AddChild(Entity& obj) {
		obj.m_Parent = this;
		m_Childs.push_back(&obj);
	}

	virtual void Update() {
		Vector4f quat = m_Rotation; //
		Vector4f zerovec = XMVectorZero();

		m_Matrix = XMMatrixAffineTransformation(m_Scale, zerovec, quat, m_Position);

		if (m_Parent != NULL) {
			m_Matrix = m_Matrix * m_Parent->m_Matrix;
		}

		UpdateChilds();
	}

	// render child entities
	virtual void Render() {
		RenderChilds();
	}
};


class FloatController : public IUpdateable {
public:
	float* m_Output;
	float m_Speed;

	FloatController(float* output, float speed = 0.5f) {
		m_Output = output;
		m_Speed = speed;
	}

	void Update();
};

class EntityController : public IUpdateable {
public:
	Entity* m_Entity;
	float m_Speed;

	EntityController(Entity& entity, float speed = 0.5f) {
		m_Entity = &entity;
		m_Speed = speed;
	}

	void Update();
};

#endif // !ENTITY_H
