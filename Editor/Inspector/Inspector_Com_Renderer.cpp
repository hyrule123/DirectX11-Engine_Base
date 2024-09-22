#include "Editor/Inspector/Inspector_Com_Renderer.h"

#include "Editor/Widget/Widget_List.h"
#include "Editor/Inspector/InspectorBase.h"

#include "Editor/Resource/MaterialEditor.h"
#include "Editor/Resource/MeshEditor.h"

#include "Editor/EditorManager.h"

#include <Engine/Manager/ResourceManager.h>


#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_Mesh.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_Sprite.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>

namespace ehw::editor
{
	Inspector_Com_Renderer::Inspector_Com_Renderer()
		: Inspector_Component(eComponentCategory::Renderer)
		, m_mesh{}
		, m_material{}
	{
	}

	Inspector_Com_Renderer::~Inspector_Com_Renderer()
	{

	}

	void Inspector_Com_Renderer::update()
	{
		if (GetTarget())
		{
			const auto& meshRenderer
				= GetTarget()->GetComponent<Com_Renderer_Mesh>();

			if (meshRenderer == nullptr)
				return;

			m_material = meshRenderer->GetCurrentMaterial();
			m_mesh = meshRenderer->GetMesh();
		}
		else
		{
			m_material.reset();
			m_mesh.reset();
		}
	}

	void Inspector_Com_Renderer::update_UI()
	{
		if (m_mesh.expired() || m_material.expired()) {
			return;
		}
		
		std::shared_ptr<Mesh> msh = m_mesh.lock();
		std::shared_ptr<Material> mtrl = m_material.lock();

		std::string meshName(msh->get_resource_name());
		std::string materialName(mtrl->get_resource_name());

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
				= ResourceManager<Mesh>::get_inst().GetResources();

			std::vector<std::string> name;
			for (const auto& mesh : meshes)
			{
				name.push_back(mesh.first);
			}

			listUI->SetItemList(name);
			listUI->SetEvent(this, std::bind(&Inspector_Com_Renderer::set_mesh
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
				= ResourceManager<Material>::get_inst().GetResources();

			std::vector<std::string> Name;
			for (const auto& material : materials)
			{
				Name.push_back(material.first);
			}

			listUI->SetItemList(Name);
			listUI->SetEvent(this, std::bind(&Inspector_Com_Renderer::set_material
				, this, std::placeholders::_1));
		}
	}


	void Inspector_Com_Renderer::set_mesh(const std::string& _key_path)
	{
		std::shared_ptr<Mesh> mesh = ResourceManager<Mesh>::get_inst().find(_key_path);

		std::shared_ptr<InspectorBase> inspector = 
			std::static_pointer_cast<InspectorBase>(EditorManager::FindGuiWindow("InspectorBase"));

		ASSERT_DEBUG(nullptr == inspector, "inspector가 nullptr 입니다.");

		GameObject* targetObj = inspector->GetTargetGameObject();
		
		if (targetObj)
		{
			targetObj->GetComponent<Com_Renderer_Mesh>()->set_mesh(mesh);
		}
	}

	void Inspector_Com_Renderer::set_material(const std::string& _key_path)
	{
		std::shared_ptr<Material> material = ResourceManager<Material>::get_inst().find(_key_path);

		std::shared_ptr<InspectorBase> inspector = std::static_pointer_cast<InspectorBase>(EditorManager::FindGuiWindow("InspectorBase"));

		ASSERT_DEBUG(nullptr == inspector, "inspector가 nullptr 입니다.");

		GameObject* targetObj = inspector->GetTargetGameObject();

		if (targetObj)
		{
			targetObj->GetComponent<Com_Renderer_Mesh>()->set_material(material);
		}
	}
}
