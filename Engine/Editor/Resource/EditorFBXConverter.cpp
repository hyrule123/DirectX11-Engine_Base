#include "Editor/Resource/EditorFBXConverter.h"

#include "Application.h"

#include "Manager/TimeManager.h"
#include "Manager/ThreadPoolManager.h"
#include "Manager/PathManager.h"
#include "Manager/ResourceManager.h"

#include "Util/define_Util.h"

#include "Resource/Model3D/Model3D.h"

namespace editor
{
	EditorFBXConverter::EditorFBXConverter()
		: EditorWindow("FBX Converter")
		, mFBXPath()
		, mOutputDirName()
		, mbStatic(true)
	{
	}

	EditorFBXConverter::~EditorFBXConverter()
	{
	}

	void EditorFBXConverter::Init()
	{
		LoadProjMeshDataCombo();
	}


	void EditorFBXConverter::UpdateUI()
	{

		if (CheckThread())
			return;




		//if (CheckThread())
			//return;


		HilightText("FBX Source File Path");
		if (mFBXPath.empty())
		{
			ImGui::Text("Empty");
		}
		else
		{
			ImGui::Text(mFBXPath.c_str());
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ChooseFBXButton();

		ImGui::SameLine();
		
		if (ImGui::Button("Clear Path", ImVec2(0.f, 35.f)))
		{
			mFBXPath.clear();
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Separator();

		HilightText("Convert Setting");
		ImGui::InputText("Output Directory Name", &mOutputDirName);

		ImGui::Checkbox("Static Mesh?", &mbStatic);

		ConvertFBXButton();

		ImGui::Dummy(ImVec2(0.0f, 30.0f));
		ImGui::Separator();

		AddAnimationFromSameModeling();
	}
	bool EditorFBXConverter::CheckThread()
	{
		bool working = false;
		if (mFutureConvertResult.valid())
		{
			working = true;
			static float waitDot{};
			static int waitDotCount{};
			waitDot += ehw::TimeManager::DeltaTime();
			if (1.f < waitDot)
			{
				waitDot = 0.f;
				++waitDotCount;
				if (4 <= waitDotCount)
				{
					waitDotCount = 0;
				}
			}

			std::future_status status = mFutureConvertResult.wait_for(std::chrono::milliseconds(10));
			if (status == std::future_status::ready)
			{
				ehw::eResult result = mFutureConvertResult.get();
				if (ehw::eResultSuccess(result))
				{
					NOTIFICATION("저장 성공");
				}
				else
				{
					NOTIFICATION("저장 실패");
				}

				//future 초기화해서 invalid로 만들어줌
				mFutureConvertResult = {};
			}
			else if (std::future_status::timeout == status)
			{
				std::string loading = "Loading";
				for (int i = 0; i < waitDotCount; ++i)
				{
					loading += ".";
				}
				HilightText(loading.c_str());
			}
		}
		return working;
	}
	void EditorFBXConverter::ChooseFBXButton()
	{
		if (ImGui::Button("Choose FBX File", ImVec2(0.f, 35.f)))
		{
			std::vector<std::fs::path> extensions = { ".fbx" };

			std::fs::path filePath = ehw::WinAPI::FileDialog(std::fs::current_path(), extensions);
			mFBXPath = filePath.string();

			mOutputDirName = filePath.stem().string();
		}
	}


	void EditorFBXConverter::ConvertFBXButton()
	{
		if (ImGui::Button("Convert FBX File", ImVec2(0.f, 35.f)))
		{
			if (mFBXPath.empty())
			{
				MessageBoxW(nullptr, L"FBX 경로를 설정하지 않았습니다.", nullptr, MB_OK);
				return;
			}
			else if (mOutputDirName.empty())
			{
				MessageBoxW(nullptr, L"출력 폴더를 설정하지 않았습니다.", nullptr, MB_OK);
				return;
			}
			
			mFutureConvertResult = ehw::ThreadPoolManager::EnqueueJob(
				[this]()->ehw::eResult
				{
					ehw::Model3D meshData{};
					ehw::eResult result = meshData.ConvertFBX(mFBXPath, mbStatic, mOutputDirName);
					if (ehw::eResultSuccess(result))
					{
						result = meshData.Save(mOutputDirName, meshData.GetStrKey());
					}

					return result;
				}
			);
		}
	}

	void EditorFBXConverter::AddAnimationFromSameModeling()
	{
		HilightText("Add Animation to Project Model3D");
		ImGui::Dummy(ImVec2(0.f, 10.f));

		mProjMeshDataCombo.InternalUpdate();
		if (ImGui::Button("Refresh List"))
		{
			LoadProjMeshDataCombo();
		}


		ImGui::Dummy(ImVec2(0.f, 20.f));
		if(ImGui::Button("Add Animation", ImVec2(0.f, 35.f)))
		{
			if (mFBXPath.empty())
			{
				NOTIFICATION("FBX 경로를 설정하지 않았습니다.");
			}
			else if (mProjMeshDataCombo.GetCurrentSelected().strName.empty())
			{
				NOTIFICATION("목표 메쉬 데이터를 설정하지 않았습니다.");
			}
			else
			{
				ehw::Model3D::AddAnimationFromFBX(mFBXPath, mProjMeshDataCombo.GetCurrentSelected().strName);
			}
		}
	}

	void EditorFBXConverter::LoadProjMeshDataCombo()
	{
		mProjMeshDataCombo.SetStrKey("Model3D List");
		const std::fs::path& meshPath = ehw::ResourceManager<ehw::Model3D>::GetBaseDir();

		try
		{
			for (const auto& entry : std::fs::directory_iterator(meshPath))
			{
				if (std::fs::is_directory(entry.path()))
				{
					mProjMeshDataCombo.AddItem(entry.path().filename().string());
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

