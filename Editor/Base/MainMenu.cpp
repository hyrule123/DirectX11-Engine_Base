#include "Editor/Base/MainMenu.h"

#include "Editor/EditorManager.h"

#include <Engine/GameEngine.h>

namespace ehw::editor
{
	EditorMainMenu::EditorMainMenu()
		: EditorBase(name::MainMenu)
	{
        //SetSize(ImVec2(100.0f, 100.0f));
	}

	EditorMainMenu::~EditorMainMenu()
	{
	}


	void EditorMainMenu::update_UI()
	{
		if (ImGui::BeginMenu("UI Menu"))
		{
			const auto& guis = EditorManager::GetGUIs();
			for (const auto& widget : guis)
			{
				if (this == widget.second.get())
					continue;
				else if (widget.second->get_parent().expired())
				{
					if (ImGui::MenuItem(widget.second->get_resource_name().c_str(), nullptr, widget.second->GetEnablePtr()))
					{
					}
				}
			}

			ImGui::Separator();

			bool p_open = GetEnable();
			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
			{
				GameEngine::get_inst().ShutDown();
			}

			ImGui::EndMenu();
		}
	}

}
