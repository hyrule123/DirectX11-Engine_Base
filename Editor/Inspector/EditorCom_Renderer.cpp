#include "PCH_Editor.h"
#include "Inspector/EditorCom_Renderer.h"

#include "Manager/ResourceManager.h"
#include "Manager/EditorManager.h"

#include "Game/GameObject.h"
#include "Game/Component/Renderer/Com_Renderer_Mesh.h"
#include "Game/Component/Renderer/Com_Renderer_Sprite.h"

#include "Resource/iResource.h"

#include "Widget/EditorWidget_List.h"
#include "Inspector/EditorInspector.h"


namespace editor
{
	EditorCom_Renderer::EditorCom_Renderer()
		: EditorComponent(ehw::eComponentCategory::Renderer)
		, mMesh{}
		, mMaterial{}
	{
	}

	EditorCom_Renderer::~EditorCom_Renderer()
	{

	}

	void EditorCom_Renderer::Update()
	{
		if (false == GetTarget().expired())
		{
			const auto& meshRenderer
				= GetTarget().lock()->GetComponent<ehw::Com_Renderer_Mesh>();

			if (meshRenderer == nullptr)
				return;

			mMaterial = meshRenderer->GetCurrentMaterial(0);
			mMesh = meshRenderer->GetMesh().get();
		}
		else
		{
			mMaterial = nullptr;
			mMesh = nullptr;
		}
	}

	void EditorCom_Renderer::UpdateUI()
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
			std::shared_ptr<EditorWidget_List> listUI = std::static_pointer_cast<EditorWidget_List>(EditorManager::FindGuiWindow("ListWidget"));
			listUI->SetEnable(true);
			

			//모든 메쉬의 리소스를 가져와야한다.
			const auto& meshes 
				= ehw::ResourceManager<ehw::Mesh>::GetResources();

			std::vector<std::string> name;
			for (const auto& mesh : meshes)
			{
				name.push_back(mesh.first);
			}

			listUI->SetItemList(name);
			listUI->SetEvent(this, std::bind(&EditorCom_Renderer::SetMesh
				, this, std::placeholders::_1));
		}


		ImGui::Text("Material"); //ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data()
			, materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			
			std::shared_ptr<EditorWidget_List> listUI = std::static_pointer_cast<EditorWidget_List>(EditorManager::FindGuiWindow("ListWidget"));
			listUI->SetEnable(true);
			//모든 메쉬의 리소스를 가져와야한다.
			const auto& materials
				= ehw::ResourceManager<ehw::Material>::GetResources();

			std::vector<std::string> Name;
			for (const auto& material : materials)
			{
				Name.push_back(material.first);
			}

			listUI->SetItemList(Name);
			listUI->SetEvent(this, std::bind(&EditorCom_Renderer::SetMaterial
				, this, std::placeholders::_1));
		}
	}


	void EditorCom_Renderer::SetMesh(const std::string& _strKey)
	{
		std::shared_ptr<ehw::Mesh> mesh = ehw::ResourceManager<ehw::Mesh>::Find(_strKey);

		std::shared_ptr<EditorInspector> inspector = 
			std::static_pointer_cast<EditorInspector>(EditorManager::FindGuiWindow("EditorInspector"));

		ASSERT_DEBUG(nullptr == inspector, "inspector가 nullptr 입니다.");

		const auto& targetObj = inspector->GetTargetGameObject();
		
		if (false == targetObj.expired())
		{
			targetObj.lock()->GetComponent<ehw::Com_Renderer_Mesh>()->SetMesh(mesh);
		}
	}

	void EditorCom_Renderer::SetMaterial(const std::string& _strKey)
	{
		std::shared_ptr<ehw::Material> material = ehw::ResourceManager<ehw::Material>::Find(_strKey);

		std::shared_ptr<EditorInspector> inspector = std::static_pointer_cast<EditorInspector>(EditorManager::FindGuiWindow("EditorInspector"));

		ASSERT_DEBUG(nullptr == inspector, "inspector가 nullptr 입니다.");

		const auto& targetObj = inspector->GetTargetGameObject();

		if (false == targetObj.expired())
		{
			targetObj.lock()->GetComponent<ehw::Com_Renderer_Mesh>()->SetMaterial(material, 0);
		}
	}
}
