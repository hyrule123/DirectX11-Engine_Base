#include <Editor/Resource/FBXConverter.h>

#include <Engine/GameEngine.h>

#include <Engine/Manager/TimeManager.h>
#include <Engine/Manager/ThreadPoolManager.h>
#include <Engine/Manager/PathManager.h>
#include <Engine/Manager/ResourceManager.h>
#include <Engine/Util/define_Util.h>
#include <Engine/Resource/Model3D/Model3D.h>

namespace core::editor
{
	EditorFBXConverter::EditorFBXConverter()
		: EditorUIWindow()
		, m_fbx_path()
		, m_output_dir()
		, m_b_static(true)
	{
	}

	EditorFBXConverter::~EditorFBXConverter()
	{
	}

	void EditorFBXConverter::init()
	{
		Super::init();

		set_unique_name("FBX Converter");

		load_meshdata_combobox();
	}


	void EditorFBXConverter::update_UI()
	{
		if (check_thread())
			return;

		hilight_text("FBX Source File Path");
		if (m_fbx_path.empty())
		{
			ImGui::Text("Empty");
		}
		else
		{
			ImGui::Text(m_fbx_path.c_str());
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		select_FBX_button();

		ImGui::SameLine();
		
		if (ImGui::Button("Clear Path", ImVec2(0.f, 35.f)))
		{
			m_fbx_path.clear();
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Separator();

		hilight_text("Convert Setting");
		ImGui::InputText("Output Directory Name", &m_output_dir);

		ImGui::Checkbox("Static Mesh?", &m_b_static);

		convert_FBX_button();

		ImGui::Dummy(ImVec2(0.0f, 30.0f));
		ImGui::Separator();

		add_animation_from_same_skeleton();
	}
	bool EditorFBXConverter::check_thread()
	{
		bool working = false;
		if (m_future_convert_result.valid())
		{
			working = true;
			static float waitDot{};
			static int waitDotCount{};
			waitDot += TimeManager::get_inst().delta_time();
			if (1.f < waitDot)
			{
				waitDot = 0.f;
				++waitDotCount;
				if (4 <= waitDotCount)
				{
					waitDotCount = 0;
				}
			}

			std::future_status status = m_future_convert_result.wait_for(std::chrono::milliseconds(10));
			if (status == std::future_status::ready)
			{
				eResult result = m_future_convert_result.get();
				if (eResult_success(result))
				{
					NOTIFICATION("저장 성공");
				}
				else
				{
					NOTIFICATION("저장 실패");
				}

				//future 초기화해서 invalid로 만들어줌
				m_future_convert_result = {};
			}
			else if (std::future_status::timeout == status)
			{
				std::string loading = "Loading";
				for (int i = 0; i < waitDotCount; ++i)
				{
					loading += ".";
				}
				hilight_text(loading.c_str());
			}
		}
		return working;
	}
	void EditorFBXConverter::select_FBX_button()
	{
		if (ImGui::Button("Choose FBX File", ImVec2(0.f, 35.f)))
		{
			std::vector<std::fs::path> extensions = { ".fbx" };

			std::fs::path filePath = WinAPI::file_open_dialog(std::fs::current_path(), extensions);
			m_fbx_path = filePath.string();

			m_output_dir = filePath.stem().string();
		}
	}


	void EditorFBXConverter::convert_FBX_button()
	{
		if (ImGui::Button("Convert FBX File", ImVec2(0.f, 35.f)))
		{
			if (m_fbx_path.empty())
			{
				MessageBoxW(nullptr, L"FBX 경로를 설정하지 않았습니다.", nullptr, MB_OK);
				return;
			}
			else if (m_output_dir.empty())
			{
				MessageBoxW(nullptr, L"출력 폴더를 설정하지 않았습니다.", nullptr, MB_OK);
				return;
			}
			
			m_future_convert_result = ThreadPoolManager::get_inst().enquque_job(
				[this]()->eResult
				{
					s_ptr<Model3D> meshData = std::make_shared<Model3D>();
					return meshData->convert_fbx(m_fbx_path, m_b_static, m_output_dir);
				}
			);
		}
	}

	void EditorFBXConverter::add_animation_from_same_skeleton()
	{
		hilight_text("Add Animation to Project Model3D");
		ImGui::Dummy(ImVec2(0.f, 10.f));

		m_skeletal_mesh_combobox.final_update();
		if (ImGui::Button("Refresh List"))
		{
			load_meshdata_combobox();
		}


		ImGui::Dummy(ImVec2(0.f, 20.f));
		if(ImGui::Button("Add Animation", ImVec2(0.f, 35.f)))
		{
			if (m_fbx_path.empty())
			{
				NOTIFICATION("FBX 경로를 설정하지 않았습니다.");
			}
			else if (m_skeletal_mesh_combobox.get_current_selected().m_name.empty())
			{
				NOTIFICATION("목표 메쉬 데이터를 설정하지 않았습니다.");
			}
			else
			{
				Model3D::add_animation_from_FBX(m_fbx_path, m_skeletal_mesh_combobox.get_current_selected().m_name);
			}
		}
	}

	void EditorFBXConverter::load_meshdata_combobox()
	{
		m_skeletal_mesh_combobox.set_unique_name("Model3D List");
		m_skeletal_mesh_combobox.reset();
		const std::fs::path& meshPath = ResourceManager<Model3D>::get_inst().get_base_directory();

		try
		{
			for (const auto& entry : std::fs::directory_iterator(meshPath))
			{
				if (std::fs::is_directory(entry.path()))
				{
					tComboItem item{};
					item.m_name = entry.path().filename().string();
					m_skeletal_mesh_combobox.add_item(item);
				}
			}
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return;
		}

	}
}

