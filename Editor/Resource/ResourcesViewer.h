#pragma once
#include "Editor/Base/EditorWindow.h"
#include "Editor/Widget/Widget_Tree.h"


namespace core::editor
{
	class EditorResources : public EditorWindow
	{
	public:
		EditorResources();
		virtual ~EditorResources();

		virtual void init() override;

		void ResetContent();

	private:
		void ToInspectorTexture(w_ptr<void> _ptr);
		void ToInspectorMaterial(w_ptr<void> _ptr);
		void ToInspectorMesh(w_ptr<void> _ptr);

		s_ptr<EditorWidget_Tree> m_textureTree;
		s_ptr<EditorWidget_Tree> m_materialTree;
		s_ptr<EditorWidget_Tree> m_meshTree;
	};
}
