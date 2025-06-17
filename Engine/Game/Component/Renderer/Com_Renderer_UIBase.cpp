#include "Engine/Game/Component/Renderer/Com_Renderer_UIBase.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"

namespace core
{
	Com_Renderer_UIBase::Com_Renderer_UIBase()
		: Com_Renderer_Mesh(Com_Renderer_UIBase::s_concrete_class_name)
		, mUIData()
	{
		//기본 UI Mesh, Material을 설정
		s_ptr<Mesh> uiMesh = ResourceManager<Mesh>::get_inst().find(name::defaultRes::mesh::RectMesh);
		set_mesh(uiMesh);

		s_ptr<Material> uiMaterial = ResourceManager<Material>::get_inst().find(name::defaultRes::material::UIMaterial);
		set_material(uiMaterial);
		SetMaterialMode(eMaterialMode::Dynamic);
	}
	Com_Renderer_UIBase::~Com_Renderer_UIBase()
	{
	}

	//void Com_Renderer_UIBase::render()
	//{
	//	ASSERT(false, "미구현");
	//	ConstBuffer* cb = nullptr;
	//		//RenderManager::get_inst().GetConstBuffer(eCBType::CustomData);
	//	cb->set_data(&mUIData);
	//	cb->bind_buffer_to_GPU_register(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

	//	Com_Renderer_Mesh::render();
	//}
}

