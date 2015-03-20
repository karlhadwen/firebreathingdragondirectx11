#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "stdafx.h"
#include "Interfaces.h"

// quite a simple class that uses IUpdateable to keep on track of what needs to be updated
class TimeManager : public IUpdateable {
public:
	ULONGLONG m_LastTime;
	float m_Time;
	float m_Diff;
	float m_Speed;

	TimeManager() {
		m_Diff = 0.0f;
		m_Speed = 2.0f; // decrease this for slow mo (increasing reminds me of noclip in counter strike source)
		m_LastTime = GetTickCount64();
		m_Time = 0.0f;
	}

	void Update() {
		ULONGLONG curTime = GetTickCount64();
		m_Diff = ((curTime - m_LastTime) / 1000.0f) * m_Speed;
		m_LastTime = curTime;
		m_Time += m_Diff;
	}
};

#endif

