#pragma once
#include "EditorBase.h"
#include <functional>

namespace editor
{
	class EditorBase;
	class guiList : public EditorBase
	{
	public:
		guiList();
		virtual ~guiList();

		virtual bool BeginUI() final;
		virtual void UpdateUI() override;
		virtual void EndUI() final;

		void SetEvent(EditorBase* widget, std::function<void(const std::string& key)> event)
		{
			mEventGUI = widget;
			mEvent = std::move(event);
		}

		void SetItemList(const std::vector<std::wstring>& _vecItem);
		void SetItemList(const std::vector<std::string>& _vecItem);

	private:
		std::vector<std::string> mResourceList;
		int mListIndex;

		ImVec2 mSize;

		EditorBase* mEventGUI;
		std::function<void(std::string key)> mEvent;
	};
}
