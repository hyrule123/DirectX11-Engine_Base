#pragma once
#include "Editor/Base/EditorChild.h"
#include "Editor/Widget/Widget_Tree.h"


namespace ehw::editor
{
	class EditorResources : public EditorWindow
	{
	public:
		EditorResources();
		virtual ~EditorResources();

		virtual void init() override;

		void ResetContent();

	private:
		template <typename T>
		void AddResources(const char* name)
		{
			//const std::vector<std::shared_ptr<Resource>>& resources
			//	= ResourceManager<T>::get_inst().GetResourcesVector();

			//EditorWidget_Tree::tNode* stemNode
			//	= mTreeWidget->AddNode(rootNode, name, tDataPtr{}, true);

			//for (const auto& resource : resources)
			//{
			//	tDataPtr data{};
			//	data.SetDataPtr(resource.second.get());
			//	mTreeWidget->AddNode(stemNode, resource, data);
			//}
		}



		void ToInspectorTexture(tDataPtr _data);
		void ToInspectorMaterial(tDataPtr _data);
		void ToInspectorMesh(tDataPtr _data);


		std::shared_ptr<EditorWidget_Tree> m_textureTree;
		std::shared_ptr<EditorWidget_Tree> m_materialTree;
		std::shared_ptr<EditorWidget_Tree> m_meshTree;
	};
}
