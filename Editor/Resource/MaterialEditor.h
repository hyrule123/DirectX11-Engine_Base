#pragma once
#include <Editor/Base/EditorUIWindow.h>
#include <Editor/Widget/ComboBoxWidget.h>

#include <Engine/Common.h>

//Material 전방선언
namespace core
{
	class Material;
}

namespace core::editor
{
	class EditorMaterial
		: public EditorUIWindow
	{
	public:
		EditorMaterial();
		virtual ~EditorMaterial();

		virtual void init() override;
		virtual void update() override;
		virtual void update_UI() override;

	private:
		void refresh_shader_setting_files();
		void update_shader();
		void update_texture_list();
		void update_rendering_mode();
		void update_material_const_buffer();
		void update_save_load();

		bool check_save_possible();
		void save_to_file();
		void load_from_file();

		void new_material();

		enum class eContext
		{
			None,
			load_from_file
		};
		

	private:
		s_ptr<Material>	mTargetMaterial;

		//기존의 재질을 수정중인지 신규 재질을 수정 중인지
		bool							mbNewMaterial;

		ComboBoxWidget		mShaderCombo;

		ComboBoxWidget		mCurrentMaterialsCombo;

		ComboBoxWidget		mRenderingModeCombo;

		//현재 어떤 창을 띄우는 상태인지
		eContext mCurContext;

		//저장할 파일명 저장용 변수
		std::string mSaveLoadFileName;

		//현재 엔진에 로드되어있는 재질 목록 표시용
		ComboBoxWidget		mCurrentLoadedMtrl;
	};
}


