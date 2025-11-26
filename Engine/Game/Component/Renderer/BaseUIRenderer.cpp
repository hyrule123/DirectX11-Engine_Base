#include "BaseUIRenderer.h"

#include <Engine/Manager/RenderManager.h>
#include <Engine/GPU/ConstBuffer.h>

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>

namespace core
{
	BaseUIRenderer::BaseUIRenderer()
		: StaticMeshRenderer()
		, m_UI_data()
	{
		//기본 UI Mesh, Material을 설정
		s_ptr<Mesh> uiMesh = ResourceManager::get_inst().find<Mesh>(default_res::mesh::rect_mesh);
		set_mesh(uiMesh);

		s_ptr<Material> uiMaterial = ResourceManager::get_inst().find<Material>(default_res::material::ui_material);
		set_material(uiMaterial);
		set_material_mode(eMaterialMode::Dynamic);
	}
	BaseUIRenderer::~BaseUIRenderer()
	{
	}

	//void BaseUIRenderer::render()
	//{
	//	ASSERT_MESSAGE(false, "미구현");
	//	ConstBuffer* cb = nullptr;
	//		//RenderManager::get_inst().GetConstBuffer(eCBType::CustomData);
	//	cb->set_data(&m_UI_data);
	//	cb->bind(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

	//	StaticMeshRenderer::render();
	//}
}

