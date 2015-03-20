#include "stdafx.h"
#include "Entity.h"
#include "Engine.h"

void EntityController::Update() {
	FloatController ctrl(m_Entity->m_Position.m128_f32, m_Speed);
	ctrl.Update();
}

void FloatController::Update() {
	bool* keys = Engine.m_Renderer.m_Window.m_Keys;
	float dispos = Engine.m_Time.m_Diff * m_Speed;

	if (keys[VK_NUMPAD8]) {
		m_Output[0] += dispos;
	} else if (keys[VK_NUMPAD2]) {
		m_Output[0] -= dispos;
	}

	if (keys[VK_NUMPAD4]) {
		m_Output[1] -= dispos;
	} else if (keys[VK_NUMPAD6]) {
		m_Output[1] += dispos;
	}

	if (keys[VK_NUMPAD9]) {
		m_Output[2] += dispos;
	} else if (keys[VK_NUMPAD3]) {
		m_Output[2] -= dispos;
	}
}
