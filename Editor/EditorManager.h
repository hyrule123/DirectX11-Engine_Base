#pragma once
#include <Engine/CommonStruct.h>
#include <Engine/define_Macro.h>

#include <Engine/GPU/CommonGPU.h>

#include "Editor/Base/EditorChild.h"
#include "imgui/ImGuizmo.h"


#include <unordered_map>
#include <functional>

namespace ehw::editor
{
	class EditorObject;
	class DebugObject;


	class EditorManager
	{
	public:
		static std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> GetEditorWindowHandleFunction();
		static std::function<void()> GetEditorRunFunction();

		static inline void SetEnable(bool _bEnable);
		static inline bool GetEnable() { return mbEnable; }
		static inline void ToggleEnable() { SetEnable(!mbEnable); }

		static std::shared_ptr<EditorBase> FindGuiWindow(const std::string_view _strKey);

		template <typename T>
		static std::shared_ptr<T> NewGuiWindow();

		static const std::unordered_map<std::string, std::shared_ptr<EditorBase>, tHashFunc_StringView, std::equal_to<>>&
			GetGUIs() { return mGuiWindows; }

	private:
		static void Run();

		static void Init();
		static void Release();

		static void Update();
		static void FinalUpdate();
		static void Render();
		
		//Window 이름으로 저장된 Json 값이 있을 경우 로드함
		static Json::Value* CheckJsonSaved(const std::string& _strKey);

		static void InitGuiWindows();

		//=================== IMGUI ===========================
		static void ImGuiInitialize();
		static void ImGuiRelease();

		static void ImGuiNewFrame();
		static void ImGuiRender();
		
		static bool AddGuiWindow(const std::shared_ptr<EditorBase>& _pBase);

		static inline std::string CreateUniqueImGuiKey(const std::string_view _str, int i);

	private:
		static std::unordered_map<std::string, std::shared_ptr<EditorBase>, tHashFunc_StringView, std::equal_to<>> mGuiWindows;

		static std::vector<std::shared_ptr<EditorObject>> mEditorObjects;
		static std::vector<std::shared_ptr<DebugObject>> mDebugObjects;

		//현재 GUI를 표시할지 여부
		static bool mbEnable;

		//GUI가 최초 1회 초기화 되었는지 여부
		static bool mbInitialized;

		static std::unique_ptr<Json::Value> mJsonUIData;

	private:
		static ImGuizmo::OPERATION mCurrentGizmoOperation;
		static void RenderGuizmo();
	};

	inline void EditorManager::SetEnable(bool _bEnable)
	{
		mbEnable = _bEnable;

		if (mbEnable && (false == mbInitialized))
		{
			EditorManager::Init();
			mbInitialized = true;
		}
	}


	template<typename T>
	inline std::shared_ptr<T> EditorManager::NewGuiWindow()
	{
		static_assert(std::is_base_of_v<EditorBase, T>);

		//editor Child는 무조건 child로 들어가기위한 용도이므로 assert
		static_assert(false == std::is_base_of_v<EditorChild, T>);

		std::shared_ptr<T> retPtr = std::make_shared<T>();

		if (false == AddGuiWindow(std::static_pointer_cast<EditorBase>(retPtr)))
		{
			return nullptr;
		}

		return retPtr;
	}

	inline std::string EditorManager::CreateUniqueImGuiKey(const std::string_view _str, int i)
	{
		std::string uniqStr;
		uniqStr.reserve(_str.size() + 5);
		uniqStr += _str;
		uniqStr += "##";
		uniqStr += std::to_string(i);
		return uniqStr;
	}
}

