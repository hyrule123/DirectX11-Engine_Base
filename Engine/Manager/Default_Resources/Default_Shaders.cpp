#include <Engine/Manager/ResourceManager.h>

#include <Engine/CompiledShaderHeader/DefaultShaders.h>

#include <Engine/Resource/Shader/GraphicsShader.h>
#include <Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h>
#include <Engine/Resource/Shader/ComputeShaders/GPUInitSetting.h>
#include <Engine/Resource/Shader/ComputeShaders/NormalConvertShader.h>
#include <Engine/Resource/Shader/ComputeShaders/ParticleShader.h>

namespace core {
	void ResourceManager::load_default_shaders()
	{
#pragma region 2D LAYOUT
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc2D;
		D3D11_INPUT_ELEMENT_DESC layout_desc{};

		layout_desc.AlignedByteOffset = 0;
		layout_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "POSITION";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc2D.push_back(layout_desc);
		layout_desc = D3D11_INPUT_ELEMENT_DESC{};

		layout_desc.AlignedByteOffset = 16;
		layout_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "TEXCOORD";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc2D.push_back(layout_desc);
		layout_desc = D3D11_INPUT_ELEMENT_DESC{};
#pragma endregion


#pragma region SPRITE SHADER
		{
			s_ptr<GraphicsShader> sprite_shader = std::make_shared<GraphicsShader>();
			sprite_shader->set_engine_default_res(true);
			sprite_shader->compile_from_byte_code(eGSStage::Vertex, Sprite_VS, sizeof(Sprite_VS));
			sprite_shader->compile_from_byte_code(eGSStage::Pixel, Sprite_PS, sizeof(Sprite_PS));
			sprite_shader->set_rasterizer_state(eRasterizerState::SolidNone);
			sprite_shader->set_input_layout_desc(vecLayoutDesc2D);
			sprite_shader->create_input_layout();

			sprite_shader->set_resource_path(name::defaultRes::shader::graphics::SpriteShader);
			ResourceManager::get_inst().insert(sprite_shader);
		}

#pragma endregion
#pragma region UI SHADER
		{
			s_ptr<GraphicsShader> ui_shader = std::make_shared<GraphicsShader>();
			ui_shader->set_engine_default_res(true);
			ui_shader->compile_from_byte_code(eGSStage::Vertex, UI_VS, sizeof(UI_VS));
			ui_shader->compile_from_byte_code(eGSStage::Pixel, UI_PS, sizeof(UI_PS));
			ui_shader->set_input_layout_desc(vecLayoutDesc2D);
			ui_shader->create_input_layout();

			ui_shader->set_resource_path(name::defaultRes::shader::graphics::UIShader);
			ResourceManager::get_inst().insert(ui_shader);
		}

#pragma endregion
#pragma region GRID SHADER
		{
			s_ptr<GraphicsShader> grid_shader = std::make_shared<GraphicsShader>();
			grid_shader->set_engine_default_res(true);
			grid_shader->compile_from_byte_code(eGSStage::Vertex, Grid_VS, sizeof(Grid_VS));
			grid_shader->compile_from_byte_code(eGSStage::Pixel, Grid_PS, sizeof(Grid_PS));
			grid_shader->set_input_layout_desc(vecLayoutDesc2D);
			grid_shader->create_input_layout();

			grid_shader->set_rasterizer_state(eRasterizerState::SolidNone);
			grid_shader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			grid_shader->set_blend_state(eBlendState::AlphaBlend);

			grid_shader->set_resource_path(name::defaultRes::shader::graphics::GridShader);
			ResourceManager::get_inst().insert(grid_shader);
		}

#pragma endregion
#pragma region DEBUG SHADER
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> debugLayoutDesc;
			D3D11_INPUT_ELEMENT_DESC layout_desc{};

			layout_desc.AlignedByteOffset = 0;
			layout_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			layout_desc.InputSlot = 0;
			layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			layout_desc.SemanticName = "POSITION";
			layout_desc.SemanticIndex = 0;
			debugLayoutDesc.push_back(layout_desc);
			layout_desc = D3D11_INPUT_ELEMENT_DESC{};

			s_ptr<GraphicsShader> debug_shader = std::make_shared<GraphicsShader>();
			debug_shader->set_engine_default_res(true);
			debug_shader->compile_from_byte_code(eGSStage::Vertex, Debug_VS, sizeof(Debug_VS));
			debug_shader->compile_from_byte_code(eGSStage::Pixel, Debug_PS, sizeof(Debug_PS));

			debug_shader->set_input_layout_desc(debugLayoutDesc);
			debug_shader->create_input_layout();

			//debug_shader->CreateBuffer(eShaderStage::Vertex, "DebugVS.hlsl", "main");
			//debug_shader->CreateBuffer(eShaderStage::Pixel, "DebugPS.hlsl", "main");
			debug_shader->set_rasterizer_state(eRasterizerState::WireframeNone);
			debug_shader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			debug_shader->set_blend_state(eBlendState::Default);

			debug_shader->set_resource_path(name::defaultRes::shader::graphics::DebugShader);
			ResourceManager::get_inst().insert(debug_shader);
		}

#pragma endregion



#pragma region POST PROCESS SHADER
		{
			s_ptr<GraphicsShader> post_process_shader = std::make_shared<GraphicsShader>();
			post_process_shader->set_engine_default_res(true);
			post_process_shader->compile_from_byte_code(eGSStage::Vertex, PostProcess_VS, sizeof(PostProcess_VS));
			post_process_shader->compile_from_byte_code(eGSStage::Pixel, PostProcess_PS, sizeof(PostProcess_PS));
			post_process_shader->set_input_layout_desc(vecLayoutDesc2D);
			post_process_shader->create_input_layout();

			post_process_shader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			post_process_shader->set_resource_path(name::defaultRes::shader::graphics::PostProcessShader);

			ResourceManager::get_inst().insert(post_process_shader);
		}
#pragma endregion

#pragma region 3D 기본 입력 레이아웃
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc3D;

