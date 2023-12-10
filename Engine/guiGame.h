#pragma once
#include "EditorWindow.h"

namespace editor
{
	class guiGame : public EditorWindow
	{
	public:
		guiGame();
		virtual ~guiGame();


		virtual void UpdateUI() override;

	private:
	};
}
