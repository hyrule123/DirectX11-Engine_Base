#include "PCH_Contents.h"
#include "Scene_Title.h"

#include <Engine/ResourceMgr.h>
#include <Engine/Com_Transform.h>
#include <Engine/Com_Renderer_Mesh.h>
#include <Engine/RenderMgr.h>
#include <Engine/Texture.h>
#include <Engine/Com_Camera.h>
#include <Engine/Com_Renderer_Sprite.h>
#include <Engine/GridScript.h>
#include <Engine/Object.h>
#include <Engine/InputMgr.h>
#include <Engine/ICollider2D.h>
#include <Engine/Player.h>
#include <Engine/Monster.h>
#include <Engine/CollisionMgr.h>
#include <Engine/Com_Animator2D.h>
#include <Engine/Com_Light3D.h>
#include <Engine/Com_Animator3D.h>
#include <Engine/Com_Renderer_ParticleSystem.h>
#include <Engine/Prefab.h>
#include <Engine/Com_Renderer_UIBase.h>
#include <Contents/Script_UIBase.h>
#include <Contents/Script_UIGauge.h>
#include <Contents/Script_Player.h>

#include "strKey_Script.h"


#include <Engine/EventMgr.h>

namespace mh
{
	Scene_Title::Scene_Title()
	{
	}
	Scene_Title::~Scene_Title()
	{
	}
	void Scene_Title::Init()
	{
		IScene::Init();

		{
			// Main Com_Camera Game Object
			GameObject* cameraObj = EventMgr::SpawnGameObject(eLayerType::Com_Camera);
			cameraObj->SetName("MainCamera");

			Com_Transform* tr = cameraObj->AddComponent<Com_Transform>();
			tr->SetRelativePos(float3(0.0f, 0.0f, -20.0f));

			Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
			cameraComp->SetProjectionType(define::eProjectionType::Perspective);

			cameraObj->AddComponent(strKey::Script::Script_CameraMove);
			cameraObj->AddComponent(strKey::Script::Script_UIBase);


			RenderMgr::SetMainCamera(cameraComp);
		}

		{
			GameObject* dirLight = EventMgr::SpawnGameObject(eLayerType::Player);
			dirLight->AddComponent<Com_Transform>();

			Com_Light3D* light3d = dirLight->AddComponent<Com_Light3D>();
			light3d->SetLightType(eLightType::Directional);
			light3d->SetDiffuse(float4(0.3f, 0.3f, 0.3f, 1.f));
			light3d->SetAmbient(float4(0.3f, 0.3f, 0.3f, 1.f));
		}

		{
			GameObject* dirLight = EventMgr::SpawnGameObject(eLayerType::Player);
			dirLight->AddComponent<Com_Transform>();
			dirLight->SetName("Point1000");

			Com_Light3D* light3d = dirLight->AddComponent<Com_Light3D>();
			light3d->SetLightType(eLightType::Point);
			light3d->SetRadius(1000.f);
			light3d->SetDiffuse(float4(0.3f, 0.3f, 0.3f, 1.f));
			light3d->SetAmbient(float4(0.3f, 0.3f, 0.3f, 1.f));
		}

		{
			GameObject* dirLight = EventMgr::SpawnGameObject(eLayerType::Player);
			dirLight->AddComponent<Com_Transform>();
			dirLight->SetName("Point500");

			Com_Light3D* light3d = dirLight->AddComponent<Com_Light3D>();
			light3d->SetLightType(eLightType::Point);
			light3d->SetRadius(500.f);

			light3d->SetDiffuse(float4(0.7f, 0.7f, 0.7f, 1.f));
			
			light3d->SetAmbient(float4(0.3f, 0.3f, 0.3f, 1.f));
		}


		{
			GameObject* player = EventMgr::SpawnGameObject(eLayerType::Player);
			player->AddComponent<Script_Player>();

			
			//GameObject* modeling = meshdata->Instantiate(eLayerType::Player);
		}
	}
	void Scene_Title::Update()
	{
		if (InputMgr::GetKeyDown(eKeyCode::N))
		{
			//SceneMgr::LoadScene(eSceneType::Play);
		}

		IScene::Update();
	}
	void Scene_Title::FixedUpdate()
	{
		IScene::FixedUpdate();
	}
	void Scene_Title::Render()
	{
		IScene::Render();
	}
	void Scene_Title::OnEnter()
	{

	}
	void Scene_Title::OnExit()
	{
	}
}
