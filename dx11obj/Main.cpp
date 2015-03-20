#include "stdafx.h"
#include "Engine.h"

#include "OBJEntity.h"
#include "Dragon.h"
#include "Terrain.h"
#include "AxisHelper.h"
#include "DirectX11CubeMapShader.h"
#include "CubeMap.h"
#include "Box.h"
#include "House.h"

/**
 * Creates houses and checks dragon fire breath collisions and removes them
 * when necessary.
 */
class HouseManager : public IUpdateable, public IRenderable {
public:
	vector<Vector3f> housePositions;
	vector<House*> houses;
	int houseCount;
	float LastTime;

	HouseManager() {
		// Coords are from 3DSMax
		housePositions.push_back(Vector3f(-91.5f, -75.031f, -128.49f));
		housePositions.push_back(Vector3f(302.04f, -4.075f, -113.024f));
		housePositions.push_back(Vector3f(-15.72f, 387.859f, -102.359f));
		housePositions.push_back(Vector3f(-127.0f, 269.675f, -114.903f));
		housePositions.push_back(Vector3f(-365.519f, 39.907f, -106.538f));
		CreateInitialHouses(5);
		LastTime = 0.0f;
	}

	// Used on collision
	void RemoveHouse(House* h) {
		Vector3f pos(h->m_Position);
		housePositions.push_back(pos);
		removeFromArray(houses, h);
	}

	// Loop through and place houses on the terrain but don't render them as new objects, it wastes
	// memory, just reuse the ones we have
	void CreateInitialHouses(int count) {
		houseCount = count;

		for (int i = 0; i < houseCount; i++) {
			CreateHouse();
		}
	}

	void CreateHouse() {
		Vector3f hpos = housePositions.back();
		housePositions.pop_back();
		CreateHouse(hpos);
	}

	void CreateHouse(Vector3f hpos) {
		House* h = new House(hpos.x, hpos.y, hpos.z);
		h->Update();
		houses.push_back(h);
	}

	virtual void Update() {
		for (int i = 0; i < houses.size(); i++) {
			houses[i]->Update();
		}

		// create a new house every 5 seconds
		if (Engine.m_Time.m_Time - LastTime < 5.0f) {
			return;
		}
		LastTime = Engine.m_Time.m_Time;

		// fire that breath!
		if (Engine.m_Renderer.m_Window.m_Keys[VK_SPACE]) {
			Ray3f r;
			r.m_Point = Engine.m_Camera.Focus;
			r.m_Direction = XMVector3Normalize(Engine.m_Camera.Focus - Engine.m_Camera.Eye);

			Vector3f intersection;

			// check for a ray interaction
			for (int i = 0; i < houses.size(); i++) {
				House* curHouse = houses[i];
				int result = curHouse->CollideShape->IntersectRay(r, intersection);
				
				if (result == 1) {
					RemoveHouse(curHouse);
					return;
				}
			}
		}

		if (houses.size() < houseCount) {
			CreateHouse();
		}

	}

	virtual void Render() {
		for (int i = 0; i < houses.size(); i++) {
			houses[i]->Render();
		}
	}

};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	Engine.m_Renderer.Initialize(800, 600);
	Engine.m_Sound.Initialize();

	// distance from dragon can be set here by changing -60 parameter
	Engine.m_Camera.Set(-60.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// create shaders
	DirectX11TexturedVertexShader::Instance = new DirectX11TexturedVertexShader();
	DirectX11TexturedPixelShader::Instance = new DirectX11TexturedPixelShader();

	DirectX11CubeMapVertexShader::Instance = new DirectX11CubeMapVertexShader();
	DirectX11CubeMapPixelShader::Instance = new DirectX11CubeMapPixelShader();

	DirectX11ParticleVertexShader::Instance = new DirectX11ParticleVertexShader();
	DirectX11ParticleGeometryShader::Instance = new DirectX11ParticleGeometryShader();
	DirectX11ParticlePixelShader::Instance = new DirectX11ParticlePixelShader();

	// create camera controller, input and dragon
	CameraTPSController g_MainCameraController(Engine.m_Camera);
	CameraTPSControllerInput caminput(g_MainCameraController, Engine.m_Renderer.m_Window.m_Keys);
	Dragon dragon(g_MainCameraController);

	// create terrain & access the axis
	Terrain terrain;
	HouseManager houses; // collection of houses (so we don't readd them always)
	//AxisHelper axis;
	CubeMap cubeMap;

	dragon.m_CheckTerrain = &terrain;

	// add to updatable to keep track of cam input & controller
	// i may remove the controller, i'm not sure just yet, we'll see in part 2

	Engine.m_Updater.AddUpdatable(caminput);
	Engine.m_Updater.AddUpdatable(g_MainCameraController);
	Engine.m_Updater.AddUpdatable(cubeMap);
	Engine.m_Updater.AddUpdatable(houses);

	// add terrain & dragon as well
	Engine.m_Updater.AddUpdatable(terrain);
	Engine.m_Updater.AddUpdatable(dragon);

	// render
	Engine.m_Renderer.AddRenderable(cubeMap);
	Engine.m_Renderer.AddRenderable(terrain);
	Engine.m_Renderer.AddRenderable(houses);
	Engine.m_Renderer.AddRenderable(dragon);

	// run the game
	Engine.Run();

	return 0;
}