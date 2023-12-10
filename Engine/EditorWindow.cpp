#include "PCH_Engine.h"
#include "EditorWindow.h"


namespace editor
{
	EditorWindow::EditorWindow(const std::string_view _strName)
		: EditorBase(_strName)
		, mWindowFlag()
	{
		SetSaveEnable(true);
	}
	EditorWindow::~EditorWindow()
	{
	}
	bool EditorWindow::BeginUI()
	{
		bool retVal = ImGui::Begin(GetStrKey().c_str(), GetEnablePtr(), mWindowFlag);
		if(false == retVal)
		{
			ImGui::End();
		}

		return retVal;
	}
	void EditorWindow::EndUI()
	{
		ImGui::End();
	}
}
