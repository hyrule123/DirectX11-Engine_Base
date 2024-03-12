#pragma once
#include "Editor/Inspector/Inspector_Component.h"

namespace ehw::editor
{
	class Inspector_Com_Transform : public Inspector_Component
	{
	public:
		Inspector_Com_Transform();
		virtual ~Inspector_Com_Transform();

		virtual void Update() override;
		virtual void UpdateUI() override;

	private:
		float3 m_position;
		Quaternion m_rotation;
		float3 m_scale;
	};

}
