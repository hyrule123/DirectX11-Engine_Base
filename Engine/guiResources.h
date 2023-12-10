#pragma once
#include "EditorChild.h"
#include "guiTreeWidget.h"

#include "iResource.h"
#include "ResourceManager.h"

namespace editor
{
	class guiResources : public EditorWindow
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
			const std::unordered_map<std::string, std::shared_ptr<ehw::iResource>, ehw::tHashFunc_StringView, std::equal_to<>>& resources
				= ehw::ResourceManager::GetResources<T>();

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
