#include <Engine/Manager/ResourceManager.h>

#include "Engine/Resource/Shader/GraphicsPipeline.h"

#include "Engine/Resource/Shader/InputLayout.h"
#include "Engine/Resource/Shader/VertexShader.h"
#include "Engine/Resource/Shader/PixelShader.h"

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

		s_ptr<InputLayout> std_2d_layout = {};

#pragma region SPRITE SHADER
		{
			s_ptr<GraphicsPipeline> sprite_pipeline = new_entity<GraphicsPipeline>();
			
			s_ptr<VertexShader> vs = load_default_resource<VertexShader>("Shader/Engine/Sprite_VS.cso");
			ASSERT(vs);

			std_2d_layout = new_entity<InputLayout>();
			bool result = std_2d_layout->create_input_layout(vecLayoutDesc2D, vs);
			ASSERT(result);
			insert_default_resource(std_2d_layout, "std_2d_input_layout");

			s_ptr<PixelShader> ps = load_default_resource<PixelShader>("Shader/Engine/Sprite_PS.cso");
			ASSERT(ps);

			sprite_pipeline->set_input_layout(std_2d_layout);
			sprite_pipeline->set_vertex_shader(vs);
			sprite_pipeline->set_pixel_shader(ps);

			insert_default_resource(sprite_pipeline, "sprite_pipeline");
		}

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

			s_ptr<VertexShader> vs = load_default_resource<VertexShader>("Shader/Engine/Debug_VS.cso");
			ASSERT(vs);

			s_ptr<PixelShader> ps = load_default_resource<PixelShader>("Shader/Engine/Debug_CS.cso");
			ASSERT(ps);

			s_ptr<InputLayout> input_layout = new_entity<InputLayout>();
			bool result = input_layout->create_input_layout(debugLayoutDesc, vs);
			ASSERT(result);

			

			//debug_shader->CreateBuffer(eShaderStage::Vertex, "DebugVS.hlsl", "main");
			//debug_shader->CreateBuffer(eShaderStage::Pixel, "DebugPS.hlsl", "main");
			debug_shader->set_rasterizer_state(eRasterizerState::WireframeNone);
			debug_shader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			debug_shader->set_blend_state(eBlendState::Default);

			s_ptr<GraphicsPipeline> debug_pass = new_entity<GraphicsPipeline>();
			ResourceManager::get_inst().insert_default_resource(debug_shader, "debug_shader");
		}

#pragma endregion



#pragma region POST PROCESS SHADER
		{
			s_ptr<GraphicsPipeline> post_process_shader = std::make_shared<GraphicsPipeline>();
			post_process_shader->compile_from_byte_code(eGSStage::Vertex, PostProcess_VS, sizeof(PostProcess_VS));
			post_process_shader->compile_from_byte_code(eGSStage::Pixel, PostProcess_PS, sizeof(PostProcess_PS));
			post_process_shader->set_input_layout_desc(vecLayoutDesc2D);
			post_process_shader->create_input_layout();

			post_process_shader->set_depth_stencil_state(eDepthStencilState::NoWrite);

			ResourceManager::get_inst().insert_default_resource(post_process_shader, "post_process_shader");
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
			s_ptr<GraphicsPipeline> basic_3D_shader = std::make_shared<GraphicsPipeline>();

			basic_3D_shader->compile_from_byte_code(eGSStage::Vertex, Forward3D_VS, sizeof(Forward3D_VS));
			basic_3D_shader->compile_from_byte_code(eGSStage::Pixel, Forward3D_PS, sizeof(Forward3D_PS));
			basic_3D_shader->set_input_layout_desc(vecLayoutDesc3D);
			basic_3D_shader->create_input_layout();

			ResourceManager::get_inst().insert_default_resource(basic_3D_shader, "forward_3D_shader");
		}
#pragma endregion

