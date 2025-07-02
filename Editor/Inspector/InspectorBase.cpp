#include <Editor/Inspector/InspectorBase.h>

#include <Editor/Resource/TextureEditor.h>

#include <Editor/Inspector/TransformInspector.h>
#include <Editor/Inspector/RendererInspector.h>

#include <Engine/Manager/RenderManager.h>
#include <Engine/Game/GameObject.h>

namespace core::editor
{
	InspectorBase::InspectorBase()
		: EditorWindow(name::Inspector)
	{
	}

	InspectorBase::~InspectorBase()
	{
	}

	void InspectorBase::init()
	{
		mTargetGameObject;
		mTargetResource;

		//컴포넌트의 경우 수동 관리
		mGuiComponents.resize((int)eComponentOrder::END);

		mGuiComponents[(int)eComponentOrder::Transform] = new_entity<TransformInspector>();
		mGuiComponents[(int)eComponentOrder::Transform]->set_size(ImVec2(0.f, 100.f));

		mGuiComponents[(int)eComponentOrder::Renderer] = new_entity<RendererInspcetor>();
		mGuiComponents[(int)eComponentOrder::Renderer]->set_size(ImVec2(0.f, 100.f));

		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{
		//	if (mGuiResources[i])
		//		mGuiResources[i]->init_recursive();
		//}
	}

	void InspectorBase::update()
	{
		s_ptr<GameObject> target = mTargetGameObject.lock();
		if (target)
		{
			if (target->is_destroyed())
			{
				mTargetGameObject.reset();
			}

			for (size_t i = 0; i < mGuiComponents.size(); ++i)
			{
				if (mGuiComponents[i])
				{
					mGuiComponents[i]->set_target(target);
					mGuiComponents[i]->update();
				}
			}
		}
	}

	void InspectorBase::update_UI()
	{
		s_ptr<GameObject> target = mTargetGameObject.lock();

		if (target)
		{
			for (size_t i = 0; i < mGuiComponents.size(); ++i)
			{
				IndicatorButton(::core::name::eComponentCategory_String[(UINT)i].data());

				if (mGuiComponents[i])
				{
					mGuiComponents[i]->final_update();
				}
			}
		}
	}

	void InspectorBase::IndicatorButton(const char* _strButtonName)
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
