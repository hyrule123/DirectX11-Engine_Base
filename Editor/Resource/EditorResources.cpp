#include "PCH_Editor.h"
#include "EditorResources.h"

#include "Application.h"

#include "Manager/EditorManager.h"
#include "Manager/ResourceManager.h"

#include "Resource/Texture.h"
#include "Resource/Material.h"
#include "Resource/Mesh.h"
#include "Resource/Shader/GraphicsShader.h"

#include "Inspector/EditorInspector.h"
#include "Resource/EditorResource.h"




namespace editor
{
	using namespace ehw::math;

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

	void EditorResources::Init()
	{
		m_textureTree = AddChild<EditorWidget_Tree>();
		m_materialTree = AddChild<EditorWidget_Tree>();
		m_meshTree = AddChild<EditorWidget_Tree>();


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


		///*EditorWidget_Tree::tNode* pRootNode = mTreeWidget->AddNode(nullptr, "GameResources", ehw::tDataPtr{}, true);*/



		//AddResources<ehw::Mesh>(pRootNode, "Mesh");
		//AddResources<ehw::Texture>(pRootNode, "Texture");
		//AddResources<ehw::Material>(pRootNode, "Materials");
		//AddResources<ehw::GraphicsShader>(pRootNode, "Shaders");
	}

	void EditorResources::ToInspectorTexture(ehw::tDataPtr _data)
	{
		const auto& resource = static_cast<ehw::iResource*>(_data.pData)->shared_from_this_T<ehw::iResource>();

		

		std::shared_ptr<EditorInspector> inspector = std::static_pointer_cast<EditorInspector>(EditorManager::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetResource(resource);
	}

	void EditorResources::ToInspectorMaterial(ehw::tDataPtr _data)
	{
	}

	void EditorResources::ToInspectorMesh(ehw::tDataPtr _data)
	{
	}


}
