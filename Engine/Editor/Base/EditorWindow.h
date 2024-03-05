#pragma once
#include "Editor/Base/EditorBase.h"
#include "-I/"

namespace editor
{
	class EditorWindow :
		public EditorBase
	{

	public:
		EditorWindow(const std::string_view _strName);
		virtual ~EditorWindow();

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


