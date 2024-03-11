#pragma once
#include "Base/EditorWindow.h"

namespace editor
{
	class EditorGame : public EditorWindow
	{
	public:
		EditorGame();
		virtual ~EditorGame();


		virtual void UpdateUI() override;

	private:
	};
}
