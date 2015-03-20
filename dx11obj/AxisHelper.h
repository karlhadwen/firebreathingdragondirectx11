#ifndef AXISHELPER_H
#define AXISHELPER_H

#include "OBJEntity.h"

/**
* test entity to see movement of objects
*/
class AxisHelper : public OBJEntity {
public:
	AxisHelper() : OBJEntity(L"Media\\Axis\\axis.obj") {
	}

	void Update() {
		m_Position = Engine.m_Camera.Focus;
		this->Entity::Update();
	}
};

#endif // !AXISHELPER_H
