#include "Editor/Base/MainMenu.h"

#include "Editor/EditorManager.h"

#include <Engine/GameEngine.h>

namespace ehw::editor
{
	EditorMainMenu::EditorMainMenu()
		: EditorBase(strKey::MainMenu)
	{
        //SetSize(ImVec2(100.0f, 100.0f));
	}

	EditorMainMenu::~EditorMainMenu()
	{
	}


	void EditorMainMenu::UpdateUI()
	{
		if (ImGui::BeginMenu("UI Menu"))
		{
			const auto& guis = EditorManager::GetGUIs();
			for (const auto& widget : guis)
			{
				if (this == widget.second.get())
					continue;
				else if (widget.second->GetParent().expired())
				{
					if (ImGui::MenuItem(widget.second->GetStrKey().c_str(), nullptr, widget.second->GetEnablePtr()))
					{
					}
				}
			}

			ImGui::Separator();

			bool p_open = GetEnable();
			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
			{
				GameEngine::GetInst().ShutDown();
			}

			ImGui::EndMenu();
		}
	}

}
