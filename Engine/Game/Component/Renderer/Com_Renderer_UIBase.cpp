#include "Engine/Game/Component/Renderer/Com_Renderer_UIBase.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"

namespace ehw
{
	Com_Renderer_UIBase::Com_Renderer_UIBase()
		: mUIData()
	{
		//기본 UI Mesh, Material을 설정
		std::shared_ptr<Mesh> uiMesh = ResourceManager<Mesh>::GetInst().Find(strKey::defaultRes::mesh::RectMesh);
		SetMesh(uiMesh);

		std::shared_ptr<Material> uiMaterial = ResourceManager<Material>::GetInst().Find(strKey::defaultRes::material::UIMaterial);
		SetMaterial(uiMaterial, 0u);
		SetMaterialMode(0u, eMaterialMode::Dynamic);
	}
	Com_Renderer_UIBase::~Com_Renderer_UIBase()
	{
	}

	void Com_Renderer_UIBase::Render()
	{
		ConstBuffer* cb = RenderManager::GetInst().GetConstBuffer(eCBType::CustomData);
		cb->SetData(&mUIData);
		cb->BindData(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		Com_Renderer_Mesh::Render();
	}
}

