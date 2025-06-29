#include "Editor/Base/EditorEntity.h"

namespace core::editor
{
	EditorEntity::EditorEntity(const std::string_view _static_type_name)
		: Entity(_static_type_name)
		, m_unique_name(_static_type_name)
	{
	}

	void EditorEntity::init()
	{
		Super::init();
		m_b_enable_save = false;
	}

	EditorEntity::~EditorEntity()
	{
	}

	void hilight_text(const char* _label, const ImVec2& _size)
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
}
