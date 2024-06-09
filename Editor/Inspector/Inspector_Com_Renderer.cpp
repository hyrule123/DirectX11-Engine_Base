#include "Editor/Inspector/Inspector_Com_Renderer.h"

#include "Editor/Widget/Widget_List.h"
#include "Editor/Inspector/InspectorBase.h"

#include "Editor/Resource/MaterialEditor.h"
#include "Editor/Resource/MeshEditor.h"

#include "Editor/EditorManager.h"

#include <Engine/Manager/ResourceManager.h>


#include <Engine/Scene/GameObject.h>
#include <Engine/Scene/Component/Renderer/Com_Renderer_Mesh.h>
#include <Engine/Scene/Component/Renderer/Com_Renderer_Sprite.h>

#include <Engine/Resource/Mesh.h>
#include <Engine/Resource/Material.h>

namespace ehw::editor
{
	Inspector_Com_Renderer::Inspector_Com_Renderer()
		: Inspector_Component(eComponentCategory::Renderer)
		, mMesh{}
		, mMaterial{}
	{
	}

	Inspector_Com_Renderer::~Inspector_Com_Renderer()
	{

	}

	void Inspector_Com_Renderer::Update()
	{
		if (GetTarget())
		{
			const auto& meshRenderer
				= GetTarget()->GetComponent<Com_Renderer_Mesh>();

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

	void Inspector_Com_Renderer::UpdateUI()
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
			std::shared_ptr<Widget_List> listUI = std::static_pointer_cast<Widget_List>(EditorManager::FindGuiWindow("ListWidget"));
			listUI->SetEnable(true);
			

			//모든 메쉬의 리소스를 가져와야한다.
			const auto& meshes 
				= ResourceManager<Mesh>::GetInst().GetResources();

			std::vector<std::string> name;
			for (const auto& mesh : meshes)
			{
				name.push_back(mesh.first);
			}

			listUI->SetItemList(name);
			listUI->SetEvent(this, std::bind(&Inspector_Com_Renderer::SetMesh
				, this, std::placeholders::_1));
		}


		ImGui::Text("Material"); //ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data()
			, materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			
			std::shared_ptr<Widget_List> listUI = std::static_pointer_cast<Widget_List>(EditorManager::FindGuiWindow("ListWidget"));
			listUI->SetEnable(true);
			//모든 메쉬의 리소스를 가져와야한다.
			const auto& materials
				= ResourceManager<Material>::GetInst().GetResources();

			std::vector<std::string> Name;
			for (const auto& material : materials)
			{
				Name.push_back(material.first);
			}

			listUI->SetItemList(Name);
			listUI->SetEvent(this, std::bind(&Inspector_Com_Renderer::SetMaterial
				, this, std::placeholders::_1));
		}
	}


	void Inspector_Com_Renderer::SetMesh(const std::string& _strKey)
	{
		std::shared_ptr<Mesh> mesh = ResourceManager<Mesh>::GetInst().Find(_strKey);

		std::shared_ptr<InspectorBase> inspector = 
			std::static_pointer_cast<InspectorBase>(EditorManager::FindGuiWindow("InspectorBase"));

		ASSERT_DEBUG(nullptr == inspector, "inspector가 nullptr 입니다.");

		GameObject* targetObj = inspector->GetTargetGameObject();
		
		if (targetObj)
		{
			targetObj->GetComponent<Com_Renderer_Mesh>()->SetMesh(mesh);
		}
	}

	void Inspector_Com_Renderer::SetMaterial(const std::string& _strKey)
	{
		std::shared_ptr<Material> material = ResourceManager<Material>::GetInst().Find(_strKey);

		std::shared_ptr<InspectorBase> inspector = std::static_pointer_cast<InspectorBase>(EditorManager::FindGuiWindow("InspectorBase"));

		ASSERT_DEBUG(nullptr == inspector, "inspector가 nullptr 입니다.");

		GameObject* targetObj = inspector->GetTargetGameObject();

		if (targetObj)
		{
			targetObj->GetComponent<Com_Renderer_Mesh>()->SetMaterial(material, 0);
		}
	}
}
