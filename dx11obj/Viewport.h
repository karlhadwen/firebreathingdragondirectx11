#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Interfaces.h"

// just a standard viewport that inherits IUpdateable so that we can update the screen
// and cords easily
class Viewport : public IUpdateable {
public:
	int m_Width;
	int m_Height;

	float m_FOV;
	float m_Near;
	float m_Far;
	XMMATRIX m_Projection;

	Viewport() {
		m_FOV = XM_PIDIV4;
		m_Near = 0.01f;
		m_Far = 1000.0f;
	}

	// update the viewport 
	void Update() {
		m_Projection = XMMatrixPerspectiveFovLH( m_FOV , (float)m_Width / (float)m_Height, m_Near, m_Far);
	}
};

#endif