#include "Engine/Game/Component/Script/GridScript.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/Game/GameObject.h"
#include "Engine/GameEngine.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/SceneManager.h"


#include "Engine/Game/Component/Com_Transform.h"
#include "Engine/Game/Component/Camera/Com_Camera.h"

namespace ehw
{
	

	GridScript::GridScript()
		: Script(GridScript::concrete_name)
		, mCamera(nullptr)
	{

	}

	GridScript::~GridScript()
	{

	}

	void GridScript::Awake()
	{
		//eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		mCamera = RenderManager::GetInst().sceneRenderAgent().GetCamera(0);
	}

	void GridScript::Update()
	{
		if (mCamera == nullptr)
		{
			return;
		}
		else if (mCamera->IsDestroyed())
		{
			mCamera = nullptr;
			return;
		}

		const auto& gameObj = mCamera->gameObject();
		const auto& TR = gameObj->GetComponent<Com_Transform>();
		
		float3 cameraPosition = TR->GetLocalPosition();
		float4 position = float4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);

		float scale = mCamera->GetScale();

		RECT winRect;
		GetClientRect(GameEngine::GetInst().GetHwnd(), &winRect);
		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		float2 resolution(width, height);

		// Constant buffer
		ConstBuffer* CB = RenderManager::GetInst().GetConstBuffer(eCBType::Grid);
		tCB_Grid data;
		data.cameraPosition = position;
		data.cameraScale = float2(scale, scale);
		data.resolution = resolution;
		
		CB->SetData(&data);
		CB->BindData(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);
 	}



}
