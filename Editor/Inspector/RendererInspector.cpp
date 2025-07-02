#include <Editor/Inspector/RendererInspector.h>

#include <Editor/Widget/Widget_List.h>
#include <Editor/Inspector/InspectorBase.h>

#include <Editor/Resource/MaterialEditor.h>
#include <Editor/Resource/MeshEditor.h>

#include <Editor/EditorManager.h>

#include <Engine/Manager/ResourceManager.h>


#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_Mesh.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_Sprite.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>

namespace core::editor
{
	RendererInspcetor::RendererInspcetor()
		: ComponentInspector(eComponentOrder::Renderer)
		, m_mesh{}
		, m_material{}
	{
	}

	RendererInspcetor::~RendererInspcetor()
	{

	}

	void RendererInspcetor::update()
	{
		if (get_target())
		{
			const auto& meshRenderer
				= get_target()->get_component<Com_Renderer_Mesh>();

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

	void RendererInspcetor::update_UI()
	{
		if (m_mesh.expired() || m_material.expired()) {
			return;
		}
		
		s_ptr<Mesh> msh = m_mesh.lock();
		s_ptr<Material> mtrl = m_material.lock();

		

		std::string meshName = msh->get_resource_name();
		std::string materialName = mtrl->get_resource_name();

		ImGui::Text("Mesh"); 
		ImGui::InputText("##MeshName", (char*)meshName.data()
			, meshName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("##MeshBtn", ImVec2(15.0f, 15.0f)))
		{
			s_ptr<Widget_List> listUI = std::static_pointer_cast<Widget_List>(EditorManager::get_inst().find_editor_window("ListWidget"));
			listUI->set_enable(true);
			

			//모든 메쉬의 리소스를 가져와야한다.
			const auto& meshes 
				= ResourceManager<Mesh>::get_inst().GetResources();

			std::vector<std::string> name;
			for (const auto& mesh : meshes)
			{
				name.push_back(mesh.first);
			}

			listUI->SetItemList(name);
			listUI->SetEvent(this, std::bind(&RendererInspcetor::set_mesh
				, this, std::placeholders::_1));
		}


		ImGui::Text("Material"); //ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data()
			, materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::SameLine();
		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			
			s_ptr<Widget_List> listUI = std::static_pointer_cast<Widget_List>(EditorManager::get_inst().find_editor_window("ListWidget"));
			listUI->set_enable(true);
			//모든 메쉬의 리소스를 가져와야한다.
			const auto& materials
				= ResourceManager<Material>::get_inst().GetResources();

			std::vector<std::string> Name;
			for (const auto& material : materials)
			{
				Name.push_back(material.first);
			}

			listUI->SetItemList(Name);
			listUI->SetEvent(this, std::bind(&RendererInspcetor::set_material
				, this, std::placeholders::_1));
		}
	}


	void RendererInspcetor::set_mesh(const std::string& _key_path)
	{
		s_ptr<Mesh> mesh = ResourceManager<Mesh>::get_inst().find(_key_path);

		s_ptr<InspectorBase> inspector = 
			std::static_pointer_cast<InspectorBase>(EditorManager::get_inst().find_editor_window("InspectorBase"));

		ASSERT_DEBUG(nullptr == inspector, "inspector가 nullptr 입니다.");

		s_ptr<GameObject> targetObj = inspector->GetTargetGameObject();
		
		if (targetObj)
		{
			targetObj->get_component<Com_Renderer_Mesh>()->set_mesh(mesh);
		}
	}

	void RendererInspcetor::set_material(const std::string& _key_path)
	{
		s_ptr<Material> material = ResourceManager<Material>::get_inst().find(_key_path);

		s_ptr<InspectorBase> inspector = std::static_pointer_cast<InspectorBase>(EditorManager::get_inst().find_editor_window("InspectorBase"));

		ASSERT_DEBUG(nullptr == inspector, "inspector가 nullptr 입니다.");

		s_ptr<GameObject> targetObj = inspector->GetTargetGameObject();

		if (targetObj)
		{
			targetObj->get_component<Com_Renderer_Mesh>()->set_material(material);
		}
	}
}
