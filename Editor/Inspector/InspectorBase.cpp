#include <Editor/Inspector/InspectorBase.h>

#include <Editor/Resource/TextureEditor.h>

#include <Editor/Inspector/TransformInspector.h>
#include <Editor/Inspector/RendererInspector.h>

#include <Engine/Manager/RenderManager.h>
#include <Engine/Game/GameObject.h>

namespace core::editor
{
	InspectorBase::InspectorBase()
		: EditorUIWindow()
	{
	}

	InspectorBase::~InspectorBase()
	{
	}

	void InspectorBase::init()
	{
		Super::init();

		set_unique_name(name::Inspector);

		m_target_gameobject;
		m_target_resource;

		//컴포넌트의 경우 수동 관리
		m_component_GUIs.resize((int)eComponentOrder::END);

		m_component_GUIs[(int)eComponentOrder::Transform] = new_entity<TransformInspector>();
		m_component_GUIs[(int)eComponentOrder::Transform]->set_size(ImVec2(0.f, 100.f));

		m_component_GUIs[(int)eComponentOrder::Renderer] = new_entity<RendererInspcetor>();
		m_component_GUIs[(int)eComponentOrder::Renderer]->set_size(ImVec2(0.f, 100.f));

		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{
		//	if (mGuiResources[i])
		//		mGuiResources[i]->init_recursive();
		//}
	}

	void InspectorBase::update()
	{
		s_ptr<GameObject> target = m_target_gameobject.lock();
		if (target)
		{
			if (target->is_destroyed())
			{
				m_target_gameobject.reset();
			}

			for (size_t i = 0; i < m_component_GUIs.size(); ++i)
			{
				if (m_component_GUIs[i])
				{
					m_component_GUIs[i]->set_target(target);
					m_component_GUIs[i]->update();
				}
			}
		}
	}

	void InspectorBase::update_UI()
	{
		s_ptr<GameObject> target = m_target_gameobject.lock();

		if (target)
		{
			for (size_t i = 0; i < m_component_GUIs.size(); ++i)
			{
				indicator_button(::core::name::eComponentCategory_String[(UINT)i].data());

				if (m_component_GUIs[i])
				{
					m_component_GUIs[i]->final_update();
				}
			}
		}
	}

	void InspectorBase::indicator_button(const char* _strButtonName)
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		ImGui::Button(_strButtonName);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
}
