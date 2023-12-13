
#include "EditorManager.h"





#include "../Application.h"
#include "../GameMainWindow.h"
#include "../Util/AtExit.h"

#include "../Resource/Mesh.h"
#include "../Resource/Material.h"

#include "../Game/Component/Transform/Com_Transform.h"
#include "../Game/Component/Renderer/Com_Renderer_Mesh.h"
#include "../Game/Component/Script/GridScript.h"
#include "../Game/Component/Renderer/Com_Renderer_Mesh.h"

#include "../json-cpp/json.h"

#include "../Editor/imgui/imgui.h"
#include "../Editor/imgui/imgui_impl_win32.h"
#include "../Editor/imgui/imgui_impl_dx11.h"
#include "../Editor/Inspector/EditorInspector.h"
#include "../Editor/DebugObject/EditorGame.h"
#include "../Editor/Base/EditorBase.h"
#include "../Editor/Resource/EditorResources.h"
#include "../Editor/Base/EditorMainMenu.h"
#include "../Editor/DebugObject/EditorConsole.h"
#include "../Editor/Widget/EditorWidget_List.h"
#include "../Editor/DebugObject/EditorGameObject.h"
#include "../Editor/Resource/EditorFBXConverter.h"
#include "../Editor/Resource/EditorGraphicsShader.h"
#include "../Editor/DebugObject/DebugObject.h"
#include "../Editor/DebugObject/EditorObject.h"
#include "../Editor/Resource/EditorMaterial.h"
#include "../Editor/Resource/EditorNormalConverter.h"
#include "../Editor/Resource/EditorUVCalculator.h"


