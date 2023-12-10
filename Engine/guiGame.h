#pragma once
#include "guiWindow.h"

namespace editor
{
	class guiGame : public guiWindow
	{
	public:
		guiGame();
		virtual ~guiGame();


		virtual void UpdateUI() override;

	private:
	};
}
