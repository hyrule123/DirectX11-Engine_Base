#include "MaterialEditor.h"

#include <Engine/GameEngine.h>

#include <Engine/Manager/PathManager.h>
#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Material/Material.h>
#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Shader/GraphicsShader.h>

#include <Engine/GPU/CommonGPU.h>

#include <Engine/Util/define_Util.h>


namespace core::editor
{
	STR_KEY eRenderingMode_String[(int)eRenderingMode::END] =
	{
		//Deffered
		"deffered_opaque",
		"deffered_mask",

		//"Light",

		//Forward
		"forward_opaque",
		"forward_cutout",
		"forward_transparent",
		"post_process",
	};



	EditorMaterial::EditorMaterial()
		: EditorUIWindow()
		, mTargetMaterial()
		, mShaderCombo{}
		, mbNewMaterial()
	{
		mTargetMaterial = std::make_shared<Material>();
	}
	EditorMaterial::~EditorMaterial()
	{
	}

	void EditorMaterial::init()
	{
		Super::init();

		set_unique_name("Material Editor");

		//쉐이더 파일 목록 업데이트
		mShaderCombo.set_unique_name("Shader Lists");
		refresh_shader_setting_files();

		//렌더링 모드 UI 생성
		mRenderingModeCombo.set_unique_name("Rendering Mode");
		{
			for (int i = 0; i < (int)eRenderingMode::END; ++i)
			{
				tComboItem item{};
				item.m_name = eRenderingMode_String[i];
				mRenderingModeCombo.add_item(item);
			}
		}
	}

	void EditorMaterial::update()
	{
		if (mShaderCombo.is_selection_changed())
		{
			std::string shaderName = mShaderCombo.get_current_selected().m_name;
			if (false == shaderName.empty())
			{
				s_ptr<GraphicsShader> gs = ResourceManager<GraphicsShader>::get_inst().load(shaderName);
			}
		}
	}

