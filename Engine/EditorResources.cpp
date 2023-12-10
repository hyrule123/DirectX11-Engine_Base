
#include "PCH_Engine.h"

#include "EditorResources.h"

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "GraphicsShader.h"
#include "ResourceManager.h"

#include "guiInspector.h"
#include "EditorResource.h"

#include "EditorManager.h"

#include "Application.h"

namespace editor
{
	using namespace ehw::math;

	EditorResources::EditorResources()
		: EditorWindow(strKey::ResourceViewer)
		, mTreeWidget(nullptr)
	{
		mTreeWidget = AddChild<EditorWidget_Tree>();

		mTreeWidget->SetEvent(this
			, std::bind(&EditorResources::toInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);
	}

	EditorResources::~EditorResources()
	{
	}

	void EditorResources::Init()
	{
		ResetContent();
	}

	void EditorResources::ResetContent()
	{
		//mTreeWidget->Close();
		mTreeWidget->Clear();

		EditorWidget_Tree::tNode* pRootNode = mTreeWidget->AddNode(nullptr, "GameResources", ehw::tDataPtr{}, true);

		//enum class eResourceType
		//{
		//	Mesh,
		//	Texture,
		//	Material,
		//	Sound,
		//	Prefab,
		//	MeshData,
		//	GraphicsShader,
		//	iComputeShader,
		//	End,
		//};
		AddResources<ehw::Mesh>(pRootNode, "Mesh");
		AddResources<ehw::Texture>(pRootNode, "Texture");
		AddResources<ehw::Material>(pRootNode, "Materials");
		AddResources<ehw::GraphicsShader>(pRootNode, "Shaders");
	}

	void EditorResources::toInspector(ehw::tDataPtr _data)
	{
		ehw::iResource* resource = static_cast<ehw::iResource*>(_data.pData);

		guiInspector* inspector = static_cast<guiInspector*>(EditorManager::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetResource(resource);
		//inspector->InitializeTargetResource();
	}

}
