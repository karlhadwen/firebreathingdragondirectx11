#ifndef ENGINE_H
#define ENGINE_H

#include "DirectX11Renderer.h"
#include "FileSystem.h"
#include "Updater.h"
#include "TimeManager.h"
#include "SoundEngine.h"
#include "Camera.h"

/**
 * main engine facade
 */
class EngineFacade {
public:
	CurrentRenderer m_Renderer;
	Updater	m_Updater;
	TimeManager m_Time;
	SoundEngine m_Sound;
	Camera m_Camera;

	EngineFacade();

	void Run();
};

extern EngineFacade Engine;

#endif