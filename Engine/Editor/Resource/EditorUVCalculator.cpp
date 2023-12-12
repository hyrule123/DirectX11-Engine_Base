#include "../../PCH_Engine.h"
#include "EditorUVCalculator.h"

#include "../../Manager/ResourceManager.h"

#include "../../Util/define_Util.h"

#include "../../Manager/PathManager.h"

#include "EditorTexture.h"

namespace editor
{
	EditorUVCalculator::EditorUVCalculator()
		: EditorWindow("UV Calculator")
		, mTexture()
		, mCursorPos()
		, mDragStartPos()
		, mDragEndPos()
		
	{
	}

	EditorUVCalculator::~EditorUVCalculator()
	{
	}

	void EditorUVCalculator::Init()
	{
		SetFlag(ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
	}

	void EditorUVCalculator::UpdateUI()
	{
		UpdateLoadButton();

		UpdateTexture();

		UpdateUVIndicator();
	}

	void EditorUVCalculator::UpdateLoadButton()
	{
		if (ImGui::Button("Load Texture", ImVec2(0.f, 30.f)))
		{
			std::vector<std::fs::path> extensions(ehw::strKey::Ext_Tex_Size);
			for (size_t i = 0; i < ehw::strKey::Ext_Tex_Size; ++i)
			{
				extensions[i] = ehw::strKey::Ext_Tex[i];
			}
			std::fs::path texPath = ehw::WinAPI::FileDialog(ehw::PathManager::GetContentPathAbsolute(ehw::eResourceType::Texture), extensions);


			texPath = ehw::PathManager::MakePathStrKey(texPath);

			mTexture = ehw::ResourceManager::Load<ehw::Texture>(texPath);

			if (mTexture)
			{
				float2 texSize = mTexture->GetSizeFloat();
				if (texSize.x >= texSize.y)
				{
					mTexRenderSize = ImVec2(720.f, texSize.y * (720.f / texSize.x));
				}
				else
				{
					mTexRenderSize = ImVec2(texSize.x * (480.f / texSize.y), 480.f);
				}
			}
			else
			{
				mTexRenderSize = ImVec2{};
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear##Texture", ImVec2(0.f, 30.f)))
		{
			mTexture = nullptr;
		}
	}
	void EditorUVCalculator::UpdateTexture()
	{
		if (mTexture)
		{
			AddFlag(ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

			ImGui::SetWindowSize(ImVec2(mTexRenderSize.x + 50.f, mTexRenderSize.y + 100.f));

			ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			ImGui::Image(mTexture->GetSRV().Get(), mTexRenderSize, ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(0.f, 1.f, 0.f, 1.f));

			if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
			{
				if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
				{
					mDragStartPos = ImGui::GetMousePos();
					mDragStartPos = ImVec2(mDragStartPos.x - cursorPos.x, mDragStartPos.y - cursorPos.y);
				}
				if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
				{
					mDragEndPos = ImGui::GetMousePos();
					mDragEndPos = ImVec2(mDragEndPos.x - cursorPos.x, mDragEndPos.y - cursorPos.y);
				}
			}




			ImVec2 rectStartPos = ImVec2(cursorPos.x + mDragStartPos.x, cursorPos.y + mDragStartPos.y);
			ImVec2 rectEndPos = ImVec2(cursorPos.x + mDragEndPos.x, cursorPos.y + mDragEndPos.y);

			ImDrawList* draw_list = ImGui::GetForegroundDrawList(); //ImGui::GetWindowDrawList();
			draw_list->AddRect(rectStartPos, rectEndPos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 255)));   // Border
			draw_list->AddRectFilled(rectStartPos, rectEndPos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 50)));    // Background

		}
		else
		{
			ImGui::SetWindowSize(ImVec2(0.f, 0.f));
			SubFlag(ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
		}
	}
	void EditorUVCalculator::UpdateUVIndicator()
	{
		//UV 계산
		if (mDragStartPos.x != 0.f
			|| mDragStartPos.y != 0.f
			|| mDragEndPos.x != 0.f
			|| mDragEndPos.y != 0.f
			)
		{
			float2 Start{};
			float2 End{};
			if (mDragStartPos.x < mDragEndPos.x)
			{
				Start.x = mDragStartPos.x;
				End.x = mDragEndPos.x;
			}
			else
			{
				Start.x = mDragEndPos.x;
				End.x = mDragStartPos.x;
			}

			if (mDragStartPos.y < mDragEndPos.y)
			{
				Start.y = mDragStartPos.y;
				End.y = mDragEndPos.y;
			}
			else
			{
				Start.y = mDragEndPos.y;
				End.y = mDragStartPos.y;
			}


			Start.x /= mTexRenderSize.x;
			Start.y /= mTexRenderSize.y;
			End.x /= mTexRenderSize.x;
			End.y /= mTexRenderSize.y;

			HilightText("UV");

			ImGui::DragFloat2("Start", reinterpret_cast<float*>(&Start));
			ImGui::DragFloat2("End", reinterpret_cast<float*>(&End));
		}
	}
}

