#include "../../PCH_Engine.h"
#include "Com_Renderer_UIBase.h"



#include "../../Manager/RenderManager.h"
#include "../../GPU/ConstBuffer.h"

#include "../../Manager/ResourceManager.h"
#include "../../Resource/Mesh.h"

namespace ehw
{
	Com_Renderer_UIBase::Com_Renderer_UIBase()
		: mUIData()
	{
		//기본 UI Mesh, Material을 설정
		std::shared_ptr<Mesh> uiMesh = ResourceManager::Find<Mesh>(strKey::Default::mesh::RectMesh);
		SetMesh(uiMesh);

		std::shared_ptr<Material> uiMaterial = ResourceManager::Find<Material>(strKey::Default::material::UIMaterial);
		SetMaterial(uiMaterial, 0u);
		SetMaterialMode(0u, eMaterialMode::Dynamic);
	}
	Com_Renderer_UIBase::~Com_Renderer_UIBase()
	{
	}


	void Com_Renderer_UIBase::Render()
	{
		ConstBuffer* cb = RenderManager::GetConstBuffer(eCBType::CustomData);
		cb->SetData(&mUIData);
		cb->BindData(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		Com_Renderer_Mesh::Render();
	}
}

