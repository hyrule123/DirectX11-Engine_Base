#include <Editor/Base/EditorWindow.h>

namespace core::editor
{
	EditorWindow::EditorWindow(const std::string_view _static_type_name)
		: EditorBase(_static_type_name)
	{
	}
	void EditorWindow::init()
	{
		Super::init();

		set_save_enable(true);

		m_widget_size = {};
		m_ImGui_window_flag = ImGuiWindowFlags_::ImGuiWindowFlags_None;
		m_b_border = false;


	}

	EditorWindow::~EditorWindow()
	{
	}

	bool EditorWindow::begin_UI()
	{
		bool result = ImGui::Begin(get_unique_name().c_str(), is_enabled_ptr(), m_ImGui_window_flag);
		if (false == result)
		{
			ImGui::End();
			return false;
		}
		return true;
	}

	void EditorWindow::end_UI()
	{
		ImGui::End();
	}
}
