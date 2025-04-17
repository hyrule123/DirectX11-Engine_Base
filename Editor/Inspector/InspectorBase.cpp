#include "Editor/Inspector/InspectorBase.h"

#include "Editor/Resource/TextureEditor.h"

#include "Editor/Inspector/Inspector_Com_Transform.h"
#include "Editor/Inspector/Inspector_Com_Renderer.h"

#include <Engine/Manager/RenderManager.h>
#include <Engine/Game/GameObject.h>

namespace core::editor
{
	InspectorBase::InspectorBase()
		: EditorWindow(name::Inspector)
		, mTargetGameObject()
		, mTargetResource()
	{
		//컴포넌트의 경우 수동 관리
		mGuiComponents.resize((int)eComponentCategory::END);
		//mGuiResources.resize((int)eResourceType::END);

		mGuiComponents[(int)eComponentCategory::Transform] = new Inspector_Com_Transform;
		mGuiComponents[(int)eComponentCategory::Transform]->SetSize(ImVec2(0.f, 100.f));

		mGuiComponents[(int)eComponentCategory::Renderer] = new Inspector_Com_Renderer;
		mGuiComponents[(int)eComponentCategory::Renderer]->SetSize(ImVec2(0.f, 100.f));

		//mGuiResources[(int)eResourceType::Texture] = new TextureEditor;
	}

	InspectorBase::~InspectorBase()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
			{
				delete mGuiComponents[i];
			}
		}

		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{
		//	if (mGuiResources[i])
		//	{
		//		delete mGuiResources[i];
		//	}
		//}
	}

	void InspectorBase::init()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
				mGuiComponents[i]->InitRecursive();
		}

		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{
		//	if (mGuiResources[i])
		//		mGuiResources[i]->InitRecursive();
		//}
	}

	void InspectorBase::update()
	{
		if (mTargetGameObject && mTargetGameObject->IsDestroyed())
		{
			mTargetGameObject = nullptr;
		}

		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
			{
				if (mTargetGameObject)
				{
					mGuiComponents[i]->SetTarget(mTargetGameObject);
					mGuiComponents[i]->update();
				}
			}
		}

		
		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{

		//	if (mGuiResources[i])
		//	{
		//		if (false == mTargetGameObject.expired())
		//		{
		//			mGuiResources[i]->SetTarget(mTargetResource);
		//			mGuiResources[i]->update();
		//		}
		//	}
		//		
		//}
	}

	void InspectorBase::update_UI()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			IndicatorButton(::core::name::eComponentCategory_String[(UINT)i].data());

			if (mTargetGameObject && mGuiComponents[i])
			{
				mGuiComponents[i]->final_update();
			}
		}

		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{
		//	if ((false == mTargetResource.expired()) && mGuiResources[i])
		//	{
		//		mGuiResources[i]->final_update();
		//	}
		//}
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
