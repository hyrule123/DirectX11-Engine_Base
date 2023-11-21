#pragma once
#include "GameObject.h"

namespace ehw
{
	class Monster : public GameObject
	{
	public:
		Monster();
		virtual ~Monster();

		virtual void Init();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
	};
}