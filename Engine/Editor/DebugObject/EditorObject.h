#pragma once
#include "../../Game/GameObject.h"

namespace editor
{
	class EditorObject : public ehw::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		void Awake();
		void Update();
		void InternalUpdate();
		void Render();

	private:
		
	};
}
