#include <Editor/Base/EditorUIWindow.h>

#include <Editor/EditorManager.h>

namespace core::editor
{
	EditorUIWindow::EditorUIWindow()
		: EditorUIBase()
	{
	}
	void EditorUIWindow::init()
	{
		Super::init();

		set_save_enable(true);

		m_widget_size = {};
		m_ImGui_window_flag = ImGuiWindowFlags_::ImGuiWindowFlags_None;
		m_b_border = false;

		
	}

	EditorUIWindow::~EditorUIWindow()
	{
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
