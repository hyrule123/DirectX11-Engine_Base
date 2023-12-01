#pragma once
#include "guiChild.h"
#include "guiTreeWidget.h"

#include "IResource.h"
#include "ResourceMgr.h"

namespace gui
{
	class guiResources : public guiWindow
	{
	public:
		guiResources();
		virtual ~guiResources();

		virtual void Init() override;

		void ResetContent();

	private:
		template <typename T>
		void AddResources(TreeWidget::tNode* rootNode, const char* name)
		{
			const std::unordered_map<std::string, std::shared_ptr<ehw::IResource>, ehw::tHashFunc_StringView, std::equal_to<>>& resources
				= ehw::ResourceMgr::GetResources<T>();

			TreeWidget::tNode* stemNode
				= mTreeWidget->AddNode(rootNode, name, ehw::tDataPtr{}, true);

			for (const auto& resource : resources)
			{
				ehw::tDataPtr data{};
				data.SetDataPtr(resource.second.get());
				mTreeWidget->AddNode(stemNode, resource.first, data);
			}
		}

		void toInspector(ehw::tDataPtr _data);

		TreeWidget* mTreeWidget;
	};
}
