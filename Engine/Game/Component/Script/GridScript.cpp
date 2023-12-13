
#include "GridScript.h"



#include "../../../GPU/ConstBuffer.h"
#include "../../../Game/GameObject.h"
#include "../../../Application.h"

#include "../../../Manager/RenderManager.h"
#include "../../../Manager/SceneManager.h"


#include "../Transform/Com_Transform.h"


namespace ehw
{
	

	GridScript::GridScript()
		: iScript()
		, mCamera(nullptr)
	{

	}

	GridScript::~GridScript()
	{

	}

	void GridScript::Awake()
	{
		//eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		mCamera = RenderManager::GetCamera(0);
	}

	void GridScript::Update()
	{
		if (mCamera == nullptr)
		{
			return;
		}

		GameObject* gameObj = mCamera->GetOwner();
		Com_Transform* TR = gameObj->GetComponent<Com_Transform>();
		
		float3 cameraPosition = TR->GetRelativePos();
		float4 position = float4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);

		float scale = mCamera->GetScale();

		RECT winRect;
		GetClientRect(Application::GetHwnd(), &winRect);
		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		float2 resolution(width, height);

		// Constant buffer
		ConstBuffer* CB = RenderManager::GetConstBuffer(eCBType::Grid);
		tCB_Grid data;
		data.cameraPosition = position;
		data.cameraScale = float2(scale, scale);
		data.resolution = resolution;
		
		CB->SetData(&data);
		CB->BindData(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);
 	}



}
