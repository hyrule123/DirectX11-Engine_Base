#pragma once
#include <Editor/Base/EditorUIBase.h>

namespace core::editor
{
	class EditorUIWindow :
		public EditorUIBase
	{
		CLASS_INFO(EditorUIWindow, EditorUIBase);
	public:
		EditorUIWindow();
		virtual void init() override;

		virtual ~EditorUIWindow();

		virtual bool begin_UI() final;
		//virtual void update_UI() override;
		virtual void end_UI() final;

		void set_size(ImVec2 _v2Size) { m_widget_size = _v2Size; }

		void add_window_flag(ImGuiWindowFlags _flag) { m_ImGui_window_flag |= _flag; }
		void sub_window_flag(ImGuiWindowFlags _flag) { m_ImGui_window_flag &= ~_flag; }
		void set_window_flag(ImGuiWindowFlags _flag) { m_ImGui_window_flag = _flag; }
		ImGuiWindowFlags get_window_flag() const { return m_ImGui_window_flag; }

	private:
		ImVec2 m_widget_size;

		ImGuiWindowFlags m_ImGui_window_flag;

		bool m_b_border;
	};
}