#pragma region Deffered3D
		{
			s_ptr<GraphicsPipeline> deffered_shader = std::make_shared<GraphicsPipeline>();

			deffered_shader->compile_from_byte_code(eGSStage::Vertex, Deffered3D_VS, sizeof(Deffered3D_VS));
			deffered_shader->compile_from_byte_code(eGSStage::Pixel, Deffered3D_PS, sizeof(Deffered3D_PS));

			deffered_shader->set_input_layout_desc(vecLayoutDesc3D);
			deffered_shader->create_input_layout();

			ResourceManager::get_inst().insert_default_resource(deffered_shader, "deffered_3D_shader");
		}
#pragma endregion


#pragma region LIGHT
		{
			s_ptr<GraphicsPipeline> light_shader = std::make_shared<GraphicsPipeline>();
			light_shader->compile_from_byte_code(eGSStage::Vertex, LightDirection_VS, sizeof(LightDirection_VS));
			light_shader->compile_from_byte_code(eGSStage::Pixel, LightDirection_PS, sizeof(LightDirection_PS));

			light_shader->set_input_layout_desc(vecLayoutDesc2D);
			light_shader->create_input_layout();

			light_shader->set_rasterizer_state(eRasterizerState::SolidBack);
			light_shader->set_depth_stencil_state(eDepthStencilState::None);
			light_shader->set_blend_state(eBlendState::OneOne);

			ResourceManager::get_inst().insert_default_resource(light_shader, "directional_light_shader");
		}

		{
			s_ptr<GraphicsPipeline> light_shader = std::make_shared<GraphicsPipeline>();
			light_shader->compile_from_byte_code(eGSStage::Vertex, LightPoint_VS, sizeof(LightPoint_VS));
			light_shader->compile_from_byte_code(eGSStage::Pixel, LightPoint_PS, sizeof(LightPoint_PS));

			light_shader->set_input_layout_desc(vecLayoutDesc3D);
			light_shader->create_input_layout();

			light_shader->set_rasterizer_state(eRasterizerState::SolidFront);
			light_shader->set_depth_stencil_state(eDepthStencilState::None);
			light_shader->set_blend_state(eBlendState::OneOne);

			ResourceManager::get_inst().insert_default_resource(light_shader, "point_light_shader");
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

			s_ptr<GraphicsPipeline> merge_shader = std::make_shared<GraphicsPipeline>();
			merge_shader->compile_from_byte_code(eGSStage::Vertex, Deffered_Merger_VS, sizeof(Deffered_Merger_VS));
			merge_shader->compile_from_byte_code(eGSStage::Pixel, Deffered_Merger_PS, sizeof(Deffered_Merger_PS));

			merge_shader->add_input_layout_desc(layout_desc);
			merge_shader->create_input_layout();

			merge_shader->set_rasterizer_state(eRasterizerState::SolidBack);
			merge_shader->set_depth_stencil_state(eDepthStencilState::None);
			merge_shader->set_blend_state(eBlendState::Default);

			ResourceManager::get_inst().insert_default_resource(merge_shader, "merge_shader");
		}
#pragma endregion


#pragma region ANIMATION 3D
		s_ptr<Animation3D_ComputeShader> anim_3D_shader = std::make_shared<Animation3D_ComputeShader>();
		anim_3D_shader->compile_from_byte_code(Animation3D_CS, sizeof(Animation3D_CS));

		ResourceManager::get_inst().insert_default_resource(anim_3D_shader, "animation_3D");
#pragma endregion

#pragma region PARTICLE SHADER
		{
			s_ptr<GraphicsPipeline> particle_shader = std::make_shared<GraphicsPipeline>();

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
			ResourceManager::get_inst().insert_default_resource(particle_shader, "particle_shader");

			s_ptr<ParticleShader> particle_CS = std::make_shared<ParticleShader>();
			ResourceManager::get_inst().insert_default_resource(particle_CS, "particle_CS");
			particle_CS->compile_from_byte_code(Particle_CS, sizeof(Particle_CS));
		}
#pragma endregion
	}

}
