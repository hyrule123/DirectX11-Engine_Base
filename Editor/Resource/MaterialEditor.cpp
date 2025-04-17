#include "Editor/Resource/MaterialEditor.h"

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
		: EditorWindow("Material Editor")
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
		//쉐이더 파일 목록 업데이트
		mShaderCombo.SetStrKey("Shader Lists");
		RefreshShaderSettingFiles();

		//렌더링 모드 UI 생성
		mRenderingModeCombo.SetStrKey("Rendering Mode");
		{
			for (int i = 0; i < (int)eRenderingMode::END; ++i)
			{
				mRenderingModeCombo.AddItem(eRenderingMode_String[i]);
			}
		}
	}

	void EditorMaterial::update()
	{
		if (mShaderCombo.IsSelectionChanged())
		{
			std::string shaderName = mShaderCombo.GetCurrentSelected().strName;
			if (false == shaderName.empty())
			{
				std::shared_ptr<GraphicsShader> gs = ResourceManager<GraphicsShader>::get_inst().load(shaderName);
			}
		}
	}

	void EditorMaterial::update_UI()
	{
		UpdateShader();

		ImGui::Separator();

		UpdateRenderingMode();

		ImGui::Separator();

		UpdateTextureList();

		ImGui::Separator();

		UpdateMtrlConstBuffer();

		ImGui::Separator();

		if(ImGui::Button("New Material", ImVec2(0.f, 35.f)))
		{
			NewMaterial();
		}

		ImGui::SameLine();

		UpdateSaveLoad();
	}
	void EditorMaterial::RefreshShaderSettingFiles()
	{
		const std::fs::path& shaderPath = ResourceManager<GraphicsShader>::get_inst().GetBaseDir();

		if (false == std::fs::exists(shaderPath))
		{
			std::fs::create_directories(shaderPath);
			return;
		}

		mShaderCombo.ClearItems();

		std::fs::directory_iterator dirIter(shaderPath);
		for (auto& entry : dirIter)
		{
			std::fs::path relPath = entry.path().lexically_relative(shaderPath);
			mShaderCombo.AddItem(relPath.string());
		}
	}
	void EditorMaterial::UpdateShader()
	{
		std::string strCurShader = "Current Shader: ";
		{
			std::shared_ptr<GraphicsShader> curShader = mTargetMaterial->get_shader();
			if (curShader)
			{
				strCurShader += curShader->get_resource_name();
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
			RefreshShaderSettingFiles();
		}

		if (mShaderCombo.IsSelectionChanged())
		{
			const std::string& shaderKey = mShaderCombo.GetCurrentSelected().strName;
			if (false == shaderKey.empty())
			{
				std::shared_ptr<GraphicsShader> GS = ResourceManager<GraphicsShader>::get_inst().load(shaderKey);
				mTargetMaterial->set_shader(GS);
			}
		}

		std::string shaderKey = "Shader: ";
		std::shared_ptr<GraphicsShader> shader = mTargetMaterial->get_shader();
		if (shader)
		{
			shaderKey += shader->get_resource_name();
		}
		else
		{
			shaderKey += "None";
		}
		ImGui::Text(shaderKey.c_str());
	}
	void EditorMaterial::UpdateTextureList()
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
				ImGui::Text(Tex->get_resource_name().data());
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
				const std::fs::path& texPath = ResourceManager<Texture>::get_inst().GetBaseDir();
				
				std::vector<std::fs::path> vecExt{};
				for (size_t i = 0; i < ::core::name::path::extension::Texture_ArrSize; ++i)
				{
					vecExt.push_back(::core::name::path::extension::Texture[i]);
				}
				std::fs::path receivedPath = WinAPI::FileDialog(texPath, vecExt);
				if (false == receivedPath.empty())
				{
					std::fs::path PathstrKey = PathManager::get_inst().MakePathStrKey(receivedPath);

					std::shared_ptr<Texture> tex = ResourceManager<Texture>::get_inst().load(PathstrKey.string());
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
	void EditorMaterial::UpdateRenderingMode()
	{
		eRenderingMode mode = mTargetMaterial->get_rendering_mode();
		mRenderingModeCombo.SetCurrentIndex((int)mode);
		mRenderingModeCombo.final_update();
		if (mRenderingModeCombo.IsSelectionChanged())
		{
			int idx = mRenderingModeCombo.GetCurrentIndex();
			if (0 <= idx)
			{
				mode = (eRenderingMode)idx;
				mTargetMaterial->set_rendering_mode(mode);
			}
		}
	}
	void EditorMaterial::UpdateMtrlConstBuffer()
	{
		float4 color{};
		
		color = mTargetMaterial->GetDiffuseColor();
		if (ImGui::DragFloat4("Diffuse Color",
			reinterpret_cast<float*>(&color)))
		{
			mTargetMaterial->SetDiffuseColor(color);
		}

		color = mTargetMaterial->GetSpecularColor();
		if (ImGui::DragFloat4("Specular Color",
			reinterpret_cast<float*>(&color)))
		{
			mTargetMaterial->SetSpecularColor(color);
		}

		color = mTargetMaterial->GetAmbientColor();
		if (ImGui::DragFloat4("Ambient Color",
			reinterpret_cast<float*>(&color)))
		{
			mTargetMaterial->SetAmbientColor(color);
		}

		color = mTargetMaterial->GetEmissiveColor();
		if (ImGui::DragFloat4("Emissive Color",
			reinterpret_cast<float*>(&color)))
		{
			mTargetMaterial->SetEmissiveColor(color);
		}
	}
	void EditorMaterial::UpdateSaveLoad()
	{
		if (ImGui::Button("Save to File", ImVec2(0.f, 35.f)))
		{
			if (CheckSavePossible())
			{
				SaveToFile();
			}
			
		}
		
		
		ImGui::SameLine();

		if (ImGui::Button("Load from File", ImVec2(0.f, 35.f)))
		{
			mCurContext = eContext::LoadFromFile;
			mSaveLoadFileName.clear();

			//ResMgr로부터 로드되어있는 재질 목록 싹 수집
			mCurrentLoadedMtrl.ClearItems();
			const auto& materials = ResourceManager<Material>::get_inst().GetResources();
			for (auto& mtrl : materials)
			{
				mCurrentLoadedMtrl.AddItem(mtrl.second->get_resource_name());
			}
		}
		LoadFromFile();
	}
	bool EditorMaterial::CheckSavePossible()
	{
		bool bPossible = true;
		//저장 조건 확인
		if (nullptr == mTargetMaterial->get_shader() || mTargetMaterial->get_shader()->get_resource_name().empty())
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
	void EditorMaterial::SaveToFile()
	{
		std::fs::path outputPath = std::fs::absolute(ResourceManager<Material>::get_inst().GetBaseDir());
			
		outputPath /= mSaveLoadFileName;
		outputPath = WinAPI::FileDialog(outputPath, ::core::name::path::extension::Material);

		if (outputPath.empty())
		{
			NOTIFICATION("경로를 설정하지 않았습니다.");
		}

		//저장할 때는 Key값을 바꿔야 하기 때문에 Clone 해서 저장해야 한다.
		//기존 리소스를 그대로 Save하게 되면 Key 값이 변경되어 에러가 발생할 수 있음.
		mTargetMaterial = std::shared_ptr<Material>(dynamic_cast<Material*>(mTargetMaterial->Clone()));

		std::string name = outputPath.filename().string();
		mTargetMaterial->set_resource_name(name);

		ResourceManager<Material>::get_inst().save(mTargetMaterial);
	}
	void EditorMaterial::LoadFromFile()
	{
		if (eContext::LoadFromFile == mCurContext)
		{
			//모달 창 팝업
			ImGui::SetNextWindowSize(ImVec2{ 400.f, 500.f });
			ImGui::OpenPopup("Material Load");
			if (ImGui::BeginPopupModal("Material Load"))
			{
				HilightText("Res/Material");
				mCurrentLoadedMtrl.final_update();

				if (ImGui::Button("Load##Material", ImVec2(100.f, 35.f)))
				{
					const std::string& mtrlKey = mCurrentLoadedMtrl.GetCurrentSelected().strName;
					if (mtrlKey.empty())
					{
						NOTIFICATION("파일을 선택하세요.");
					}
					else
					{
						std::shared_ptr<Material> mtrl = ResourceManager<Material>::get_inst().find(mtrlKey);

						//엔진 기본 Material일 경우에는 Clone
						if (mtrl->IsEngineDefaultRes())
						{
							mtrl = std::shared_ptr<Material>(dynamic_cast<Material*>(mtrl->Clone()));
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
					const std::fs::path& mtrlDir = std::fs::absolute(ResourceManager<Material>::get_inst().GetBaseDir());

					//Res 폴더
					const std::fs::path& resDir = PathManager::get_inst().GetResPathAbsolute();

					std::fs::path filePath = WinAPI::FileDialog(mtrlDir, ".json");
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
	void EditorMaterial::NewMaterial()
	{
		mTargetMaterial = std::make_shared<Material>();
	}
}
