#pragma once
#include "EditorBase.h"
#include "define_Struct.h"

namespace editor
{
	class EditorWidget_ComboBox :
		public EditorBase
	{
	public:
		struct tComboItem
		{
			std::string strName;
			ehw::tDataPtr pData;

			tComboItem() : strName(), pData() {};

			tComboItem(const std::string& _strName, ehw::tDataPtr _pData = {})
				: strName(_strName)
				, pData(_pData)
			{}
		};

		EditorWidget_ComboBox();
		virtual ~EditorWidget_ComboBox();

		virtual bool BeginUI() override;
		virtual void UpdateUI() override;
		virtual void EndUI() override;

		void SetImguiComboFlag(ImGuiComboFlags _Flags) { mComboFlags = _Flags; }

		void AddItem(const tComboItem& _tItem) { mItems.push_back(_tItem); }
		void AddItem(const std::string& _strItemName) { mItems.push_back(tComboItem{ _strItemName, }); }

		//새로 들어온 벡터값으로 값을 교체
		void SetItems(const std::vector<std::string>& _strItems);
		void SetItems(const std::vector<tComboItem>& _vecItem) { Reset(); mItems = _vecItem; }

		void Reset() { mItems.clear(); mCurrentSelected = -1; mComboFlags = 0; }

		size_t GetItemCount() const { return mItems.size(); }

		void SetCurrentIndex(int _idx) { mCurrentSelected = _idx; }
		int	GetCurrentIndex() const { return mCurrentSelected; }
		void SetCurrentSelected(const std::string& _SelectedName);
		const tComboItem& GetCurrentSelected() const;
		void UnSelect() { mCurrentSelected = -1; }

		void SetClickCallback(std::function<void(const tComboItem&)> _pFunc) { mCallback = _pFunc; }

		bool IsSelectionChanged() const { return mbSelectionChanged; }


	private:
		bool IsIndexValid() const;

	private:
		const static tComboItem gVoidItem;

		std::vector<tComboItem> mItems;
		int mCurrentSelected;
		ImGuiComboFlags mComboFlags;

		//함수 호출 대상
		std::function<void(const tComboItem&)> mCallback;

		//선택된 항목이 바뀌었는지(1프레임만 유지)
		bool mbSelectionChanged;
	};

	inline bool EditorWidget_ComboBox::IsIndexValid() const
	{
		return (0 <= mCurrentSelected && mCurrentSelected < mItems.size());
	}

	inline void EditorWidget_ComboBox::SetItems(const std::vector<std::string>& _strItems)
	{
		mItems.clear();
		for (size_t i = 0; i < _strItems.size(); ++i)
		{
			mItems.push_back(tComboItem{ _strItems[i],});
		}
	}
}


