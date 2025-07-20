#pragma once
#include <Engine/Game/Component/Script/Script.h>


namespace core
{
	class Camera;
	class GridScript : public Script
	{
		CLASS_INFO(GridScript, Script);
		
	public:
		GridScript();
		virtual ~GridScript();

		virtual void awake();
		virtual void update();

		void SetCamera(const s_ptr<Camera>& camera) { m_camera = camera; }

	private:
		w_ptr<Camera> m_camera;
	};
}
