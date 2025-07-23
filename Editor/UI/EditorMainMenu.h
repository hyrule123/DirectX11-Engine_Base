#pragma once
#include <Editor/Base/EditorUIBase.h>

namespace core::editor
{
	class EditorUIWindow;
	class EditorMainMenu : public EditorUIBase
	{
		CLASS_INFO(EditorMainMenu, EditorUIBase);
	public:
		EditorMainMenu();
		virtual void init() override;

		virtual ~EditorMainMenu();

		virtual bool begin_UI() final;
		virtual void update_UI() override;
		virtual void end_UI() final;

		void add_window(const s_ptr<EditorUIWindow>& _window);
	private:
		std::vector<w_ptr<EditorUIWindow>> m_editor_windows;
	};
}
