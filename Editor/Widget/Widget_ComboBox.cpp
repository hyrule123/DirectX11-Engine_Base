#include "Widget_ComboBox.h"


namespace core::editor
{
	const tComboItem EditorWidget_ComboBox::g_void_item{};

	EditorWidget_ComboBox::EditorWidget_ComboBox()
		: EditorBase("ComboBox")
		, m_items()
		, m_current_selected_idx(-1)
		, m_combo_flags()
		, m_callback()
		, m_is_selection_changed()
	{

	}
	EditorWidget_ComboBox::~EditorWidget_ComboBox()
	{
	}
	bool EditorWidget_ComboBox::begin_UI()
	{
		m_is_selection_changed = false;

		const char* preview = nullptr;
		if (is_valid_idx())
		{
			preview = m_items[m_current_selected_idx].m_name.c_str();
		}

		return ImGui::BeginCombo(get_unique_name().c_str(), preview, m_combo_flags);
	}
	void EditorWidget_ComboBox::update_UI()
	{
		

		for (int n = 0; n < (int)m_items.size(); ++n)
		{
			bool isSelected = (m_current_selected_idx == n);

			//여기는 콤보박스에서 특정 값이 선택(클릭)되었을 떄 진입한다.
			if (true == ImGui::Selectable(m_items[n].m_name.c_str(), isSelected))
			{
				m_current_selected_idx = n;
				m_is_selection_changed = true;

				if (m_callback)
					m_callback(m_items[n]);
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
	}
	void EditorWidget_ComboBox::end_UI()
	{
		ImGui::EndCombo();
	}

	void EditorWidget_ComboBox::set_items(const std::vector<tComboItem>& _items)
	{
		clear_items(); 
		m_items = _items;
	}

	void EditorWidget_ComboBox::clear_items()
	{
		m_items.clear();
		m_current_selected_idx = -1;
	}

	void EditorWidget_ComboBox::reset()
	{
		clear_items();
		m_combo_flags = ImGuiComboFlags_::ImGuiComboFlags_None;
	}

	void EditorWidget_ComboBox::set_current_selected(const std::string& _SelectedName)
	{
		size_t size = m_items.size();
		for (size_t i = 0; i < size; i++)
		{
			if (_SelectedName == m_items[i].m_name)
			{
				m_current_selected_idx = (int)i;
				return;
			}
		}

		m_current_selected_idx = -1;
	}

	const tComboItem& EditorWidget_ComboBox::get_current_selected() const
	{
		if (true == is_valid_idx())
			return m_items[m_current_selected_idx];
		else
			return g_void_item;
	}
}
