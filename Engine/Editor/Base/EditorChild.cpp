
#include "EditorChild.h"

editor::EditorChild::EditorChild(const std::string_view _strName)
    : EditorWindow(_strName)
    , mbBorder(true)
	, mSize{}
{
}

editor::EditorChild::~EditorChild()
{
}

bool editor::EditorChild::BeginUI()
{
	bool retVal = ImGui::BeginChild(GetStrKey().c_str(), mSize, mbBorder, GetImGuiWindowFlag());
	if (false == retVal)
	{
		ImGui::EndChild();
	}

	return retVal;
}


void editor::EditorChild::EndUI()
{
    ImGui::EndChild();
}
