#pragma once
#include "Editor/Base/EditorWindow.h"

#include "Editor/Widget/Widget_ComboBox.h"

#include <Engine/GPU/CommonGPU.h>

namespace ehw
{
	class GraphicsShader;
}

namespace ehw::editor
{
	class EditorGraphicsShader
		: public EditorWindow
	{
	public:
		EditorGraphicsShader();
		virtual ~EditorGraphicsShader();

		virtual void init() override;
		virtual void update_UI() override;

		bool CreateDefaultShaders();
		
	private:
		void LoadShaderSettingComboBox();

		void DXGISelectCallback(const EditorWidget_ComboBox::tComboItem& _item);

		void InputElementEditModal();

		void CreateSTDInputLayout(const eDimensionType _dimType);

		void SaveModal();
		void LoadModal();

		void SaveToJson(const std::filesystem::path& _filePath);
		void LoadFromJson(const std::filesystem::path& _filePath);
		 
	private:
		int		mSemanticEditIdx;

		std::vector<D3D11_INPUT_ELEMENT_DESC> mInputLayoutDescs;

		std::string mSemanticName;
		D3D11_INPUT_ELEMENT_DESC mDescForEdit;


		EditorWidget_ComboBox mDXGIFormatCombo;

		//EditorWidget_ComboBox mTopologyCombo;

		std::array<std::string, (int)eGSStage::END> mStageNames;

		EditorWidget_ComboBox mRSTypeCombo;
		EditorWidget_ComboBox mDSTypeCombo;
		EditorWidget_ComboBox mBSTypeCombo;

		//std::unique_ptr<GraphicsShader> mEditTarget;

		bool mbSaveModal;
		EditorWidget_ComboBox mStageTypeCombo;
		std::string mSaveFileName;

		bool mbLoadModal;
		EditorWidget_ComboBox mLoadFileCombo;
	};
}


