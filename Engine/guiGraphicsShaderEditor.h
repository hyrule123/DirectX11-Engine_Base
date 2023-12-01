#pragma once

#include "guiWindow.h"
#include "define_GPU.h"
#include "guiComboBox.h"

namespace ehw
{
	class GraphicsShader;
}

namespace gui
{
	class guiGraphicsShaderEditor
		: public guiWindow
	{
	public:
		guiGraphicsShaderEditor();
		virtual ~guiGraphicsShaderEditor();

		virtual void Init() override;
		virtual void UpdateUI() override;

		bool CreateDefaultShaders();
		
	private:
		void LoadShaderSettingComboBox();

		void DXGISelectCallback(const guiComboBox::tComboItem& _item);

		void InputElementEditModal();

		void CreateSTDInputLayout(const ehw::eDimensionType _dimType);

		void SaveModal();
		void LoadModal();

		void SaveToJson(const std::filesystem::path& _filePath);
		void LoadFromJson(const std::filesystem::path& _filePath);
		 
	private:
		int		mSemanticEditIdx;

		std::vector<D3D11_INPUT_ELEMENT_DESC> mInputLayoutDescs;

		std::string mSemanticName;
		D3D11_INPUT_ELEMENT_DESC mDescForEdit;


		guiComboBox mDXGIFormatCombo;

		guiComboBox mTopologyCombo;

		std::array<std::string, (int)ehw::eGSStage::END> mStageNames;

		guiComboBox mRSTypeCombo;
		guiComboBox mDSTypeCombo;
		guiComboBox mBSTypeCombo;

		//std::unique_ptr<ehw::GraphicsShader> mEditTarget;

		bool mbSaveModal;
		guiComboBox mStageTypeCombo;
		std::string mSaveFileName;

		bool mbLoadModal;
		guiComboBox mLoadFileCombo;
	};
}


