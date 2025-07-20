#pragma once
#include <Editor/Base/EditorUIWindow.h>

#include <Editor/Widget/ComboBoxWidget.h>

#include <thread>
#include <future>

namespace core
{
	class Model3D;
}


namespace core::editor
{
	class EditorFBXConverter :
		public EditorUIWindow
	{
	public:
		EditorFBXConverter();
		virtual ~EditorFBXConverter();

		virtual void init() override;
		virtual void update_UI() override;

	private:
		bool check_thread();
		void select_FBX_button();
		void convert_FBX_button();
		void add_animation_from_same_skeleton();

		void load_meshdata_combobox();

	private:
		std::string		m_fbx_path;
		std::string		m_output_dir;
		bool			m_b_static;

		std::future<eResult> m_future_convert_result;

		ComboBoxWidget		m_skeletal_mesh_combobox;
	};
}


