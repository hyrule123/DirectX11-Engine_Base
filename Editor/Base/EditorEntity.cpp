#include <Editor/Base/EditorEntity.h>

namespace core::editor
{
	EditorEntity::EditorEntity()
		: Entity()
		, m_unique_name()
		, m_b_enable_save()
	{
	}

	void EditorEntity::init()
	{
		Super::init();

		m_unique_name = get_concrete_class_name();
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
