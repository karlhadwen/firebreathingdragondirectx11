#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"
#include "Renderer.h"
#include "Math.h"

class Camera : public IUpdateable {
public:
	XMVECTOR Eye;
	XMVECTOR Focus;
	XMVECTOR Up;

	XMMATRIX View;

	Camera() {}

	Camera(float eyeX, float eyeY, float eyeZ,
		float focusX, float focusY, float focusZ,
		float upX, float upY, float upZ) {
		Set(eyeX, eyeY, eyeZ, focusX, focusY, focusZ, upX, upY, upZ);
	}

	void Set(float eyeX, float eyeY, float eyeZ,
		float focusX, float focusY, float focusZ,
		float upX, float upY, float upZ) {
		Eye = XMVectorSet(eyeX, eyeY, eyeZ, 0.0f);
		Focus = XMVectorSet(focusX, focusY, focusZ, 0.0f);
		Up = XMVectorSet(upX, upY, upZ, 0.0f);

		Update();
	}

	void Update() {
		View = XMMatrixLookAtLH(Eye, // the eye, or viewer's position.
			Focus,	// the look at point.
			Up);	// which way is up.
	}
};

class CameraTPSController : public IUpdateable {
public:
	Camera& m_Cam;

	// keys that we'll be mapping eventually
	bool m_Forward;
	bool m_Backward;
	bool m_StrafeLeft;
	bool m_StrafeRight;

	int m_MouseDiffX;
	int m_MouseDiffY;

	float m_MoveSpeed;
	float m_RotationSpeed; // degrees per pixel of mouse movement

	CameraTPSController(Camera& cam) : m_Cam(cam) {
		m_Forward = false;
		m_Backward = false;
		m_StrafeLeft = false;
		m_StrafeRight = false;
		m_MouseDiffX = 0;
		m_MouseDiffY = 0;
		m_MoveSpeed = 50.0f;
		m_RotationSpeed = 1.0f / 60.0f;
	}

	void Update();
};

class CameraTPSControllerInput : public IUpdateable {
public:
	CameraTPSController* m_Controller;
	bool* m_KeyStates;

	CameraTPSControllerInput(CameraTPSController& ctrl, bool* input) {
		m_Controller = &ctrl;
		m_KeyStates = input;
	}

	// standard gaming keys to use
	void Update() {
		m_Controller->m_Forward = m_KeyStates['W'];
		m_Controller->m_Backward = m_KeyStates['S'];
		m_Controller->m_StrafeLeft = m_KeyStates['A'];
		m_Controller->m_StrafeRight = m_KeyStates['D'];
	}
};

#endif // !CAMERA_H
