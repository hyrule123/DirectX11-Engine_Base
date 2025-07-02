#pragma once
#include <Editor/Base/EditorBase.h>

namespace core::editor
{
	class EditorWindow;
	class EditorMainMenu : public EditorBase
	{
		CLASS_INFO(EditorMainMenu, EditorBase);
	public:
		EditorMainMenu();
		virtual ~EditorMainMenu();

		virtual bool begin_UI() final;
		virtual void update_UI() override;
		virtual void end_UI() final;

		void register_window(const s_ptr<EditorWindow>& _window) {
			if (_window) { m_editor_windows.push_back(_window); }
		}
	private:
		std::vector<w_ptr<EditorWindow>> m_editor_windows;
	};
}
