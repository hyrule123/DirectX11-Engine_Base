#include "PCH_Engine.h"

#include "EditorMainMenu.h"
#include "Application.h"
#include "EditorManager.h"

namespace editor
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
				if (this == widget.second)
					continue;
				else if (nullptr == widget.second->GetParent())
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
				ehw::Application::ShutDown();
			}

			ImGui::EndMenu();
		}
	}

}
