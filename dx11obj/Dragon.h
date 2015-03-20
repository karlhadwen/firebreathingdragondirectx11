#ifndef DRAGON_H
#define DRAGON_H

#include "OBJEntity.h"
#include "Camera.h"
#include "ParticleEmitter.h"

// seperate .obj for wings so that they move relative to the dragon
class DragonLeftWing : public OBJEntity {
public:
	DragonLeftWing() : OBJEntity(L"Media\\Dragon\\wings.obj") { }
};

// seperate .obj for wings so that they move relative to the dragon
class DragonRightWing : public OBJEntity {
public:
	DragonRightWing() : OBJEntity(L"Media\\Dragon\\right_wing.obj") { }
};

class Dragon : public OBJEntity {
public:
	DragonLeftWing leftWing;
	DragonRightWing rightWing;
	float m_WingSpeed;
	Wave m_FlapSound;
	CameraTPSController& m_CamController;
	ParticleEmitter m_DragonBreath;
	bool m_Landed;
	OBJEntity* m_CheckTerrain;

	// these are used for animation
	float angs[3];
	int totalDiffX;
	int totalDiffY;

	Dragon(CameraTPSController& controller) : OBJEntity(L"Media\\Dragon\\dragon_wo_wings.obj"), m_CamController(controller) {
		// map the left wing on the dragon (cords: 3ds max)
		leftWing.Translate(3.795984f, 1.593502f, -1.546502f);
		AddChild(leftWing);

		// map the right wing on the dragon (cords: 3ds max)
		rightWing.Translate(5.046482f, -0.765500f, -0.937f);
		AddChild(rightWing);

		// spinning stuff (making sure the wings are angelled correctly)
		angs[0] = -85;
		angs[1] = 180;
		angs[2] = 90;

		totalDiffX = 0;
		totalDiffY = 0;

		m_WingSpeed = 1.1f;
		m_FlapSound.Load("Media\\Dragon\\wing_flap.wav");

		m_DragonBreath.m_ParticleStartingSize = 0.01f;
		m_DragonBreath.m_ParticleStartingSpeed = 100.0f;
		m_DragonBreath.m_AlphaChangePerSecond = -0.1;
		m_DragonBreath.m_EmissionTime = 0.01;
		m_DragonBreath.m_SizeChangePerSecond = 10.0;
		m_DragonBreath.m_Texture.Create(L"Media\\Particles\\DragonBreath.jpg");
		m_DragonBreath.SetEmittingCone(15.0f);

		m_Landed = false;
		m_CheckTerrain = nullptr;
	}

	// up down
	float wingAnimator1() {
		return fmod(tanf(fmod(Engine.m_Time.m_Time, XM_PIDIV4) + XM_PIDIV4), XM_PIDIV2);
	}

	float wingAnimator2() {
		float ang = fmod(Engine.m_Time.m_Time * m_WingSpeed, XM_PIDIV2);
		if (ang > XM_PIDIV4) {
			ang = XM_PIDIV4 - (ang - XM_PIDIV4);
		}
		else if (ang > (XM_PIDIV4 - 0.2f))  {
			// when begining to flap a wing, play sound
			if (!m_FlapSound.IsPlaying()) {
				m_FlapSound.Play();
			}
		}
		return ang;
	}

	// something i've always liked seeing game developers do so i thought
	// i'd go ahead and add debug text myself
	void DebugText(float x, float y = 0.0f, float z = 0.0f) {
		wchar_t tmp[256];
		swprintf(tmp, L"%f %f %f", x, y, z);
		Engine.m_Renderer.m_Window.Text(tmp);
	}

	// update the camera and the mouse and store angels for future use
	void Update() {
		totalDiffX += m_CamController.m_MouseDiffX * 2;
		totalDiffY += m_CamController.m_MouseDiffY * 2;

		if (totalDiffX > 1200) {
			totalDiffX = 1200;
		} else if (totalDiffX < -1200) {
			totalDiffX = -1200;
		}

		if (totalDiffY > 1200) {
			totalDiffY = 1200;
		} else if (totalDiffY < -1200) {
			totalDiffY = -1200;
		}

		this->m_Position = Engine.m_Camera.Eye;

		float ang = wingAnimator2();

		// keeping stuff simple
		leftWing.Rotate(ang, 0.0f, 0.0f);
		rightWing.Rotate(-ang, 0.0f, 0.0f);

		XMVECTOR direction = Engine.m_Camera.Focus - Engine.m_Camera.Eye;
		XMVECTOR determ;

		FloatController fc(angs, 30.0f);
		fc.Update();

		float ang0 = angs[0] + (cos(Engine.m_Time.m_Time) * 2.0f) + ((float)(-totalDiffY)* (m_CamController.m_RotationSpeed));
		float ang1 = angs[1] + (sin(Engine.m_Time.m_Time) * 1.0f) + ((float)(totalDiffX)* (m_CamController.m_RotationSpeed));
		float ang2 = angs[2];

		if (totalDiffX < 0) {
			totalDiffX += 1;
		} else if (totalDiffX > 1) {
			totalDiffX -= 1;
		}

		if (totalDiffY < 0) {
			totalDiffY += 1;
		} else if (totalDiffY > 1) {
			totalDiffY -= 1;
		}

		//DebugText(ang0, ang1, ang2);

		// multiplying with inverse of camera view will make dragon stay in front of screen
		m_Matrix = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(ang0),
			XMConvertToRadians(ang1),
			XMConvertToRadians(ang2)) *
			XMMatrixInverse(&determ, Engine.m_Camera.View) *
			XMMatrixTranslationFromVector(direction);

		// update wings; making sure they flap at the same time and are relative
		UpdateChilds();

		XMVECTOR newP = XMVectorSet(12.548f, 0.0f, 5.547f, 0.0f);
		newP = XMVector3Transform(newP, m_Matrix);

		m_DragonBreath.m_Position = newP;
		//XMVectorAdd(m_DragonBreath.m_Position, newP);

		m_DragonBreath.Update();

		m_DragonBreath.m_Emitting = Engine.m_Renderer.m_Window.m_Keys[VK_SPACE];
	}

	// render objs
	void Render() {

		this->OBJEntity::Render();
		m_DragonBreath.Render();
	}
};

#endif