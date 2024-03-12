#pragma once
#include "Editor/Base/EditorBase.h"

#include <functional>

namespace ehw::editor
{
	class EditorBase;
	class Widget_List : public EditorBase
	{
	public:
		Widget_List();
		virtual ~Widget_List();

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
