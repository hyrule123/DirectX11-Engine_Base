#include "ListWidget.h"

#include <Engine/Util/StringConverter.h>

namespace core::editor
{
	ListWidget::ListWidget()
		: EditorUIBase()
		, m_selected_element_idx(-1)
		, m_size(200.f, 200.f)
		//, m_event_GUI()
	{
		//set_state(eState::Paused);
	}

	ListWidget::~ListWidget()
	{
	}

	bool ListWidget::begin_UI()
	{
		bool retval = ImGui::BeginListBox(get_unique_name().c_str(), m_size);
		if (retval)
		{
			ImGui::SetNextWindowFocus();
		}
		return retval;
	}

	void ListWidget::update_UI()
	{
		ImGui::SetWindowFocus(get_unique_name().c_str());
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			bool bselectable = (m_selected_element_idx == i);
			if (ImGui::Selectable(m_elements[i].c_str(), &bselectable))
			{
				m_selected_element_idx = (int)i;
			}

			if (bselectable)
			{
				ImGui::SetItemDefaultFocus();
			}
				
			// 해당 아이템이 더블클릭 되었다면
			if (ImGui::IsItemHovered() 
				&& ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				//if (m_event_GUI && m_selected_callback)
				//{
				//	m_selected_callback(m_elements[i].c_str());
				//}
				m_elements.clear();
				ImGui::SetWindowFocus(nullptr);
				close();
			}
		}
		
	}

	void ListWidget::end_UI()
	{
		ImGui::EndListBox();
	}

	void ListWidget::SetItemList(const std::vector<std::wstring>& _vecItem)
	{
		for (const auto& wName : _vecItem)
		{
			std::string name = StringConverter::UTF16_to_UTF8(wName);
			m_elements.push_back(name);
		}
	}

	void ListWidget::SetItemList(const std::vector<std::string>& _vecItem)
	{
		m_elements = _vecItem;
	}
}
