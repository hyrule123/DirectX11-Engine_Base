#pragma once
#include <Editor/Base/EditorUIWindow.h>

namespace core::editor
{
	template <class ResourceType>
	class ResourceEditor : public EditorUIWindow
	{
	public:
		ResourceEditor() {}
		virtual ~ResourceEditor() {}

		void update_UI() override;
		
		void set_target(const w_ptr<ResourceType>& _target) { m_target_resource = _target; }
		const w_ptr<ResourceType>& get_target() const { return m_target_resource; }

	private:
		w_ptr<ResourceType> m_target_resource;
	};

	template<class ResourceType>
	inline void ResourceEditor<ResourceType>::update_UI()
	{
		if (m_target_resource.expired())
			return;

		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		//ImGui::Button(name::ArrResName[(UINT)mTarget->GetResType()]);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
}
