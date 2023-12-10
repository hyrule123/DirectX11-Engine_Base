#pragma once
#include "EditorBase.h"

namespace editor
{
	class guiWindow :
		public EditorBase
	{

	public:
		guiWindow(const std::string_view _strName);
		virtual ~guiWindow();

		virtual bool BeginUI() override;
		virtual void UpdateUI() {};
		virtual void EndUI() override;

		void SetFlag(ImGuiWindowFlags _flag) { mWindowFlag = _flag; }
		void AddFlag(ImGuiWindowFlags _flag) { mWindowFlag |= _flag; }
		void SubFlag(ImGuiWindowFlags _flag) { mWindowFlag &= ~_flag; }
		ImGuiWindowFlags GetImGuiWindowFlag() const { return mWindowFlag; }

	private:
		ImGuiWindowFlags mWindowFlag;
	};
}


