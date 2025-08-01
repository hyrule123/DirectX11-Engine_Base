#pragma once
#include <Engine/Common.h>
#include <Engine/define_Macro.h>

#include <Engine/GPU/CommonGPU.h>

#include <Editor/imgui/ImGuizmo.h>

namespace core::editor
{
	class EditorObject;
	class DebugObject;
	class EditorUIBase;
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
		inline bool get_enable() { return m_b_enable; }
		inline void toggle_enable() { set_enable(!m_b_enable); }
		void open_editor_as_default(bool _enable) { m_b_open_editor_as_default = _enable; }

		s_ptr<EditorUIBase> find_editor_window(const std::string_view _UI_name);

		template <typename T>
		s_ptr<T> new_editor_UI();

		const std::vector<s_ptr<EditorUIBase>>&
			get_all_editor_UIs() { return m_editor_UIs; }

		s_ptr<EditorMainMenu> get_editor_main_menu() const { return m_editor_main_menu; }

	private:
		void init();
		void awake();

		void run();

		void update();
		void final_update();
		void render();

		//Window 이름으로 저장된 Json 값이 있을 경우 로드함
		Json::Value* check_json_saved(const std::string& _key_path);

		void create_default_UIs();

		//=================== IMGUI ===========================
		void imgui_initialize();
		void imgui_release();

		void imgui_new_frame();
		void imgui_render();

	private:
		bool add_editor_UI(const s_ptr<EditorUIBase>& _new_widget);

		std::vector<s_ptr<EditorUIBase>> m_editor_UIs;

		std::shared_ptr<EditorMainMenu> m_editor_main_menu;

		//현재 GUI를 표시할지 여부
		bool m_b_enable;

		//GUI가 최초 1회 초기화 되었는지 여부
		bool m_b_awake;

		bool m_b_open_editor_as_default;

		std::unique_ptr<Json::Value> m_json_UI_data;

	private:
		ImGuizmo::OPERATION m_current_gizmo_operation;
		void RenderGuizmo();
	};

	template<typename T>
	inline s_ptr<T> EditorManager::new_editor_UI()
	{
		static_assert(std::is_base_of_v<EditorUIBase, T>);

		s_ptr<T> new_ui = new_entity<T>();
		if (false == add_editor_UI(new_ui))
		{
			return nullptr;
		}

		return new_ui;
	}

}