		layout_desc = {};
		layout_desc.AlignedByteOffset = 0;
		layout_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "POSITION";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(layout_desc);

		layout_desc = {};
		layout_desc.AlignedByteOffset = 16;
		layout_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "TEXCOORD";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(layout_desc);

		layout_desc = {};
		layout_desc.AlignedByteOffset = 24;
		layout_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "TANGENT";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(layout_desc);

		layout_desc = {};
		layout_desc.AlignedByteOffset = 36;
		layout_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "BINORMAL";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(layout_desc);

		layout_desc = {};
		layout_desc.AlignedByteOffset = 48;
		layout_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "NORMAL";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(layout_desc);

		layout_desc = {};
		layout_desc.AlignedByteOffset = 60;
		layout_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "BLENDINDICES";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(layout_desc);

		layout_desc = {};
		layout_desc.AlignedByteOffset = 76;
		layout_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		layout_desc.InputSlot = 0;
		layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout_desc.SemanticName = "BLENDWEIGHT";
		layout_desc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(layout_desc);


#pragma endregion

#pragma region Forward3D
		{
			s_ptr<GraphicsShader> basic_3D_shader = std::make_shared<GraphicsShader>();
			basic_3D_shader->set_engine_default_res(true);

			basic_3D_shader->compile_from_byte_code(eGSStage::Vertex, Forward3D_VS, sizeof(Forward3D_VS));
			basic_3D_shader->compile_from_byte_code(eGSStage::Pixel, Forward3D_PS, sizeof(Forward3D_PS));
			basic_3D_shader->set_input_layout_desc(vecLayoutDesc3D);
			basic_3D_shader->create_input_layout();

			basic_3D_shader->set_resource_path(name::defaultRes::shader::graphics::Forward3DShader);
			ResourceManager::get_inst().insert(basic_3D_shader);
		}
#pragma endregion

#pragma region Deffered3D
		{
			s_ptr<GraphicsShader> deffered_shader = std::make_shared<GraphicsShader>();
			deffered_shader->set_engine_default_res(true);

			deffered_shader->compile_from_byte_code(eGSStage::Vertex, Deffered3D_VS, sizeof(Deffered3D_VS));
			deffered_shader->compile_from_byte_code(eGSStage::Pixel, Deffered3D_PS, sizeof(Deffered3D_PS));

			deffered_shader->set_input_layout_desc(vecLayoutDesc3D);
			deffered_shader->create_input_layout();

			deffered_shader->set_resource_path(name::defaultRes::shader::graphics::Deffered3DShader);
			ResourceManager::get_inst().insert(deffered_shader);
		}
#pragma endregion


#pragma region LIGHT
		{
			s_ptr<GraphicsShader> light_shader = std::make_shared<GraphicsShader>();
			light_shader->compile_from_byte_code(eGSStage::Vertex, LightDirection_VS, sizeof(LightDirection_VS));
			light_shader->compile_from_byte_code(eGSStage::Pixel, LightDirection_PS, sizeof(LightDirection_PS));

			light_shader->set_input_layout_desc(vecLayoutDesc2D);
			light_shader->create_input_layout();

			light_shader->set_rasterizer_state(eRasterizerState::SolidBack);
			light_shader->set_depth_stencil_state(eDepthStencilState::None);
			light_shader->set_blend_state(eBlendState::OneOne);

			light_shader->set_resource_path(name::defaultRes::shader::graphics::LightDirShader);
			ResourceManager::get_inst().insert(light_shader);
		}

