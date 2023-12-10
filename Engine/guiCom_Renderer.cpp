
#include "PCH_Engine.h"

#include "Com_Renderer_Mesh.h"
#include "ResourceMgr.h"
#include "Com_Renderer_Sprite.h"
#include "iResource.h"
#include "GameObject.h"

#include "guiCom_Renderer.h"
#include "guiMgr.h"
#include "guiList.h"
#include "guiInspector.h"


namespace gui
{
	guiCom_Renderer::guiCom_Renderer()
		: guiComponent(ehw::eComponentCategory::Renderer)
		, mMesh{}
		, mMaterial{}
	{
	}

	guiCom_Renderer::~guiCom_Renderer()
	{

	}

	void guiCom_Renderer::Update()
	{
		if (GetTarget())
		{
			ehw::Com_Renderer_Mesh* meshRenderer
				= GetTarget()->GetComponent<ehw::Com_Renderer_Mesh>();

			if (meshRenderer == nullptr)
				return;

			//ehw::SpriteRenderer* spriteRenderer
			//	= GetTarget()->GetComponent<ehw::SpriteRenderer>();

			//if (spriteRenderer == nullptr)
			//	return;


			mMaterial = meshRenderer->GetCurrentMaterial(0);
			mMesh = meshRenderer->GetMesh().get();
		}
	}

	void guiCom_Renderer::UpdateUI()
	{
		if (mMesh == nullptr || mMaterial == nullptr)
			return;

		std::string meshName
			= mMesh->GetStrKey();
		std::string materialName
			= mMaterial->GetStrKey();

		ImGui::Text("Mesh"); 
		ImGui::InputText("##MeshName", (char*)meshName.data()
			, meshName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("##MeshBtn", ImVec2(15.0f, 15.0f)))
		{
			guiList* listUI = static_cast<guiList*>(guiMgr::FindGuiWindow("ListWidget"));
			listUI->SetEnable(true);
			

			//모든 메쉬의 리소스를 가져와야한다.
			const auto& meshes 
				= ehw::ResourceMgr::GetResources<ehw::Mesh>();

			std::vector<std::string> name;
			for (const auto& mesh : meshes)
			{
				name.push_back(mesh.first);
			}

			listUI->SetItemList(name);
			listUI->SetEvent(this, std::bind(&guiCom_Renderer::SetMesh
				, this, std::placeholders::_1));
		}


		ImGui::Text("Material"); //ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data()
			, materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			guiList* listUI = static_cast<guiList*>(guiMgr::FindGuiWindow("ListWidget"));
			listUI->SetEnable(true);
			//모든 메쉬의 리소스를 가져와야한다.
			const auto& materials
				= ehw::ResourceMgr::GetResources<ehw::Material>();

			std::vector<std::string> Name;
			for (const auto& material : materials)
			{
				Name.push_back(material.first);
			}

			listUI->SetItemList(Name);
			listUI->SetEvent(this, std::bind(&guiCom_Renderer::SetMaterial
				, this, std::placeholders::_1));
		}
	}


	void guiCom_Renderer::SetMesh(const std::string& _strKey)
	{
		std::shared_ptr<ehw::Mesh> mesh = ehw::ResourceMgr::Find<ehw::Mesh>(_strKey);

		guiInspector* inspector = static_cast<guiInspector*>(guiMgr::FindGuiWindow("guiInspector"));
		inspector->GetTargetGameObject()->GetComponent<ehw::Com_Renderer_Mesh>()->SetMesh(mesh);
	}

	void guiCom_Renderer::SetMaterial(const std::string& _strKey)
	{
		std::shared_ptr<ehw::Material> material = ehw::ResourceMgr::Find<ehw::Material>(_strKey);

		guiInspector* inspector = static_cast<guiInspector*>(guiMgr::FindGuiWindow("guiInspector"));
		inspector->GetTargetGameObject()->GetComponent<ehw::Com_Renderer_Mesh>()->SetMaterial(material, 0);
	}
}
