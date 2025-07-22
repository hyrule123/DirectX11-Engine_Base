#pragma once
#include <Editor/Base/EditorUIWindow.h>
#include <Editor/Widget/HierarchyTreeWidget.h>


namespace core::editor
{
	class EditorResources : public EditorUIWindow
	{
		CLASS_INFO(EditorResources, EditorUIWindow);
	public:
		EditorResources();
		virtual ~EditorResources();

		virtual void init() override;

		void ResetContent();

	private:
		void to_texture_inspector(w_ptr<void> _ptr);
		void to_material_inspector(w_ptr<void> _ptr);
		void to_mesh_inspector(w_ptr<void> _ptr);

		s_ptr<HierarchyTreeWidget> m_textureTree;
		s_ptr<HierarchyTreeWidget> m_materialTree;
		s_ptr<HierarchyTreeWidget> m_meshTree;
	};
}
