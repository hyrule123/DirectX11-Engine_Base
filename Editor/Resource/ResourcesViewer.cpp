#include "Editor/Resource/ResourcesViewer.h"

#include "Editor/EditorManager.h"

#include "Editor/Inspector/InspectorBase.h"
#include "Editor/Resource/ResourceEditor.h"

#include <Engine/GameEngine.h>

#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Material.h>
#include <Engine/Resource/Mesh.h>
#include <Engine/Resource/Shader/GraphicsShader.h>






namespace ehw::editor
{
	using namespace math;

	EditorResources::EditorResources()
		: EditorWindow(strKey::ResourceViewer)
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
		m_textureTree = add_child<EditorWidget_Tree>();
		m_materialTree = add_child<EditorWidget_Tree>();
		m_meshTree = add_child<EditorWidget_Tree>();


		m_textureTree->SetEvent(this
			, std::bind(&EditorResources::ToInspectorTexture, this, std::placeholders::_1));
		//m_textureTree->SetDummyRoot(true);


		m_materialTree->SetEvent(this
			, std::bind(&EditorResources::ToInspectorMaterial, this, std::placeholders::_1));
		//m_materialTree->SetDummyRoot(true);


		m_meshTree->SetEvent(this
			, std::bind(&EditorResources::ToInspectorMesh, this, std::placeholders::_1));
		//m_meshTree->SetDummyRoot(true);

		ResetContent();
	}

	void EditorResources::ResetContent()
	{
		//m_textureTree->Clear();
		//m_textureTree->SEt

		//m_materialTree->Clear();
		//m_meshTree->Clear();


		///*EditorWidget_Tree::tNode* pRootNode = mTreeWidget->AddNode(nullptr, "GameResources", tDataPtr{}, true);*/



		//AddResources<Mesh>(pRootNode, "Mesh");
		//AddResources<Texture>(pRootNode, "Texture");
		//AddResources<Material>(pRootNode, "Materials");
		//AddResources<GraphicsShader>(pRootNode, "Shaders");
	}

	void EditorResources::ToInspectorTexture(tDataPtr _data)
	{
		const auto& resource = static_cast<Resource*>(_data.pData)->shared_from_this_T<Resource>();

		

		std::shared_ptr<InspectorBase> inspector = std::static_pointer_cast<InspectorBase>(EditorManager::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetResource(resource);
	}

	void EditorResources::ToInspectorMaterial(tDataPtr _data)
	{
	}

	void EditorResources::ToInspectorMesh(tDataPtr _data)
	{
	}


}
