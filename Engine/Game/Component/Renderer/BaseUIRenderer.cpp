#include "BaseUIRenderer.h"

#include <Engine/Manager/RenderManager.h>
#include <Engine/GPU/ConstBuffer.h>

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>

namespace core
{
	BaseUIRenderer::BaseUIRenderer()
		: StaticMeshRenderer(BaseUIRenderer::s_static_type_name)
		, m_UI_data()
	{
		//기본 UI Mesh, Material을 설정
		s_ptr<Mesh> uiMesh = ResourceManager<Mesh>::get_inst().find(name::defaultRes::mesh::RectMesh);
		set_mesh(uiMesh);

		s_ptr<Material> uiMaterial = ResourceManager<Material>::get_inst().find(name::defaultRes::material::UIMaterial);
		set_material(uiMaterial);
		set_material_mode(eMaterialMode::Dynamic);
	}
	BaseUIRenderer::~BaseUIRenderer()
	{
	}

	//void BaseUIRenderer::render()
	//{
	//	ASSERT(false, "미구현");
	//	ConstBuffer* cb = nullptr;
	//		//RenderManager::get_inst().GetConstBuffer(eCBType::CustomData);
	//	cb->set_data(&m_UI_data);
	//	cb->bind_buffer_to_GPU_register(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

	//	StaticMeshRenderer::render();
	//}
}

