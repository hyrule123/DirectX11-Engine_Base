#include "UVCalculator.h"

#include <Editor/Resource/TextureEditor.h>

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/PathManager.h>

#include <Engine/Util/define_Util.h>

#include <Engine/Resource/Texture.h>

namespace core::editor
{
	EditorUVCalculator::EditorUVCalculator()
		: EditorUIWindow("UV Calculator")
		, m_texture()
		, m_cursor_position()
		, m_drag_start_position()
		, m_drag_end_position()
		
	{
	}

	EditorUVCalculator::~EditorUVCalculator()
	{
	}

	void EditorUVCalculator::init()
	{
		set_window_flag(ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
	}

	void EditorUVCalculator::update_UI()
	{
		update_load_button();

		update_texture();

		update_UV_indicator();
	}

	void EditorUVCalculator::update_load_button()
	{
		if (ImGui::Button("Load Texture", ImVec2(0.f, 30.f)))
		{
			std::vector<std::fs::path> extensions(::core::name::path::extension::Texture_ArrSize);
			for (size_t i = 0; i < ::core::name::path::extension::Texture_ArrSize; ++i)
			{
				extensions[i] = ::core::name::path::extension::Texture[i];
			}

			const std::fs::path& absTexPath = std::fs::absolute(ResourceManager<Texture>::get_inst().get_base_directory());

			std::fs::path texPath = WinAPI::file_open_dialog(absTexPath, extensions);


			texPath = PathManager::get_inst().make_path_strkey(texPath);

			m_texture = ResourceManager<Texture>::get_inst().load(texPath.string());

			if (m_texture)
			{
				float2 texSize = m_texture->get_texture_size_float();
				if (texSize.x >= texSize.y)
				{
					m_texture_render_size = ImVec2(720.f, texSize.y * (720.f / texSize.x));
				}
				else
				{
					m_texture_render_size = ImVec2(texSize.x * (480.f / texSize.y), 480.f);
				}
			}
			else
			{
				m_texture_render_size = ImVec2{};
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear##Texture", ImVec2(0.f, 30.f)))
		{
			m_texture = nullptr;
		}
	}
	void EditorUVCalculator::update_texture()
	{
		if (m_texture)
		{
			ImGuiWindowFlags flag = get_window_flag();
			flag |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
			set_window_flag(flag);

			ImGui::SetWindowSize(ImVec2(m_texture_render_size.x + 50.f, m_texture_render_size.y + 100.f));

			ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			ImGui::Image(m_texture->get_SRV().Get(), m_texture_render_size, ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(0.f, 1.f, 0.f, 1.f));

			if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
			{
				if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
				{
					m_drag_start_position = ImGui::GetMousePos();
					m_drag_start_position = ImVec2(m_drag_start_position.x - cursorPos.x, m_drag_start_position.y - cursorPos.y);
				}
				if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
				{
					m_drag_end_position = ImGui::GetMousePos();
					m_drag_end_position = ImVec2(m_drag_end_position.x - cursorPos.x, m_drag_end_position.y - cursorPos.y);
				}
			}




			ImVec2 rectStartPos = ImVec2(cursorPos.x + m_drag_start_position.x, cursorPos.y + m_drag_start_position.y);
			ImVec2 rectEndPos = ImVec2(cursorPos.x + m_drag_end_position.x, cursorPos.y + m_drag_end_position.y);

			ImDrawList* draw_list = ImGui::GetForegroundDrawList(); //ImGui::GetWindowDrawList();
			draw_list->AddRect(rectStartPos, rectEndPos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 255)));   // Border
			draw_list->AddRectFilled(rectStartPos, rectEndPos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 50)));    // Background

		}
		else
		{
			ImGui::SetWindowSize(ImVec2(0.f, 0.f));

			ImGuiWindowFlags flag = get_window_flag();
			flag &= ~ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
			set_window_flag(flag);
		}
	}
	void EditorUVCalculator::update_UV_indicator()
	{
		//UV 계산
		if (m_drag_start_position.x != 0.f
			|| m_drag_start_position.y != 0.f
			|| m_drag_end_position.x != 0.f
			|| m_drag_end_position.y != 0.f
			)
		{
			float2 Start{};
			float2 End{};
			if (m_drag_start_position.x < m_drag_end_position.x)
			{
				Start.x = m_drag_start_position.x;
				End.x = m_drag_end_position.x;
			}
			else
			{
				Start.x = m_drag_end_position.x;
				End.x = m_drag_start_position.x;
			}

			if (m_drag_start_position.y < m_drag_end_position.y)
			{
				Start.y = m_drag_start_position.y;
				End.y = m_drag_end_position.y;
			}
			else
			{
				Start.y = m_drag_end_position.y;
				End.y = m_drag_start_position.y;
			}


			Start.x /= m_texture_render_size.x;
			Start.y /= m_texture_render_size.y;
			End.x /= m_texture_render_size.x;
			End.y /= m_texture_render_size.y;

			hilight_text("UV");

			ImGui::DragFloat2("Start", reinterpret_cast<float*>(&Start));
			ImGui::DragFloat2("End", reinterpret_cast<float*>(&End));
		}
	}
}

