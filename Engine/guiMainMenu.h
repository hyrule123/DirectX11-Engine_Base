#pragma once

#include "EditorBase.h"

namespace editor
{
	class guiMainMenu : public EditorBase
	{
	public:
		guiMainMenu();
		virtual ~guiMainMenu();

		virtual bool BeginUI() final { return ImGui::BeginMainMenuBar(); }
		virtual void UpdateUI() override;
		virtual void EndUI() final { return ImGui::EndMainMenuBar(); }

	private:

	};
}
