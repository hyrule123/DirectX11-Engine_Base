
#include "EditorWidget_List.h"

#include "../../Util/define_Util.h"
#include "../../Util/EngineString.h"

namespace editor
{
	EditorWidget_List::EditorWidget_List()
		: EditorBase(strKey::ListBox)
		, mListIndex(-1)
		, mSize(200.f, 200.f)
		, mEventGUI()
	{
		//SetState(eState::Paused);
	}

	EditorWidget_List::~EditorWidget_List()
	{
	}

	bool EditorWidget_List::BeginUI()
	{
		bool retval = ImGui::BeginListBox(GetStrKey().c_str(), mSize);
		if (retval)
		{
			ImGui::SetNextWindowFocus();
		}
		return retval;
	}

	void EditorWidget_List::UpdateUI()
	{
		ImGui::SetWindowFocus(GetStrKey().c_str());
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

	void EditorWidget_List::EndUI()
	{
		ImGui::EndListBox();
	}

	void EditorWidget_List::SetItemList(const std::vector<std::wstring>& _vecItem)
	{
		for (const auto& wName : _vecItem)
		{
			std::string name = EngineString::StaticConvertUnicodeToUTF8(wName);
			mResourceList.push_back(name);
		}
	}

	void EditorWidget_List::SetItemList(const std::vector<std::string>& _vecItem)
	{
		mResourceList = _vecItem;
	}
}
