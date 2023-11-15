#pragma once
#include "GameObject.h"

namespace ehw
{
	class Player : public GameObject
	{
	public:
		Player();
		virtual ~Player();

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		//GameObject* head;
		//GameObject* body;
	};
}
