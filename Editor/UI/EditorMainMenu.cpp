#include "Editor/UI/EditorMainMenu.h"

#include "Editor/EditorManager.h"
#include "Editor/Base/EditorWindow.h"

#include <Engine/GameEngine.h>

namespace core::editor
{
	EditorMainMenu::EditorMainMenu()
		: EditorBase(name::MainMenu)
	{
        //SetSize(ImVec2(100.0f, 100.0f));
	}

	EditorMainMenu::~EditorMainMenu()
	{
	}

	bool EditorMainMenu::begin_UI()
	{
		if (false == Super::begin_UI()) { return false; }

		return ImGui::BeginMainMenuBar();
	}

	void EditorMainMenu::update_UI()
	{
		if (ImGui::BeginMenu("UI Menu"))
		{
			for (size_t i = 0; i < m_editor_windows.size(); ++i)
			{
				s_ptr<EditorWindow> ui = m_editor_windows[i].lock();
				if (ui)
				{
					bool result =
						ImGui::MenuItem(ui->get_unique_name().c_str(), nullptr, is_enabled_ptr());
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

}
