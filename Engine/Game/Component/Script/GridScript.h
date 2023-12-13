#pragma once
#include "iScript.h"
#include "../Camera/Com_Camera.h"

namespace ehw
{
	class GridScript : public iScript
	{
	public:
		GridScript();
		virtual ~GridScript();

		virtual void Awake();
		virtual void Update();

		void SetCamera(Com_Camera* camera) { mCamera = camera; }

	private:
		Com_Camera* mCamera;
	};
}
