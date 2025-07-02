#pragma once
#include <Editor/Base/EditorBase.h>

#include <Engine/Common.h>

namespace core::editor
{
	struct tComboItem
	{
		std::string m_name;
		s_ptr<void> m_data_ptr;
	};

	class EditorWidget_ComboBox :
		public EditorBase
	{
	public:
		EditorWidget_ComboBox();
		virtual ~EditorWidget_ComboBox();

		virtual bool begin_UI() override;
		virtual void update_UI() override;
		virtual void end_UI() override;

		void set_imgui_combo_flag(ImGuiComboFlags _Flags) { m_combo_flags = _Flags; }
		ImGuiComboFlags get_imgui_combo_flag() const { return m_combo_flags; }

		void add_item(const tComboItem& _item) { m_items.push_back(_item); }

		//새로 들어온 벡터값으로 값을 교체
		void set_items(const std::vector<tComboItem>& _items);

		void clear_items();
		void reset();

		size_t get_items_count() const { return m_items.size(); }

		void set_current_idx(int _idx) { m_current_selected_idx = _idx; }
		int	get_current_idx() const { return m_current_selected_idx; }
		void set_current_selected(const std::string& _SelectedName);
		const tComboItem& get_current_selected() const;
		void unselect() { m_current_selected_idx = -1; }

		void set_click_callback(const std::function<void(const tComboItem&)>& _pFunc) { m_callback = _pFunc; }

		bool is_selection_changed() const { return m_is_selection_changed; }

	private:
		bool is_valid_idx() const;

	private:
		const static tComboItem g_void_item;

		std::vector<tComboItem> m_items;
		int m_current_selected_idx;
		ImGuiComboFlags m_combo_flags;

		//함수 호출 대상
		std::function<void(const tComboItem&)> m_callback;

		//선택된 항목이 바뀌었는지(1프레임만 유지)
		bool m_is_selection_changed;
	};

	inline bool EditorWidget_ComboBox::is_valid_idx() const
	{
		return (0 <= m_current_selected_idx && m_current_selected_idx < m_items.size());
	}
}


