#include "ResourcesViewer.h"

#include <Editor/EditorManager.h>

#include <Editor/Inspector/InspectorBase.h>
#include <Editor/Resource/ResourceEditor.h>

#include <Engine/GameEngine.h>

#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Material/Material.h>
#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Shader/GraphicsShader.h>


namespace core::editor
{
	using namespace math;

	EditorResources::EditorResources()
		: EditorUIWindow()
		, m_textureTree()
		, m_materialTree()
		, m_meshTree()
	{
	}

	EditorResources::~EditorResources()
	{
	}

	void EditorResources::init()
	{
		Super::init();

		set_unique_name(name::ResourceViewer);

		m_textureTree = new_entity<HierarchyTreeWidget>();
		m_materialTree = new_entity<HierarchyTreeWidget>();
		m_meshTree = new_entity<HierarchyTreeWidget>();

		s_ptr<EditorResources> ths = std::static_pointer_cast<EditorResources>(shared_from_this());

		m_textureTree->set_selected_callaback_func(
			std::bind(&EditorResources::to_texture_inspector, this, std::placeholders::_1)
		);

		//m_materialTree->set_selected_callaback_func(this
		//	, std::bind(&EditorResources::ToInspectorMaterial, this, std::placeholders::_1));

		//m_meshTree->set_selected_callaback_func(this
		//	, std::bind(&EditorResources::ToInspectorMesh, this, std::placeholders::_1));

		ResetContent();
	}

	void EditorResources::ResetContent()
	{
		//m_textureTree->Clear();
		//m_textureTree->SEt

		//m_materialTree->Clear();
		//m_meshTree->Clear();


		///*TreeNode* pRootNode = m_tree_widget->add_child(nullptr, "GameResources", DataPtr{}, true);*/



		//AddResources<Mesh>(pRootNode, "Mesh");
		//AddResources<Texture>(pRootNode, "Texture");
		//AddResources<Material>(pRootNode, "Materials");
		//AddResources<GraphicsShader>(pRootNode, "Shaders");
	}

	void EditorResources::to_texture_inspector(w_ptr<void> _ptr)
	{
		s_ptr<Resource> res = std::reinterpret_pointer_cast<Resource>(_ptr.lock());

		s_ptr<InspectorBase> inspector = 
			std::static_pointer_cast<InspectorBase>(EditorManager::get_inst().find_editor_window(name::Inspector));
		inspector->set_target_resource(res);
	}

	void EditorResources::to_material_inspector(w_ptr<void> _ptr)
	{
	}

	void EditorResources::to_mesh_inspector(w_ptr<void> _ptr)
	{
	}


}
