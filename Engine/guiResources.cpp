
#include "PCH_Engine.h"

#include "guiResources.h"

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "GraphicsShader.h"
#include "ResourceMgr.h"

#include "guiInspector.h"
#include "guiResource.h"

#include "guiMgr.h"

#include "Application.h"

namespace gui
{
	using namespace ehw::math;

	guiResources::guiResources()
		: guiWindow(strKey::ResourceViewer)
		, mTreeWidget(nullptr)
	{
		mTreeWidget = AddChild<TreeWidget>();

		mTreeWidget->SetEvent(this
			, std::bind(&guiResources::toInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);
	}

	guiResources::~guiResources()
	{
	}

	void guiResources::Init()
	{
		ResetContent();
	}

	void guiResources::ResetContent()
	{
		//mTreeWidget->Close();
		mTreeWidget->Clear();

		TreeWidget::tNode* pRootNode = mTreeWidget->AddNode(nullptr, "GameResources", ehw::define::tDataPtr{}, true);

		//enum class eResourceType
		//{
		//	Mesh,
		//	Texture,
		//	Material,
		//	Sound,
		//	Prefab,
		//	MeshData,
		//	GraphicsShader,
		//	ComputeShader,
		//	End,
		//};
		AddResources<ehw::Mesh>(pRootNode, "Mesh");
		AddResources<ehw::Texture>(pRootNode, "Texture");
		AddResources<ehw::Material>(pRootNode, "Materials");
		AddResources<ehw::GraphicsShader>(pRootNode, "Shaders");
	}

	void guiResources::toInspector(ehw::define::tDataPtr _data)
	{
		ehw::IResource* resource = static_cast<ehw::IResource*>(_data.pData);

		guiInspector* inspector = static_cast<guiInspector*>(guiMgr::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetResource(resource);
		//inspector->InitializeTargetResource();
	}

}