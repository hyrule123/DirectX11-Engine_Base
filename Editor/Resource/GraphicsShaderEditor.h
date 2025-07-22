#pragma once
#include <Editor/Base/EditorUIWindow.h>

#include <Editor/Widget/ComboBoxWidget.h>

#include <Engine/GPU/CommonGPU.h>

namespace core
{
	class GraphicsShader;
}

namespace core::editor
{
	class EditorGraphicsShader
		: public EditorUIWindow
	{
		CLASS_INFO(EditorGraphicsShader, EditorUIWindow);
	public:
		EditorGraphicsShader();
		virtual ~EditorGraphicsShader();

		virtual void init() override;
		virtual void update_UI() override;

		bool create_default_shaders();
		
	private:
		void load_shader_setting_combobox();

		void DXGI_select_callback(const tComboItem& _item);

		void input_element_edit_modal();

		void create_standard_input_layout(const eDimensionType _dimType);

		void save_modal();
		void load_modal();

		void save_to_json(const std::filesystem::path& _filePath);
		void load_from_json(const std::filesystem::path& _filePath);
		 
	private:
		int		m_semantic_edit_idx;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_input_layout_descs;

		std::string m_semantic_name;
		D3D11_INPUT_ELEMENT_DESC m_desc_for_edit;


		ComboBoxWidget m_DXGI_format_combo;

		//ComboBoxWidget mTopologyCombo;

		std::array<std::string, (int)eGSStage::END> m_stage_names;

		ComboBoxWidget m_rasterizer_type_combo;
		ComboBoxWidget m_depth_stencil_type_combo;
		ComboBoxWidget m_blend_state_type_combo;

		//std::unique_ptr<GraphicsShader> mEditTarget;

		bool m_b_save_modal;
		ComboBoxWidget m_stage_type_combo;
		std::string m_save_file_name;

		bool m_b_load_modal;
		ComboBoxWidget m_load_file_combo;
	};
}


