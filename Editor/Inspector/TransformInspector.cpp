#include <Editor/Inspector/TransformInspector.h>

#include <Editor/Inspector/InspectorBase.h>

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Transform.h>

namespace core::editor
{

	TransformInspector::TransformInspector()
		: ComponentInspector(eComponentOrder::Transform)
	{
		//SetSize(ImVec2(200.0f, 120.0f));
	}

	TransformInspector::~TransformInspector()
	{

	}

	void TransformInspector::update()
	{
		if (nullptr == get_target())
		{
			return;
		}

		const auto& tr = get_target()->get_component<Transform>();

		m_position = tr->get_local_position();
		m_rotation = tr->get_local_rotation();
		m_scale = tr->get_local_scale();
	}

	void TransformInspector::update_UI()
	{
		ImGui::DragFloat3("Position", (float*)&m_position);
		ImGui::DragFloat3("Rotation", (float*)&m_rotation);
		ImGui::DragFloat3("Scale", (float*)&m_scale);

		if (get_target())
		{
			const auto& tr = get_target()->get_component<Transform>();

			tr->set_local_position(m_position);
			tr->set_local_rotation(m_rotation);
			tr->set_local_scale(m_scale);
		}
	}

}
