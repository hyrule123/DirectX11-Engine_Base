
#include "GridScript.h"

#include <Engine/GPU/ConstBuffer.h>
#include <Engine/Game/GameObject.h>
#include <Engine/GameEngine.h>

#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/SceneManager.h>


#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Camera/Camera.h>

namespace core
{
	

	GridScript::GridScript()
		: Script(GridScript::s_static_type_name)
		, m_camera()
	{

	}

	GridScript::~GridScript()
	{

	}

	void GridScript::awake()
	{
		//eSceneType type = SceneManager::get_active_scene()->GetSceneType();
		m_camera = RenderManager::get_inst().sceneRenderAgent().get_camera(0);
	}

	void GridScript::update()
	{
		if (m_camera.expired())
		{
			return;
		}

		s_ptr<Camera> cam = m_camera.lock();
		const auto& gameObj = cam->get_owner();
		const auto& TR = gameObj->get_component<Transform>();
		
		float3 cameraPosition = TR->get_local_position();
		float4 position = float4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);

		float scale = cam->GetScale();

		RECT winRect;
		GetClientRect(GameEngine::get_inst().get_HWND(), &winRect);
		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		float2 resolution(width, height);

		// Constant buffer
		//ConstBuffer* CB = RenderManager::get_inst().GetConstBuffer(eCBType::Grid);
		//tCB_Grid data;
		//data.cameraPosition = position;
		//data.cameraScale = float2(scale, scale);
		//data.resolution = resolution;
		//
		//CB->set_data(&data);
		//CB->bind_buffer_to_GPU_register(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);
 	}



}
