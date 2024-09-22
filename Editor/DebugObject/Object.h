#pragma once
#include <Engine/Game/GameObject.h>

namespace ehw::editor
{
	class EditorObject : public GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		void Awake();
		void update();
		void final_update();
		void render();

	private:
		
	};
}