	void EditorMaterial::update_UI()
	{
		update_shader();

		ImGui::Separator();

		update_rendering_mode();

		ImGui::Separator();

		update_texture_list();

		ImGui::Separator();

		update_material_const_buffer();

		ImGui::Separator();

		if(ImGui::Button("New Material", ImVec2(0.f, 35.f)))
		{
			new_material();
		}

		ImGui::SameLine();

		update_save_load();
	}
	void EditorMaterial::refresh_shader_setting_files()
	{
		const std::fs::path& shaderPath = ResourceManager<GraphicsShader>::get_inst().get_base_directory();

		if (false == std::fs::exists(shaderPath))
		{
			std::fs::create_directories(shaderPath);
			return;
		}

		mShaderCombo.reset();

		std::fs::directory_iterator dirIter(shaderPath);
		for (auto& entry : dirIter)
		{
			std::fs::path relPath = entry.path().lexically_relative(shaderPath);

			tComboItem item;
			item.m_name = relPath.string();
			mShaderCombo.add_item(item);
		}
	}
	void EditorMaterial::update_shader()
	{
		std::string strCurShader = "Current Shader: ";
		{
			s_ptr<GraphicsShader> curShader = mTargetMaterial->get_shader();
			if (curShader)
			{
				strCurShader += curShader->get_res_filename();
			}
			else
			{
				strCurShader += "None";
			}
		}
		ImGui::Text(strCurShader.c_str());


		mShaderCombo.final_update();
		if (ImGui::Button("Refresh Shaders"))
		{
			refresh_shader_setting_files();
		}

		if (mShaderCombo.is_selection_changed())
		{
			const std::string& shaderKey = mShaderCombo.get_current_selected().m_name;
			if (false == shaderKey.empty())
			{
				s_ptr<GraphicsShader> GS = ResourceManager<GraphicsShader>::get_inst().load(shaderKey);
				mTargetMaterial->set_shader(GS);
			}
		}

		std::string shaderKey = "Shader: ";
		s_ptr<GraphicsShader> shader = mTargetMaterial->get_shader();
		if (shader)
		{
			shaderKey += shader->get_res_filename();
		}
		else
		{
			shaderKey += "None";
		}
		ImGui::Text(shaderKey.c_str());
	}
	void EditorMaterial::update_texture_list()
	{
		for (int i = 0; i < (int)eTextureSlot::END; ++i)
		{
			constexpr const char* texText = "Texture_";
			std::string text = texText;
			text += std::to_string(i);
			text += ": ";
			ImGui::Text(text.c_str());

			ImGui::SameLine();

			auto Tex = mTargetMaterial->get_texture((eTextureSlot)i);
			if (Tex)
			{
				ImGui::Text(Tex->get_res_filename().data());
			}
			else
			{
				ImGui::Text("None");
			}

			ImGui::SameLine();

			std::string ButtonName = "Load##";
			ButtonName += std::to_string(i);
			if (ImGui::Button(ButtonName.c_str()))
			{
				const std::fs::path& texPath = ResourceManager<Texture>::get_inst().get_base_directory();
				
				std::vector<std::fs::path> vecExt{};
				for (size_t i = 0; i < ::core::name::path::extension::Texture_ArrSize; ++i)
				{
					vecExt.push_back(::core::name::path::extension::Texture[i]);
				}
				std::fs::path receivedPath = WinAPI::file_open_dialog(texPath, vecExt);
				if (false == receivedPath.empty())
				{
					std::fs::path PathstrKey = PathManager::get_inst().make_path_strkey(receivedPath);

					s_ptr<Texture> tex = ResourceManager<Texture>::get_inst().load(PathstrKey.string());
					if (tex)
					{
						mTargetMaterial->set_texture((eTextureSlot)i, tex);
					}
					else
					{
						ERROR_MESSAGE("텍스처 찾기 실패");
					}
				}

			}
			ImGui::SameLine();

			ButtonName = "Remove##";
			ButtonName += std::to_string(i);
			if (ImGui::Button(ButtonName.c_str()))
			{
				mTargetMaterial->set_texture((eTextureSlot)i, nullptr);
			}
		}
	}
	void EditorMaterial::update_rendering_mode()
	{
		eRenderingMode mode = mTargetMaterial->get_rendering_mode();
		mRenderingModeCombo.set_current_idx((int)mode);
		mRenderingModeCombo.final_update();
		if (mRenderingModeCombo.is_selection_changed())
		{
			int idx = mRenderingModeCombo.get_current_idx();
			if (0 <= idx)
			{
				mode = (eRenderingMode)idx;
				mTargetMaterial->set_rendering_mode(mode);
			}
		}
	}
	void EditorMaterial::update_material_const_buffer()
	{
		float4 color{};
		
		color = mTargetMaterial->get_diffuse_color();
		if (ImGui::DragFloat4("Diffuse Color",
			reinterpret_cast<float*>(&color)))
		{
			mTargetMaterial->set_diffuse_color(color);
		}

		color = mTargetMaterial->get_specular_color();
		if (ImGui::DragFloat4("Specular Color",
			reinterpret_cast<float*>(&color)))
		{
			mTargetMaterial->set_specular_color(color);
		}

		color = mTargetMaterial->get_ambient_color();
		if (ImGui::DragFloat4("Ambient Color",
			reinterpret_cast<float*>(&color)))
		{
			mTargetMaterial->set_ambient_color(color);
		}

		color = mTargetMaterial->get_emmisive_color();
		if (ImGui::DragFloat4("Emissive Color",
			reinterpret_cast<float*>(&color)))
		{
			mTargetMaterial->set_emmisive_color(color);
		}
	}
	void EditorMaterial::update_save_load()
	{
		if (ImGui::Button("Save to File", ImVec2(0.f, 35.f)))
		{
			if (check_save_possible())
			{
				save_to_file();
			}
			
		}
		
		
		ImGui::SameLine();

		if (ImGui::Button("Load from File", ImVec2(0.f, 35.f)))
		{
			mCurContext = eContext::load_from_file;
			mSaveLoadFileName.clear();

			//ResMgr로부터 로드되어있는 재질 목록 싹 수집
			mCurrentLoadedMtrl.reset();
			const auto& materials = ResourceManager<Material>::get_inst().get_resources();
			for (auto& mtrl : materials)
			{
				tComboItem item{};
				item.m_name = mtrl.second->get_res_filename();
				mCurrentLoadedMtrl.add_item(item);
			}
		}
		load_from_file();
	}
	bool EditorMaterial::check_save_possible()
	{
		bool bPossible = true;
		//저장 조건 확인
		if (nullptr == mTargetMaterial->get_shader() || mTargetMaterial->get_shader()->get_res_filename().empty())
		{
			MessageBoxW(nullptr, L"쉐이더를 설정하지 않았습니다.\n쉐이더는 반드시 설정해야 합니다.", nullptr, MB_OK);
			bPossible = false;
		}
		else if (nullptr == mTargetMaterial->get_texture(eTextureSlot::_0))
		{
			if (IDNO == MessageBoxW(nullptr, L"첫 번째 텍스처(Tex0)가 없습니다.\n(에러는 아님 나중에 코드로 할거면 괜찮음)\n저장할까요?", L"Notification", MB_YESNO))
			{
				bPossible = false;
			}
		}

		return bPossible;
	}
	void EditorMaterial::save_to_file()
	{
		std::fs::path outputPath = std::fs::absolute(ResourceManager<Material>::get_inst().get_base_directory());
			
		outputPath /= mSaveLoadFileName;
		outputPath = WinAPI::file_open_dialog(outputPath, ::core::name::path::extension::Material);

		if (outputPath.empty())
		{
			NOTIFICATION("경로를 설정하지 않았습니다.");
		}

		//저장할 때는 Key값을 바꿔야 하기 때문에 Clone 해서 저장해야 한다.
		//기존 리소스를 그대로 Save하게 되면 Key 값이 변경되어 에러가 발생할 수 있음.
		mTargetMaterial = std::dynamic_pointer_cast<Material>(mTargetMaterial->clone());

		std::string name = outputPath.filename().string();
		mTargetMaterial->get_res_filename(name);

		ResourceManager<Material>::get_inst().save(mTargetMaterial);
	}
	void EditorMaterial::load_from_file()
	{
		if (eContext::load_from_file == mCurContext)
		{
			//모달 창 팝업
			ImGui::SetNextWindowSize(ImVec2{ 400.f, 500.f });
			ImGui::OpenPopup("Material Load");
			if (ImGui::BeginPopupModal("Material Load"))
			{
				hilight_text("Res/Material");
				mCurrentLoadedMtrl.final_update();

				if (ImGui::Button("Load##Material", ImVec2(100.f, 35.f)))
				{
					const std::string& mtrlKey = mCurrentLoadedMtrl.get_current_selected().m_name;
					if (mtrlKey.empty())
					{
						NOTIFICATION("파일을 선택하세요.");
					}
					else
					{
						s_ptr<Material> mtrl = ResourceManager<Material>::get_inst().find(mtrlKey);

						//엔진 기본 Material일 경우에는 Clone
						if (mtrl->is_engine_default_res())
						{
							mtrl = std::dynamic_pointer_cast<Material>(mtrl->clone());
							mtrl->set_engine_default_res(false);
						}

						mTargetMaterial = mtrl;
						if (nullptr == mTargetMaterial)
						{
							NOTIFICATION("제대로 로드되지 않았습니다.");
							mTargetMaterial = std::make_shared<Material>();
						}


						mCurContext = eContext::None;
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel##Material", ImVec2(100.f, 35.f)))
				{
					mCurContext = eContext::None;
				}

				ImGui::Separator();
				
				//파일 선택해서 로드
				if (ImGui::Button("Load From Other Directory", ImVec2(0.f, 35.f)))
				{
					//Res/Material 폴더
					const std::fs::path& mtrlDir = std::fs::absolute(ResourceManager<Material>::get_inst().get_base_directory());

					//Res 폴더
					const std::fs::path& resDir = PathManager::get_inst().get_absolute_resource_directory();

					std::fs::path filePath = WinAPI::file_open_dialog(mtrlDir, ".json");
					if (false == std::fs::exists(filePath))
					{
						NOTIFICATION("파일을 찾지 못했습니다.");
					}
					else
					{
						mTargetMaterial = ResourceManager<Material>::get_inst().load(filePath.filename().string());
						if (nullptr == mTargetMaterial)
						{
							std::wstring errMsg = filePath.wstring();
							errMsg += L"\n불러오기에 실패했습니다.";
							NOTIFICATION_W(errMsg.c_str());
							mTargetMaterial = std::make_shared<Material>();
						}

						//불러왔던 불러오지 못했던 모달 빠져나가줌
						mCurContext = eContext::None;
					}
				}

				ImGui::EndPopup();
			}
		}
	}
	void EditorMaterial::new_material()
	{
		mTargetMaterial = std::make_shared<Material>();
	}
}
