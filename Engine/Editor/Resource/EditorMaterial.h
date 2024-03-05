#pragma once
#include "Editor/Base/EditorWindow.h"
#include "Editor/Widget/EditorWidget_ComboBox.h"

#include "CommonStruct.h"

//Material 전방선언
namespace ehw
{
	class Material;
}

namespace editor
{
	class EditorMaterial
		: public EditorWindow
	{
	public:
		EditorMaterial();
		virtual ~EditorMaterial();

		virtual void Init() override;
		virtual void Update() override;
		virtual void UpdateUI() override;

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
		std::shared_ptr<ehw::Material>	mTargetMaterial;

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


