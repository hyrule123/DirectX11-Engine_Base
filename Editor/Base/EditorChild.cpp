#include "Editor/Base/EditorChild.h"

namespace ehw::editor
{
	EditorChild::EditorChild(const std::string_view _strName)
		: EditorWindow(_strName)
		, mbBorder(true)
		, mSize{}
	{
	}

	EditorChild::~EditorChild()
	{
	}

	bool EditorChild::BeginUI()
	{
		bool retVal = ImGui::BeginChild(get_resource_name().c_str(), mSize, mbBorder, GetImGuiWindowFlag());
		if (false == retVal)
		{
			ImGui::EndChild();
		}

		return retVal;
	}


	void EditorChild::EndUI()
	{
		ImGui::EndChild();
	}
}

