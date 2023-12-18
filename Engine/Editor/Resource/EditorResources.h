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
		void AddResources(const char* name)
		{
			const std::vector<std::shared_ptr<ehw::iResource>>& resources
				= ehw::ResourceManager<T>::GetResourcesVector();

			EditorWidget_Tree::tNode* stemNode
				= mTreeWidget->AddNode(rootNode, name, ehw::tDataPtr{}, true);

			for (const auto& resource : resources)
			{
				ehw::tDataPtr data{};
				data.SetDataPtr(resource.second.get());
				mTreeWidget->AddNode(stemNode, resource, data);
			}
		}

		void toInspector(ehw::tDataPtr _data);


		std::shared_ptr<EditorWidget_Tree> m_textureTree;
		std::shared_ptr<EditorWidget_Tree> m_materialTree;
		std::shared_ptr<EditorWidget_Tree> m_meshTree;
	};
}
