
#include "EditorWidget_ComboBox.h"


namespace editor
{
	const EditorWidget_ComboBox::tComboItem EditorWidget_ComboBox::gVoidItem{};

	EditorWidget_ComboBox::EditorWidget_ComboBox()
		: EditorBase("ComboBox")
		, mItems()
		, mCurrentSelected(-1)
		, mComboFlags()
		, mCallback()
		, mbSelectionChanged()
	{

	}
	EditorWidget_ComboBox::~EditorWidget_ComboBox()
	{
	}
	bool EditorWidget_ComboBox::BeginUI()
	{
		mbSelectionChanged = false;

		std::string Preview;
		if (IsIndexValid())
			Preview = mItems[mCurrentSelected].strName;

		return ImGui::BeginCombo(GetStrKey().c_str(), Preview.c_str(), mComboFlags);
	}
	void EditorWidget_ComboBox::UpdateUI()
	{
		

		for (int n = 0; n < (int)mItems.size(); ++n)
		{
			bool isSelected = (mCurrentSelected == n);

			//여기는 콤보박스에서 특정 값이 선택(클릭)되었을 떄 진입한다.
			if (true == ImGui::Selectable(mItems[n].strName.c_str(), isSelected))
			{
				mCurrentSelected = n;
				mbSelectionChanged = true;

				if (mCallback)
					mCallback(mItems[n]);
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
	}
	void EditorWidget_ComboBox::EndUI()
	{
		ImGui::EndCombo();
	}

	void EditorWidget_ComboBox::SetCurrentSelected(const std::string& _SelectedName)
	{
		size_t size = mItems.size();
		for (size_t i = 0; i < size; i++)
		{
			if (_SelectedName == mItems[i].strName)
			{
				mCurrentSelected = (int)i;
				return;
			}
		}

		mCurrentSelected = -1;
	}

	const EditorWidget_ComboBox::tComboItem& EditorWidget_ComboBox::GetCurrentSelected() const
	{
		if (true == IsIndexValid())
			return mItems[mCurrentSelected];
		else
			return gVoidItem;
	}


}
