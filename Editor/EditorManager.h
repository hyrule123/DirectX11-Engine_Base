#pragma once
#include <Engine/Common.h>
#include <Engine/define_Macro.h>

#include <Engine/GPU/CommonGPU.h>

#include "imgui/ImGuizmo.h"

#include <unordered_map>
#include <functional>

namespace core::editor
{
	class EditorObject;
	class DebugObject;
	class EditorBase;
	class EditorMainMenu;

	class EditorManager
	{
		DECLARE_SINGLETON(EditorManager);
	private:
		EditorManager();
		~EditorManager();

	public:
		std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> get_editor_window_func();
		std::function<void()> get_editor_run_func();

		void set_enable(bool _bEnable);
		inline bool get_enable() { return mbEnable; }
		inline void toggle_enable() { set_enable(!mbEnable); }
		void open_editor_as_default(bool _enable) { m_IsOpenEditorAsDefault = _enable; }

		s_ptr<EditorBase> find_editor_window(const std::string_view _UI_name);

		template <typename T>
		s_ptr<T> new_editor_UI();

		const std::vector<s_ptr<EditorBase>>&
			get_editor_windows() { return m_editor_UIs; }

	private:
		void run();

		void init();
		void release();

		void update();
		void final_update();
		void render();

		//Window 이름으로 저장된 Json 값이 있을 경우 로드함
		Json::Value* check_json_saved(const std::string& _key_path);

		void init_default_windows();

		//=================== IMGUI ===========================
		void imgui_initialize();
		void imgui_release();

		void imgui_new_frame();
		void imgui_render();
		
		s_ptr<EditorBase> add_editor_window(const s_ptr<EditorBase>& _new_widget);

	private:
		std::vector<s_ptr<EditorBase>> m_editor_UIs;

		std::unique_ptr<EditorMainMenu> m_editor_main_menu;

		std::vector<s_ptr<EditorObject>> mEditorObjects;
		std::vector<s_ptr<DebugObject>> mDebugObjects;

		//현재 GUI를 표시할지 여부
		bool mbEnable;

		//GUI가 최초 1회 초기화 되었는지 여부
		bool mbInitialized;

		bool m_IsOpenEditorAsDefault;

		std::unique_ptr<Json::Value> mJsonUIData;

	private:
		ImGuizmo::OPERATION mCurrentGizmoOperation;
		void RenderGuizmo();
	};

	template<typename T>
	inline s_ptr<T> EditorManager::new_editor_UI()
	{
		static_assert(std::is_base_of_v<EditorBase, T>);

		s_ptr<T> retPtr = new_entity<T>();

		if (nullptr == add_editor_window(std::static_pointer_cast<EditorBase>(retPtr)))
		{
			return nullptr;
		}

		return retPtr;
	}

}

