#include "Editor/Inspector/Inspector_Com_Transform.h"

#include "Editor/Inspector/InspectorBase.h"

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Transform.h>

namespace ehw::editor
{

	Inspector_Com_Transform::Inspector_Com_Transform()
		: Inspector_Component(eComponentCategory::Transform)
	{
		//SetSize(ImVec2(200.0f, 120.0f));
	}

	Inspector_Com_Transform::~Inspector_Com_Transform()
	{

	}

	void Inspector_Com_Transform::Update()
	{
		if (nullptr == GetTarget())
		{
			return;
		}

		const auto& tr = GetTarget()->GetComponent<Transform>();

		m_position = tr->get_local_position();
		m_rotation = tr->get_local_rotation();
		m_scale = tr->get_local_scale();
	}

	void Inspector_Com_Transform::UpdateUI()
	{
		ImGui::DragFloat3("Position", (float*)&m_position);
		ImGui::DragFloat3("Rotation", (float*)&m_rotation);
		ImGui::DragFloat3("Scale", (float*)&m_scale);

		if (GetTarget())
		{
			const auto& tr = GetTarget()->GetComponent<Transform>();

			tr->set_local_position(m_position);
			tr->set_local_rotation(m_rotation);
			tr->set_local_scale(m_scale);
		}
	}

}
