#pragma once
#include "Editor/Base/EditorBase.h"

#include <functional>

namespace core::editor
{
	class EditorBase;
	class Widget_List : public EditorBase
	{
	public:
		Widget_List();
		virtual ~Widget_List();

		virtual bool begin_UI() final;
		virtual void update_UI() override;
		virtual void end_UI() final;

		void SetEvent(EditorBase* widget, std::function<void(const std::string& key)> event)
		{
			m_event_GUI = widget;
			m_selected_callback = std::move(event);
		}

		void SetItemList(const std::vector<std::wstring>& _vecItem);
		void SetItemList(const std::vector<std::string>& _vecItem);

	private:
		std::vector<std::string> mResourceList;
		int mListIndex;

		ImVec2 mSize;

		EditorBase* m_event_GUI;
		std::function<void(std::string key)> m_selected_callback;
	};
}
