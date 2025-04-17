#include "Widget_List.h"

#include <Engine/Util/StringConverter.h>

namespace core::editor
{
	Widget_List::Widget_List()
		: EditorBase(name::ListBox)
		, mListIndex(-1)
		, mSize(200.f, 200.f)
		, mEventGUI()
	{
		//SetState(eState::Paused);
	}

	Widget_List::~Widget_List()
	{
	}

	bool Widget_List::BeginUI()
	{
		bool retval = ImGui::BeginListBox(get_resource_name().c_str(), mSize);
		if (retval)
		{
			ImGui::SetNextWindowFocus();
		}
		return retval;
	}

	void Widget_List::update_UI()
	{
		ImGui::SetWindowFocus(get_resource_name().c_str());
		for (size_t i = 0; i < mResourceList.size(); i++)
		{
			bool bselectable = (mListIndex == i);
			if (ImGui::Selectable(mResourceList[i].c_str(), &bselectable))
			{
				mListIndex = (int)i;
			}

			if (bselectable)
			{
				ImGui::SetItemDefaultFocus();
			}
				
			// 해당 아이템이 더블클릭 되었다면
			if (ImGui::IsItemHovered() 
				&& ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (mEventGUI && mEvent)
				{
					mEvent(mResourceList[i].c_str());
				}
				mResourceList.clear();
				ImGui::SetWindowFocus(nullptr);
				Close();
			}
		}
		
	}

	void Widget_List::EndUI()
	{
		ImGui::EndListBox();
	}

	void Widget_List::SetItemList(const std::vector<std::wstring>& _vecItem)
	{
		for (const auto& wName : _vecItem)
		{
			std::string name = StringConverter::Unicode_to_UTF8(wName);
			mResourceList.push_back(name);
		}
	}

	void Widget_List::SetItemList(const std::vector<std::string>& _vecItem)
	{
		mResourceList = _vecItem;
	}
}
