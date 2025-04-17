#include "Editor/EditorManager.h"

#include <Engine/EngineMain.h>
#include <Engine/GameEngine.h>

#include <Engine/Util/AtExit.h>
#include <Engine/Util/Serialize/JsonSerializer.h>
#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>
#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_Mesh.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_Mesh.h>

#include <Engine/Game/Collision/CollisionSystem.h>

#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/InputManager.h>
#include <Engine/Manager/PathManager.h>
#include <Engine/Manager/SceneManager.h>

#include "Editor/imgui/imgui.h"
#include "Editor/imgui/imgui_impl_win32.h"
#include "Editor/imgui/imgui_impl_dx11.h"
#include "Editor/Inspector/InspectorBase.h"
#include "Editor/DebugObject/SceneViewer.h"
#include "Editor/Base/EditorBase.h"
#include "Editor/Resource/ResourcesViewer.h"
#include "Editor/Base/MainMenu.h"
#include "Editor/DebugObject/Console.h"
#include "Editor/Widget/Widget_List.h"
#include "Editor/DebugObject/GameObjectHierarchy.h"
#include "Editor/Resource/FBXConverter.h"
#include "Editor/Resource/GraphicsShaderEditor.h"
#include "Editor/DebugObject/DebugObject.h"
#include "Editor/DebugObject/Object.h"
#include "Editor/Resource/MaterialEditor.h"
#include "Editor/Resource/NormalConverter.h"
#include "Editor/Resource/UVCalculator.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace core::editor
{
	constexpr const char* imguiSaveINI = "imgui.ini";
	constexpr const char* imguiSaveJSON = "imgui.json";


	std::unordered_map<std::string, std::shared_ptr<EditorBase>, Hasher_StringView, std::equal_to<>> EditorManager::mGuiWindows{};
	//std::vector<EditorBase*> EditorManager::mGuiWindows{};
	std::vector<std::shared_ptr<EditorObject>> EditorManager::mEditorObjects{};
	std::vector<std::shared_ptr<DebugObject>> EditorManager::mDebugObjects{};

	bool EditorManager::mbEnable{};
	bool EditorManager::mbInitialized{};
	bool EditorManager::m_IsOpenEditorAsDefault{};

	std::unique_ptr<Json::Value> EditorManager::mJsonUIData{};

	ImGuizmo::OPERATION EditorManager::mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

	using namespace math;
	
	void EditorManager::init()
	{
		AtExit::AddFunc(EditorManager::release);

		EngineMain::AddCommonMsgHandleFunc(ImGui_ImplWin32_WndProcHandler);

		// 충돌체의 종류 갯수만큼만 있으면 된다.
		//mDebugObjects.resize((UINT)eColliderType::END);

		//std::shared_ptr<Mesh> rectMesh = ResourceManager<Mesh>::get_inst().Find(::core::name::defaultRes::mesh::DebugRectMesh);
		//std::shared_ptr<Material> material = ResourceManager<Material>::get_inst().Find(::core::name::defaultRes::material::DebugMaterial);

		//mDebugObjects[(UINT)eColliderType::Rect] = std::make_shared<DebugObject>();
		//auto renderer
		//	= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<Com_Renderer_Mesh>();

		//renderer->set_material(material, 0);
		//renderer->set_mesh(rectMesh);

		//std::shared_ptr<Mesh> circleMesh = ResourceManager<Mesh>::get_inst().Find("CircleMesh");

		//mDebugObjects[(UINT)eColliderType::Circle] = std::make_shared<DebugObject>();
		//renderer
		//	= mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<Com_Renderer_Mesh>();

		//renderer->set_material(material, 0);
		//renderer->set_mesh(circleMesh);


		//그리드 이쪽으로 옮겨줘야 한다.
		// Grid Object
		//EditorObject* gridObject = new EditorObject();
		//Com_Renderer_Mesh* gridMr = gridObject->AddComponent<Com_Renderer_Mesh>();
		//gridMr->set_mesh(ResourceManager::Find<Mesh>(L"RectMesh"));
		//gridMr->set_material(ResourceManager<Material>::get_inst().Find(L"GridMaterial"));
		//GridScript* gridScript = gridObject->AddComponent<GridScript>();
		//gridScript->SetCamera(gMainCamera);

		//mEditorObjects.push_back(gridObject);

		ImGuiInitialize();

		InitGuiWindows();


		for (const auto& iter : mGuiWindows)
		{
			iter.second->InitRecursive();
		}
	}

	std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> EditorManager::GetEditorWindowHandleFunction()
	{
		return ImGui_ImplWin32_WndProcHandler;
	}

	std::function<void()> EditorManager::GetEditorRunFunction()
	{
		return EditorManager::Run;
	}

	void EditorManager::Run()
	{
		if (m_IsOpenEditorAsDefault)
		{
			m_IsOpenEditorAsDefault = false;
			SetEnable(true);
		}

		if (
			InputManager::get_inst().GetKeyPress(eKeyCode::LCTRL)
			&&
			InputManager::get_inst().GetKeyPress(eKeyCode::LSHIFT)
			&&
			InputManager::get_inst().GetKeyDown(eKeyCode::E)
			)
		{
			EditorManager::ToggleEnable();
		}

		if (false == mbEnable)
		{
			return;
		}

		update();
		final_update();
		render();

		mbInitialized = true;
	}


	void EditorManager::update()
	{
		ImGuiNewFrame();

		for (const auto& obj : mEditorObjects)
		{
			obj->update();
		}
	}

	void EditorManager::final_update()
	{
		for (const auto& obj : mEditorObjects)
		{
			obj->final_update();
		}

		for (const auto& guiPair : mGuiWindows)
		{
			guiPair.second->final_update();
		}

		if (InputManager::get_inst().GetKeyPress(eKeyCode::Z))
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		}
		if (InputManager::get_inst().GetKeyPress(eKeyCode::X))
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		}
	}

	void EditorManager::render()
	{
		Scene* scene = SceneManager::get_inst().GetActiveScene();
		if (scene)
		{
			CollisionSystem* colsys = scene->GetCollisionSystem();
			if (colsys)
			{
				colsys->render();
			}
		}
		RenderManager::get_inst().render_debug();
		
		//CollisionSystem::Render();

		for (const auto& obj : mEditorObjects)
		{
			obj->render();
		}

		ImGuiRender();
	}

	void EditorManager::release()
	{
		if (false == mbInitialized)
			return;
		mbInitialized = false;

		//IMGUI 내부 세팅 저장
		const std::fs::path& saveDir = PathManager::get_inst().GetResPathRelative();
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
					guiPair.second->serialize_json(&saveVal);
				}
				//delete guiPair.second;
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

		mEditorObjects.clear();

		mEditorObjects.clear();
		mDebugObjects.clear();

		ImGuiRelease();
	}

	Json::Value* EditorManager::CheckJsonSaved(const std::string& _key_path)
	{
		Json::Value* retJson = nullptr;

		if (mJsonUIData->isMember(_key_path))
		{
			retJson = &((*mJsonUIData)[_key_path]);
		}

		return retJson;
	}

	void EditorManager::InitGuiWindows()
	{
		NewGuiWindow<EditorMainMenu>();

		NewGuiWindow<InspectorBase>();

		NewGuiWindow<GameObjectHierarchy>();

		NewGuiWindow<EditorResources>();

		NewGuiWindow<EditorGraphicsShader>();

		NewGuiWindow<EditorFBXConverter>();

		NewGuiWindow<EditorMaterial>();

		NewGuiWindow<EditorNormalConverter>();

		NewGuiWindow<EditorUVCalculator>();
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
		const std::fs::path& saveDir = PathManager::get_inst().GetResPathRelative();
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
		ImGui_ImplWin32_Init(GameEngine::get_inst().GetHwnd());
		ImGui_ImplDX11_Init(RenderManager::get_inst().Device()
			, RenderManager::get_inst().Context());



		//설정 파일들 로드
		//TODO: 여기
		//std::filesystem::path origDir = PathManager::get_inst()->GetPathRel_Content();

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
		//io.Fonts->AddFontFromFileTTF("misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("misc/fonts/Cousine-Regular.ttf", 15.0f);
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
		
		// update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	bool EditorManager::AddGuiWindow(const std::shared_ptr<EditorBase>& _pBase)
	{
		//최상위 윈도우는 이름 자체가 고유값이여야 함
		const std::string_view guiName = _pBase->get_resource_name();

		//중복되는 이름이 있을 경우 unique 이름을 만들어줌
		std::shared_ptr<EditorBase> foundPtr = FindGuiWindow(guiName);
		
		if (foundPtr)
		{
			ERROR_MESSAGE("이미 해당 이름을 가진 윈도우 창이 존재합니다.");
			return false;
			//_pBase->MakeUniqueKeyByName();
		}

		mGuiWindows.insert(std::make_pair(_pBase->get_resource_name(), _pBase));

		_pBase->InitRecursive();

		Json::Value* pJval = CheckJsonSaved(_pBase->GetName());
		if (pJval)
		{
			_pBase->deserialize_json(pJval);
		}


		return true;
	}

	void EditorManager::RenderGuizmo()
	{
		/*
		if (!targetgameobject)
		{
			return;
		}

		const auto& mainCam = RenderManager::Get_main_camera();

		if (!mainCam)
		{
			return;
		}

		//ImGuizmo::SetOrthographic(false);
		//ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

		const auto& tr = targetgameobject->GetComponent<Transform>();
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
				tr->set_local_scale(scale);
			}
			else
			{
				return;
			}
		}
		*/
	}

	void EditorManager::ImGuiRelease()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}


	std::shared_ptr<EditorBase> EditorManager::FindGuiWindow(const std::string_view _key_path)
	{
		std::shared_ptr<EditorBase> pui = nullptr;

		const auto& iter = mGuiWindows.find(_key_path);
		if (iter != mGuiWindows.end())
		{
			pui = iter->second;
		}
		
		return pui;
	}
}
