#pragma once
#include "Base/EditorWindow.h"

namespace editor
{
	class EditorChild :
		public EditorWindow
	{
	public:
		EditorChild(const std::string_view _strName);
		virtual ~EditorChild();

		virtual bool BeginUI() override;
		virtual void UpdateUI() override {};
		virtual void EndUI() override;

		void SetSize(ImVec2 _v2Size) { mSize = _v2Size; }

	private:
		ImVec2 mSize;
		bool mbBorder;
	};
}


