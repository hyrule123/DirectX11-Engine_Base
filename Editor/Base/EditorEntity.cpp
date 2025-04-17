#include "Editor/Base/EditorEntity.h"



namespace core::editor
{
	UINT32 EditorEntity::g_NextID{};

	EditorEntity::EditorEntity(const std::string_view _strName)
		: m_ID(++g_NextID)
		, m_Name(_strName)
		, m_StrKey(_strName)
		, m_bSaveEnable()	
	{
	}


	EditorEntity::~EditorEntity()
	{
	}
	
}

void core::editor::HilightText(const char* _label, const ImVec2& _size)
{
	ImGui::PushID(0);
	static ImColor highLight = { 144, 12, 63 };
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)highLight);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)highLight);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)highLight);

	ImGui::Button(_label, _size);
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}
