#pragma once
#include <Editor/Base/EditorWindow.h>
#include <Editor/Widget/Widget_ComboBox.h>

#include <Engine/Common.h>

//Material 전방선언
namespace core
{
	class Material;
}

namespace core::editor
{
	class EditorMaterial
		: public EditorWindow
	{
	public:
		EditorMaterial();
		virtual ~EditorMaterial();

		virtual void init() override;
		virtual void update() override;
		virtual void update_UI() override;

	private:
		void RefreshShaderSettingFiles();
		void UpdateShader();
		void UpdateTextureList();
		void UpdateRenderingMode();
		void UpdateMtrlConstBuffer();
		void UpdateSaveLoad();

		bool CheckSavePossible();
		void SaveToFile();
		void LoadFromFile();

		void NewMaterial();

		enum class eContext
		{
			None,
			LoadFromFile
		};
		

	private:
		s_ptr<Material>	mTargetMaterial;

		//기존의 재질을 수정중인지 신규 재질을 수정 중인지
		bool							mbNewMaterial;

		EditorWidget_ComboBox		mShaderCombo;

		EditorWidget_ComboBox		mCurrentMaterialsCombo;

		EditorWidget_ComboBox		mRenderingModeCombo;

		//현재 어떤 창을 띄우는 상태인지
		eContext mCurContext;

		//저장할 파일명 저장용 변수
		std::string mSaveLoadFileName;

		//현재 엔진에 로드되어있는 재질 목록 표시용
		EditorWidget_ComboBox		mCurrentLoadedMtrl;
	};
}


