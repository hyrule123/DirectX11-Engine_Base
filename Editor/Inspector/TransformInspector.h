#pragma once
#include <Editor/Inspector/ComponentInspector.h>

namespace core::editor
{
	class TransformInspector : public ComponentInspector
	{
	public:
		TransformInspector();
		virtual ~TransformInspector();

		virtual void update() override;
		virtual void update_UI() override;

	private:
		float3 m_position;
		Quaternion m_rotation;
		float3 m_scale;
	};

}