#include "ResourceManager.h"
#include "GPUManager.h"
#include "InputManager.h"
#include "PathManager.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace editor
{
	constexpr const char* imguiSaveINI = "imgui.ini";
	constexpr const char* imguiSaveJSON = "imgui.json";


	std::unordered_map<std::string, EditorBase*, ehw::tHashFunc_StringView, std::equal_to<>> EditorManager::mGuiWindows{};
	//std::vector<EditorBase*> EditorManager::mGuiWindows{};
	std::vector<EditorObject*> EditorManager::mEditorObjects{};
	std::vector<DebugObject*> EditorManager::mDebugObjects{};

	bool EditorManager::mbEnable{};
	bool EditorManager::mbInitialized{};

	std::unique_ptr<Json::Value> EditorManager::mJsonUIData{};

	ImGuizmo::OPERATION EditorManager::mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

	using namespace ehw::math;
	
	void EditorManager::Init()
	{
		AtExit::AddFunc(EditorManager::Release);

		::GameMainWindow::RegisterImGuiWndProc(ImGui_ImplWin32_WndProcHandler);

		// 충돌체의 종류 갯수만큼만 있으면 된다.
		mDebugObjects.resize((UINT)ehw::eColliderType::END);

		std::shared_ptr<ehw::Mesh> rectMesh = ehw::ResourceManager::Find<ehw::Mesh>(ehw::strKey::Default::mesh::DebugRectMesh);
		std::shared_ptr<ehw::Material> material = ehw::ResourceManager::Find<ehw::Material>(ehw::strKey::Default::material::DebugMaterial);

		mDebugObjects[(UINT)ehw::eColliderType::Rect] = new DebugObject();
		ehw::Com_Renderer_Mesh* renderer
			= mDebugObjects[(UINT)ehw::eColliderType::Rect]->AddComponent<ehw::Com_Renderer_Mesh>();

		renderer->SetMaterial(material, 0);
		renderer->SetMesh(rectMesh);

		std::shared_ptr<ehw::Mesh> circleMesh = ehw::ResourceManager::Find<ehw::Mesh>("CircleMesh");

		mDebugObjects[(UINT)ehw::eColliderType::Circle] = new DebugObject();
		renderer
			= mDebugObjects[(UINT)ehw::eColliderType::Circle]->AddComponent<ehw::Com_Renderer_Mesh>();

		renderer->SetMaterial(material, 0);
		renderer->SetMesh(circleMesh);


		//그리드 이쪽으로 옮겨줘야 한다.
		// Grid Object
		//EditorObject* gridObject = new EditorObject();
		//ehw::Com_Renderer_Mesh* gridMr = gridObject->AddComponent<ehw::Com_Renderer_Mesh>();
		//gridMr->SetMesh(ehw::ResourceManager::Find<ehw::Mesh>(L"RectMesh"));
		//gridMr->SetMaterial(ehw::ResourceManager::Find<Material>(L"GridMaterial"));
		//ehw::GridScript* gridScript = gridObject->AddComponent<ehw::GridScript>();
		//gridScript->SetCamera(gMainCamera);

		//mEditorObjects.push_back(gridObject);

		ImGuiInitialize();

		InitGuiWindows();


		for (const auto& iter : mGuiWindows)
		{
			iter.second->InitRecursive();
		}
	}

	void EditorManager::Run()
	{
		if (
			ehw::InputManager::GetKeyPress(ehw::eKeyCode::LCTRL)
			&&
			ehw::InputManager::GetKeyPress(ehw::eKeyCode::LSHIFT)
			&&
			ehw::InputManager::GetKeyDown(ehw::eKeyCode::E)
			)
		{
			editor::EditorManager::ToggleEnable();
		}


		if (ehw::InputManager::GetKeyPress(ehw::eKeyCode::Z))
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		}
		if (ehw::InputManager::GetKeyPress(ehw::eKeyCode::X))
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		}

		if (false == mbEnable)
			return;
		Update();
		InternalUpdate();
		Render();

		mbInitialized = true;
	}


	void EditorManager::Update()
	{
		ImGuiNewFrame();

		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}

	void EditorManager::InternalUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->InternalUpdate();
		}

		for (const auto& guiPair : mGuiWindows)
		{
			guiPair.second->InternalUpdate();
		}
	}

	void EditorManager::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}

		auto& DebugMeshes = ehw::RenderManager::GetDebugMeshes();
		for ( ehw::tDebugMesh& mesh : DebugMeshes)
		{
			DebugRender(mesh);
		}
		DebugMeshes.clear();

		ImGuiRender();
	}

	void EditorManager::Release()
	{
		if (false == mbInitialized)
			return;

		//IMGUI 내부 세팅 저장
		const std::fs::path& saveDir = ehw::PathManager::GetResPathRelative();
		std::fs::path savePath = saveDir / imguiSaveINI;
		ImGui::SaveIniSettingsToDisk(savePath.string().c_str());

		//IMGUI 프로젝트 세팅 저장
		savePath.remove_filename();
		savePath /= imguiSaveJSON;
		for (const auto& guiPair : mGuiWindows)
		{
			if (guiPair.second)
			{
				if (guiPair.second->IsSaveEnable())
				{
					//한 파일에 몰아서 저장
					Json::Value& saveVal = (*mJsonUIData.get())[guiPair.first];
					guiPair.second->SaveJson(&saveVal);
				}
				delete guiPair.second;
			}
		}
		mGuiWindows.clear();


		//json 저장
		std::ofstream ofs(savePath);
		if (ofs.is_open())
		{
			ofs << (*mJsonUIData.get());
			ofs.close();
		}
		mJsonUIData.reset();

		
		for (auto& obj : mEditorObjects)
		{
			SAFE_DELETE(obj);
		}

		SAFE_DELETE(mDebugObjects[(UINT)ehw::eColliderType::Rect]);
		SAFE_DELETE(mDebugObjects[(UINT)ehw::eColliderType::Circle]);

		mEditorObjects.clear();
		mDebugObjects.clear();

		ImGuiRelease();
	}

	void EditorManager::DebugRender(ehw::tDebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];
		
		ehw::Com_Transform* tr = debugObj->GetComponent<ehw::Com_Transform>();
		tr->SetRelativePos(mesh.position);
		tr->SetRelativeRotXYZ(mesh.rotatation);
		

		if (mesh.type == ehw::eColliderType::Rect)
			tr->SetRelativeScale(mesh.scale);
		else
			tr->SetRelativeScale(Vector3(mesh.radius));


		ehw::iRenderer* renderer = debugObj->GetComponent<ehw::iRenderer>();
		ehw::Com_Camera* mainCam = ehw::RenderManager::GetMainCam();

		tr->InternalUpdate();

		ehw::Com_Camera::SetGpuViewMatrix(
			mainCam->GetViewMatrix());
		ehw::Com_Camera::SetGpuProjectionMatrix(mainCam->GetProjectionMatrix());

		debugObj->Render();
	}

	Json::Value* EditorManager::CheckJsonSaved(const std::string& _strKey)
	{
		Json::Value* retJson = nullptr;

		if (mJsonUIData->isMember(_strKey))
		{
			retJson = &((*mJsonUIData)[_strKey]);
		}

		return retJson;
	}

	void EditorManager::InitGuiWindows()
	{
		AddGuiWindow<EditorMainMenu>();

		AddGuiWindow<EditorInspector>();

		AddGuiWindow<EditorGameObject>();

		AddGuiWindow<EditorResources>();

		AddGuiWindow<EditorGraphicsShader>();

		AddGuiWindow<EditorFBXConverter>();

		AddGuiWindow<EditorMaterial>();

		AddGuiWindow<EditorNormalConverter>();

		AddGuiWindow<EditorUVCalculator>();
	}

	void EditorManager::ImGuiInitialize()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		//내부 세팅 로드
		const std::fs::path& saveDir = ehw::PathManager::GetResPathRelative();
		std::fs::path savePath = saveDir / imguiSaveINI;
		if (std::fs::exists(savePath))
		{
			ImGui::LoadIniSettingsFromDisk(savePath.string().c_str());
		}
		

		//프로젝트 세팅 로드
		mJsonUIData = std::make_unique<Json::Value>();
		savePath.remove_filename();
		savePath /= imguiSaveJSON;
		std::ifstream ifs(savePath);
		if (ifs.is_open())
		{
			ifs >> *mJsonUIData;
			ifs.close();
		}


		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(ehw::Application::GetHwnd());
		ImGui_ImplDX11_Init(ehw::GPUManager::Device().Get()
			, ehw::GPUManager::Context().Get());



		//설정 파일들 로드
		//TODO: 여기
		//std::filesystem::path origDir = ehw::PathManager::GetInst()->GetPathRel_Content();

		//origDir /= DIRECTORY_NAME::SAVED_SETTING;
		//std::filesystem::path fullPath = origDir / "imgui.ini";
		//io.IniFilename = NULL;

		//ImGui::LoadIniSettingsFromDisk(fullPath.string().c_str());

		//fullPath.clear();
		//fullPath = origDir;
		//fullPath /= "ImGuiSave.json";

		//std::ifstream loadfile(fullPath);
		//if (true == loadfile.is_open())
		//{
		//	loadfile >> m_SavedUIData;
		//	loadfile.close();
		//}


		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != nullptr);

		// Our state

	}

	void EditorManager::ImGuiNewFrame()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		RenderGuizmo();

		ImGuiIO io = ImGui::GetIO();
	}

	void EditorManager::ImGuiRender()
	{
		bool show_demo_window = false;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		
		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void EditorManager::AddGuiWindow(EditorBase* _pBase)
	{
		//최상위 윈도우는 이름 자체가 고유값이여야 함
		const std::string_view guiName = _pBase->GetStrKey();
		EditorBase* findPtr = FindGuiWindow(guiName);
		if (findPtr)
		{
			_pBase->MakeUniqueKeyByName();
		}

		mGuiWindows.insert(std::make_pair(_pBase->GetStrKey(), _pBase));

		if (mbInitialized)
		{
			_pBase->InitRecursive();
		}
	}

	void EditorManager::RenderGuizmo()
	{
		ehw::GameObject* targetgameobject = ehw::RenderManager::GetInspectorGameObject();

		if (!targetgameobject)
		{
			return;
		}

		ehw::Com_Camera* mainCam = ehw::RenderManager::GetMainCam();

		if (!mainCam)
		{
			return;
		}

		//ImGuizmo::SetOrthographic(false);
		//ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

		ehw::Com_Transform* tr = targetgameobject->GetComponent<ehw::Com_Transform>();
		if (nullptr == tr)
		{
			return;
		}

		constexpr float kOffsetX = 60.f;
		constexpr float kOffsetY = 45.f;
		const float		x = ImGui::GetWindowPos().x - 60;
		const float		y = ImGui::GetWindowPos().y - 45;
		const float		width = ImGui::GetWindowViewport()->Size.x;
		const float		height = ImGui::GetWindowViewport()->Size.y;
		ImGuizmo::SetRect(x, y, width, height);

		MATRIX view = mainCam->GetViewMatrix();
		MATRIX projection = mainCam->GetProjectionMatrix();
		MATRIX worldMatrix = tr->GetWorldMat();

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(&worldMatrix.m[0][0], matrixTranslation, matrixRotation, matrixScale);

		MATRIX matTranslation = MATRIX::CreateTranslation(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
		MATRIX matRotation = MATRIX::CreateFromYawPitchRoll(matrixRotation[1], matrixRotation[0], matrixRotation[2]);
		MATRIX matScale = MATRIX::CreateScale(matrixScale[0], matrixScale[1], matrixScale[2]);

		worldMatrix = matScale * matRotation * matTranslation;

		ImGuizmo::Manipulate(&view.m[0][0], &projection.m[0][0], mCurrentGizmoOperation, ImGuizmo::WORLD, &worldMatrix.m[0][0]);

		if (ImGuizmo::IsUsing())
		{
			Vector3 position{};
			Quaternion rotation{};
			Vector3 scale{};
			worldMatrix.Decompose(scale, rotation, position);


			if (mCurrentGizmoOperation == ImGuizmo::TRANSLATE)
			{
				tr->SetRelativePos(position);
			}
			else if (mCurrentGizmoOperation == ImGuizmo::ROTATE)
			{
				// 회전 작동 오류있음
				// 현재사용 X
				Matrix mat = Matrix::CreateFromQuaternion(rotation);
				float x, y, z;

				if (mat._13 > 0.998f)
				{
					y = atan2f(mat._21, mat._22);
					x = XM_PI / 2.0f;
					z = 0;
				}
				else if (mat._13 < -0.998f)
				{
					y = atan2f(mat._21, mat._22);
					x = -XM_PI / 2.0f;
					z = 0;
				}
				else
				{
					y = atan2f(-mat._23, mat._33);
					x = asinf(mat._13);
					z = atan2f(-mat._12, mat._11);
				}

				Vector3 axisRotation(x, y, z);
				tr->SetRelativeRotXYZ(axisRotation);

			}
			else if (mCurrentGizmoOperation == ImGuizmo::SCALE)
			{
				tr->SetRelativeScale(scale);
			}
			else
			{
				return;
			}
		}
	}

	void EditorManager::ImGuiRelease()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}


	EditorBase* EditorManager::FindGuiWindow(const std::string_view _strKey)
	{
		EditorBase* pui = nullptr;

		const auto& iter = mGuiWindows.find(_strKey);
		if (iter != mGuiWindows.end())
		{
			pui = iter->second;
		}
		
		return pui;
	}
}