		{
			s_ptr<GraphicsShader> light_shader = std::make_shared<GraphicsShader>();
			light_shader->compile_from_byte_code(eGSStage::Vertex, LightPoint_VS, sizeof(LightPoint_VS));
			light_shader->compile_from_byte_code(eGSStage::Pixel, LightPoint_PS, sizeof(LightPoint_PS));

			light_shader->set_input_layout_desc(vecLayoutDesc3D);
			light_shader->create_input_layout();

			light_shader->set_rasterizer_state(eRasterizerState::SolidFront);
			light_shader->set_depth_stencil_state(eDepthStencilState::None);
			light_shader->set_blend_state(eBlendState::OneOne);

			light_shader->set_resource_path(name::defaultRes::shader::graphics::LightPointShader);
			ResourceManager::get_inst().insert(light_shader);
		}
#pragma endregion

#pragma region MERGE
		{
			layout_desc = D3D11_INPUT_ELEMENT_DESC{};
			layout_desc.AlignedByteOffset = 0;
			layout_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			layout_desc.InputSlot = 0;
			layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			layout_desc.SemanticName = "POSITION";
			layout_desc.SemanticIndex = 0;

			s_ptr<GraphicsShader> merge_shader = std::make_shared<GraphicsShader>();
			merge_shader->set_engine_default_res(true);
			merge_shader->compile_from_byte_code(eGSStage::Vertex, Deffered_Merger_VS, sizeof(Deffered_Merger_VS));
			merge_shader->compile_from_byte_code(eGSStage::Pixel, Deffered_Merger_PS, sizeof(Deffered_Merger_PS));

			merge_shader->add_input_layout_desc(layout_desc);
			merge_shader->create_input_layout();

			merge_shader->set_rasterizer_state(eRasterizerState::SolidBack);
			merge_shader->set_depth_stencil_state(eDepthStencilState::None);
			merge_shader->set_blend_state(eBlendState::Default);

			merge_shader->set_resource_path(name::defaultRes::shader::graphics::MergeShader);
			ResourceManager::get_inst().insert(merge_shader);
		}
#pragma endregion


#pragma region ANIMATION 3D
		s_ptr<Animation3D_ComputeShader> anim_3D_shader = std::make_shared<Animation3D_ComputeShader>();
		anim_3D_shader->set_engine_default_res(true);
		anim_3D_shader->compile_from_byte_code(Animation3D_CS, sizeof(Animation3D_CS));

		anim_3D_shader->set_resource_path(name::defaultRes::shader::compute::Animation3D);
		ResourceManager::get_inst().insert(anim_3D_shader);
#pragma endregion

#pragma region PARTICLE SHADER
		{
			s_ptr<GraphicsShader> particle_shader = std::make_shared<GraphicsShader>();
			particle_shader->set_engine_default_res(true);

			particle_shader->compile_from_byte_code(eGSStage::Vertex, Particle_VS, sizeof(Particle_VS));
			particle_shader->compile_from_byte_code(eGSStage::Geometry, Particle_GS, sizeof(Particle_GS));
			particle_shader->compile_from_byte_code(eGSStage::Pixel, Particle_PS, sizeof(Particle_PS));
			particle_shader->set_input_layout_desc(vecLayoutDesc2D);
			particle_shader->create_input_layout();


			particle_shader->set_rasterizer_state(eRasterizerState::SolidNone);
			particle_shader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			particle_shader->set_blend_state(eBlendState::AlphaBlend);

			//TODO: 파티클을 위한 토폴로지 설정 해줘야함
			//particle_shader->set_topology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			particle_shader->set_resource_path(name::defaultRes::shader::graphics::ParticleShader);
			ResourceManager::get_inst().insert(particle_shader);

			s_ptr<ParticleShader> particle_CS = std::make_shared<ParticleShader>();
			particle_CS->set_engine_default_res(true);
			particle_CS->set_resource_path(name::defaultRes::shader::compute::ParticleCS);
			ResourceManager::get_inst().insert(particle_CS);
			particle_CS->compile_from_byte_code(Particle_CS, sizeof(Particle_CS));
		}
#pragma endregion
	}

}
