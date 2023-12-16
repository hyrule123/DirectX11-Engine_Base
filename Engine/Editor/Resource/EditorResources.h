#pragma once
#include "../../Editor/Base/EditorChild.h"
#include "../../Editor/Widget/EditorWidget_Tree.h"

#include "../../Manager/ResourceManager.h"

#include "../../Resource/iResource.h"


namespace editor
{
	class EditorResources : public EditorWindow
	{
	public:
		EditorResources();
		virtual ~EditorResources();

		virtual void Init() override;

		void ResetContent();

	private:
		template <typename T>
		void AddResources(EditorWidget_Tree::tNode* rootNode, const char* name)
		{
			const std::unordered_map<std::string, std::shared_ptr<ehw::iResource>, ehw::tHashFunc_StringView, std::equal_to<>>& resources
				= ehw::ResourceManager::GetResources<T>();

			EditorWidget_Tree::tNode* stemNode
				= mTreeWidget->AddNode(rootNode, name, ehw::tDataPtr{}, true);

			for (const auto& resource : resources)
			{
				ehw::tDataPtr data{};
				data.SetDataPtr(resource.second.get());
				mTreeWidget->AddNode(stemNode, resource.first, data);
			}
		}

		void toInspector(ehw::tDataPtr _data);

		EditorWidget_Tree* mTreeWidget;
	};
}