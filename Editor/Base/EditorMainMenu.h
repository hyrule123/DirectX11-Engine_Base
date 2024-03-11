#pragma once
#include "Base/EditorBase.h"

namespace editor
{
	class EditorMainMenu : public EditorBase
	{
	public:
		EditorMainMenu();
		virtual ~EditorMainMenu();

		virtual bool BeginUI() final { return ImGui::BeginMainMenuBar(); }
		virtual void UpdateUI() override;
		virtual void EndUI() final { return ImGui::EndMainMenuBar(); }

	private:

	};
}
