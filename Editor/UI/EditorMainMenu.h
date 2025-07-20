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
		virtual ~EditorMainMenu();

		virtual bool begin_UI() final;
		virtual void update_UI() override;
		virtual void end_UI() final;

		void register_window(const s_ptr<EditorUIWindow>& _window) {
			if (_window) { m_editor_windows.push_back(_window); }
		}
	private:
		std::vector<w_ptr<EditorUIWindow>> m_editor_windows;
	};
}
