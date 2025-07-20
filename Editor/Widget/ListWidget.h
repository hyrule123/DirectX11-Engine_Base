#pragma once
#include <Editor/Base/EditorUIBase.h>

#include <functional>

namespace core::editor
{
	class EditorUIBase;

	class ListWidget : public EditorUIBase
	{
	public:
		ListWidget();
		virtual ~ListWidget();

		virtual bool begin_UI() final;
		virtual void update_UI() override;
		virtual void end_UI() final;

		void SetItemList(const std::vector<std::wstring>& _vecItem);
		void SetItemList(const std::vector<std::string>& _vecItem);

	private:
		std::vector<std::string> m_elements;
		int m_selected_element_idx;

		ImVec2 m_size;

		//std::function<void(std::string key)> m_selected_callback;
	};
}
