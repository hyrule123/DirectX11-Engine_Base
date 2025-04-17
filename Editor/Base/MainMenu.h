#pragma once
#include "Editor/Base/EditorBase.h"

namespace core::editor
{
	class EditorMainMenu : public EditorBase
	{
	public:
		EditorMainMenu();
		virtual ~EditorMainMenu();

		virtual bool BeginUI() final { return ImGui::BeginMainMenuBar(); }
		virtual void update_UI() override;
		virtual void EndUI() final { return ImGui::EndMainMenuBar(); }

	private:

	};
}
