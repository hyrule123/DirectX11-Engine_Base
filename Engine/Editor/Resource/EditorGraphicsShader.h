#pragma once
#include "../../Editor/Base/EditorWindow.h"


#include "../../Editor/Widget/EditorWidget_ComboBox.h"
#include "../../GPU/CommonInclude_GPU.h"

namespace ehw
{
	class GraphicsShader;
}

namespace editor
{
	class EditorGraphicsShader
		: public EditorWindow
	{
	public:
		EditorGraphicsShader();
		virtual ~EditorGraphicsShader();

		virtual void Init() override;
		virtual void UpdateUI() override;

		bool CreateDefaultShaders();
		
	private:
		void LoadShaderSettingComboBox();

		void DXGISelectCallback(const EditorWidget_ComboBox::tComboItem& _item);

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


		EditorWidget_ComboBox mDXGIFormatCombo;

		EditorWidget_ComboBox mTopologyCombo;

		std::array<std::string, (int)ehw::eGSStage::END> mStageNames;

		EditorWidget_ComboBox mRSTypeCombo;
		EditorWidget_ComboBox mDSTypeCombo;
		EditorWidget_ComboBox mBSTypeCombo;

		//std::unique_ptr<ehw::GraphicsShader> mEditTarget;

		bool mbSaveModal;
		EditorWidget_ComboBox mStageTypeCombo;
		std::string mSaveFileName;

		bool mbLoadModal;
		EditorWidget_ComboBox mLoadFileCombo;
	};
}


