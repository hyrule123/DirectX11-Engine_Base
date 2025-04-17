#include "Editor/Base/EditorWindow.h"

namespace core::editor
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
		bool retVal = ImGui::Begin(get_resource_name().c_str(), GetEnablePtr(), mWindowFlag);
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
