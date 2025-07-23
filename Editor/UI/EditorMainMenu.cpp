#include <Editor/UI/EditorMainMenu.h>

#include <Editor/EditorManager.h>
#include <Editor/Base/EditorUIWindow.h>

#include <Engine/GameEngine.h>

namespace core::editor
{
	EditorMainMenu::EditorMainMenu()
		: EditorUIBase()
	{
        //SetSize(ImVec2(100.0f, 100.0f));
	}

	void EditorMainMenu::init()
	{
		Super::init();

		set_enable(true);
	}

	EditorMainMenu::~EditorMainMenu()
	{
	}

	bool EditorMainMenu::begin_UI()
	{
		return ImGui::BeginMainMenuBar();
	}

	void EditorMainMenu::update_UI()
	{
		if (ImGui::BeginMenu("UI Menu"))
		{
			for (size_t i = 0; i < m_editor_windows.size(); ++i)
			{
				s_ptr<EditorUIWindow> ui = m_editor_windows[i].lock();
				if (ui)
				{
					bool result =
						ImGui::MenuItem(ui->get_unique_name().c_str(), nullptr, ui->is_enabled_ptr());
				}
			}

			ImGui::Separator();

			bool p_open = is_enabled();
			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
			{
				::core::GameEngine::get_inst().shutdown();
				//GameEngine::get_inst().shutdown();
			}

			ImGui::EndMenu();
		}
	}

	void EditorMainMenu::end_UI()
	{
		ImGui::EndMainMenuBar();
	}
	void EditorMainMenu::add_window(const s_ptr<EditorUIWindow>& _window)
	{
		if (_window) 
		{ 
			m_editor_windows.push_back(_window); 
		}
	}

}
