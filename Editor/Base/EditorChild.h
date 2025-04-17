#pragma once
#include "Editor/Base/EditorWindow.h"

namespace core::editor
{
	class EditorChild :
		public EditorWindow
	{
	public:
		EditorChild(const std::string_view _strName);
		virtual ~EditorChild();

		virtual bool BeginUI() override;
		virtual void update_UI() override {};
		virtual void EndUI() override;

		void SetSize(ImVec2 _v2Size) { mSize = _v2Size; }

	private:
		ImVec2 mSize;
		bool mbBorder;
	};
}


