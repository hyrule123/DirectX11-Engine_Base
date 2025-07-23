#include <Editor/Base/EditorUIWindow.h>

#include <Editor/EditorManager.h>
#include <Editor/UI/EditorMainMenu.h>

namespace core::editor
{
	EditorUIWindow::EditorUIWindow()
		: EditorUIBase()
		, m_widget_size{}
		, m_ImGui_window_flag{ ImGuiWindowFlags_::ImGuiWindowFlags_None }
		, m_b_border{ false }
	{
	}
	void EditorUIWindow::init()
	{
		Super::init();

		set_save_enable(true);
	}

	EditorUIWindow::~EditorUIWindow()
	{
	}

	void EditorUIWindow::awake()
	{
		Super::awake();

		s_ptr<EditorMainMenu> main_menu = EditorManager::get_inst().get_editor_main_menu();
		if (main_menu)
		{
			s_ptr<EditorUIWindow> ths = std::static_pointer_cast<EditorUIWindow>(shared_from_this());
			main_menu->add_window(ths);
		}
	}

	bool EditorUIWindow::begin_UI()
	{
		bool result = ImGui::Begin(get_unique_name().c_str(), is_enabled_ptr(), m_ImGui_window_flag);
		if (false == result)
		{
			ImGui::End();
			return false;
		}
		return true;
	}

	void EditorUIWindow::end_UI()
	{
		ImGui::End();
	}
}
