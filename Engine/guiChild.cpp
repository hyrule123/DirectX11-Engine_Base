#include "PCH_Engine.h"
#include "guiChild.h"

editor::guiChild::guiChild(const std::string_view _strName)
    : EditorWindow(_strName)
    , mbBorder(true)
	, mSize{}
{
}

editor::guiChild::~guiChild()
{
}

bool editor::guiChild::BeginUI()
{
	bool retVal = ImGui::BeginChild(GetStrKey().c_str(), mSize, mbBorder, GetImGuiWindowFlag());
	if (false == retVal)
	{
		ImGui::EndChild();
	}

	return retVal;
}


void editor::guiChild::EndUI()
{
    ImGui::EndChild();
}
