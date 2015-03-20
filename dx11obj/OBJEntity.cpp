#include "stdafx.h"
#include "OBJEntity.h"

int OBJEntity::IntersectRay(Ray3f& ray, Vector3f& intersection) {
	bool translatedCheck = false;
	float sum = m_Position.m128_f32[0] + m_Position.m128_f32[1] + m_Position.m128_f32[2];
	
	if ( sum > 0.00001f || sum < -0.00001f) {
		translatedCheck = true;
	}

	if (translatedCheck) {
		Vector3f tv1;
		Vector3f tv2;
		Vector3f tv3;
		Vector3f curPos(m_Position.m128_f32[0], m_Position.m128_f32[1], m_Position.m128_f32[2]);

		for (int i = 0; i < m_Renderer->m_VB.m_Parts.size(); i++) {
			auto curPart = m_Renderer->m_VB.m_Parts[i];
			int faces = curPart->m_Buffer.m_VertexCount / 3;

			for (int k = 0; k < faces; k++) {
				int curFace = k * 3;

				auto v1 = curPart->m_Buffer.m_Vertices[curFace];
				auto v2 = curPart->m_Buffer.m_Vertices[curFace + 1];
				auto v3 = curPart->m_Buffer.m_Vertices[curFace + 2];

				tv1 = v1.Position + curPos;
				tv2 = v2.Position + curPos;
				tv3 = v3.Position + curPos;

				int intResult = Triangle::IntersectRay(tv1, tv2, tv3, ray, intersection);
				
				if (intResult == 1) {
					return 1;
				}
			}
		}
	} else {}

	return 0;
}

int OBJEntity::IntersectRay2(Ray3f& ray, Vector3f& intersection, float length) {
	for (int i = 0; i < m_Renderer->m_VB.m_Parts.size(); i++) {
		auto curPart = m_Renderer->m_VB.m_Parts[i];
		int faces = curPart->m_Buffer.m_VertexCount / 3;

		for (int k = 0; k < faces; k++) {
			int curFace = k * 3;

			auto v1 = curPart->m_Buffer.m_Vertices[curFace];
			auto v2 = curPart->m_Buffer.m_Vertices[curFace + 1];
			auto v3 = curPart->m_Buffer.m_Vertices[curFace + 2];

			int intResult = Triangle::IntersectRay(v1.Position, v2.Position, v3.Position, ray, intersection);
			
			if (intResult == 1) {
				Vector3f diff = intersection - ray.m_Point;
				
				if (diff.Length() <= length) {
					return 1;
				}
			}
		}
	}

	return 0;
}

#include "House.h"

OBJRenderer* House::globalHouse = nullptr;