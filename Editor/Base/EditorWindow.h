#pragma once
#include "Editor/Base/EditorBase.h"

namespace ehw::editor
{
	class EditorWindow :
		public EditorBase
	{

	public:
		EditorWindow(const std::string_view _strName);
		virtual ~EditorWindow();

		virtual bool BeginUI() override;
		virtual void update_UI() {};
		virtual void EndUI() override;

		void SetFlag(ImGuiWindowFlags _flag) { mWindowFlag = _flag; }
		void AddFlag(ImGuiWindowFlags _flag) { mWindowFlag |= _flag; }
		void SubFlag(ImGuiWindowFlags _flag) { mWindowFlag &= ~_flag; }
		ImGuiWindowFlags GetImGuiWindowFlag() const { return mWindowFlag; }

	private:
		ImGuiWindowFlags mWindowFlag;
	};
}


