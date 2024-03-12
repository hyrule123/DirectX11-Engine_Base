#include "Editor/Inspector/Inspector_Com_Transform.h"

#include "Editor/Inspector/InspectorBase.h"

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Transform/Com_Transform.h>

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
		if (GetTarget().expired())
		{
			return;
		}

		const auto& tr = GetTarget().lock()->GetComponent<Com_Transform>();

		m_position = tr->GetLocalPosition();
		m_rotation = tr->GetLocalRotation();
		m_scale = tr->GetLocalScale();
	}

	void Inspector_Com_Transform::UpdateUI()
	{
		ImGui::DragFloat3("Position", (float*)&m_position);
		ImGui::DragFloat3("Rotation", (float*)&m_rotation);
		ImGui::DragFloat3("Scale", (float*)&m_scale);

		if (false == GetTarget().expired())
		{
			const auto& tr = GetTarget().lock()->GetComponent<Com_Transform>();

			tr->SetLocalPosition(m_position);
			tr->SetLocalRotation(m_rotation);
			tr->SetLocalScale(m_scale);
		}
	}

}
