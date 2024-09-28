#include "Engine/Manager/Resourcemanagers.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/CompiledShaderHeader/DefaultShaders.h"

#include "Engine/Resource/Shader/GraphicsShader.h"
#include "Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h"
#include "Engine/Resource/Shader/ComputeShaders/GPUInitSetting.h"
#include "Engine/Resource/Shader/ComputeShaders/NormalConvertShader.h"
#include "Engine/Resource/Shader/ComputeShaders/ParticleShader.h"

namespace ehw {
	void ResourceManagers::load_default_shaders()
	{
#pragma region 2D LAYOUT
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc2D;
		D3D11_INPUT_ELEMENT_DESC LayoutDesc{};

		LayoutDesc.AlignedByteOffset = 0;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "POSITION";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc2D.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		LayoutDesc.AlignedByteOffset = 16;
		LayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TEXCOORD";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc2D.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
#pragma endregion


#pragma region SPRITE SHADER
		{
			std::shared_ptr<GraphicsShader> spriteShader = std::make_shared<GraphicsShader>();
			spriteShader->set_engine_default_res(true);
			spriteShader->compile_from_byte_code(eGSStage::Vertex, Sprite_VS, sizeof(Sprite_VS));
			spriteShader->compile_from_byte_code(eGSStage::Pixel, Sprite_PS, sizeof(Sprite_PS));
			spriteShader->set_rasterizer_state(eRasterizerState::SolidNone);
			spriteShader->SetInputLayoutDesc(vecLayoutDesc2D);
			spriteShader->CreateInputLayout();


			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::SpriteShader, spriteShader);
		}

#pragma endregion
#pragma region UI SHADER
		{
			std::shared_ptr<GraphicsShader> uiShader = std::make_shared<GraphicsShader>();
			uiShader->set_engine_default_res(true);
			uiShader->compile_from_byte_code(eGSStage::Vertex, UI_VS, sizeof(UI_VS));
			uiShader->compile_from_byte_code(eGSStage::Pixel, UI_PS, sizeof(UI_PS));
			uiShader->SetInputLayoutDesc(vecLayoutDesc2D);
			uiShader->CreateInputLayout();


			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::UIShader, uiShader);
		}

#pragma endregion
#pragma region GRID SHADER
		{
			std::shared_ptr<GraphicsShader> gridShader = std::make_shared<GraphicsShader>();
			gridShader->set_engine_default_res(true);
			gridShader->compile_from_byte_code(eGSStage::Vertex, Grid_VS, sizeof(Grid_VS));
			gridShader->compile_from_byte_code(eGSStage::Pixel, Grid_PS, sizeof(Grid_PS));
			gridShader->SetInputLayoutDesc(vecLayoutDesc2D);
			gridShader->CreateInputLayout();

			gridShader->set_rasterizer_state(eRasterizerState::SolidNone);
			gridShader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			gridShader->set_blend_state(eBlendState::AlphaBlend);

			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::GridShader, gridShader);
		}

#pragma endregion
#pragma region DEBUG SHADER
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> debugLayoutDesc;
			D3D11_INPUT_ELEMENT_DESC LayoutDesc{};

			LayoutDesc.AlignedByteOffset = 0;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "POSITION";
			LayoutDesc.SemanticIndex = 0;
			debugLayoutDesc.push_back(LayoutDesc);
			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

			std::shared_ptr<GraphicsShader> debugShader = std::make_shared<GraphicsShader>();
			debugShader->set_engine_default_res(true);
			debugShader->compile_from_byte_code(eGSStage::Vertex, Debug_VS, sizeof(Debug_VS));
			debugShader->compile_from_byte_code(eGSStage::Pixel, Debug_PS, sizeof(Debug_PS));

			debugShader->SetInputLayoutDesc(debugLayoutDesc);
			debugShader->CreateInputLayout();

			//debugShader->CreateBuffer(eShaderStage::Vertex, "DebugVS.hlsl", "main");
			//debugShader->CreateBuffer(eShaderStage::Pixel, "DebugPS.hlsl", "main");
			debugShader->set_rasterizer_state(eRasterizerState::WireframeNone);
			debugShader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			debugShader->set_blend_state(eBlendState::Default);

			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::DebugShader, debugShader);
		}

#pragma endregion



#pragma region POST PROCESS SHADER
		{
			std::shared_ptr<GraphicsShader> postProcessShader = std::make_shared<GraphicsShader>();
			postProcessShader->set_engine_default_res(true);
			postProcessShader->compile_from_byte_code(eGSStage::Vertex, PostProcess_VS, sizeof(PostProcess_VS));
			postProcessShader->compile_from_byte_code(eGSStage::Pixel, PostProcess_PS, sizeof(PostProcess_PS));
			postProcessShader->SetInputLayoutDesc(vecLayoutDesc2D);
			postProcessShader->CreateInputLayout();

			postProcessShader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::PostProcessShader, postProcessShader);
		}
#pragma endregion

#pragma region 3D 기본 입력 레이아웃
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc3D;

		LayoutDesc = {};
		LayoutDesc.AlignedByteOffset = 0;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "POSITION";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = {};
		LayoutDesc.AlignedByteOffset = 16;
		LayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TEXCOORD";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = {};
		LayoutDesc.AlignedByteOffset = 24;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TANGENT";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = {};
		LayoutDesc.AlignedByteOffset = 36;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BINORMAL";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = {};
		LayoutDesc.AlignedByteOffset = 48;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "NORMAL";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = {};
		LayoutDesc.AlignedByteOffset = 60;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BLENDINDICES";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = {};
		LayoutDesc.AlignedByteOffset = 76;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BLENDWEIGHT";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);


#pragma endregion

#pragma region Forward3D
		{
			std::shared_ptr<GraphicsShader> basic3DShader = std::make_shared<GraphicsShader>();
			basic3DShader->set_engine_default_res(true);

			basic3DShader->compile_from_byte_code(eGSStage::Vertex, Forward3D_VS, sizeof(Forward3D_VS));
			basic3DShader->compile_from_byte_code(eGSStage::Pixel, Forward3D_PS, sizeof(Forward3D_PS));
			basic3DShader->SetInputLayoutDesc(vecLayoutDesc3D);
			basic3DShader->CreateInputLayout();

			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::Forward3DShader, basic3DShader);
		}
#pragma endregion

#pragma region Deffered3D
		{
			std::shared_ptr<GraphicsShader> defferedShader = std::make_shared<GraphicsShader>();
			defferedShader->set_engine_default_res(true);

			defferedShader->compile_from_byte_code(eGSStage::Vertex, Deffered3D_VS, sizeof(Deffered3D_VS));
			defferedShader->compile_from_byte_code(eGSStage::Pixel, Deffered3D_PS, sizeof(Deffered3D_PS));

			defferedShader->SetInputLayoutDesc(vecLayoutDesc3D);
			defferedShader->CreateInputLayout();

			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::Deffered3DShader, defferedShader);
		}
#pragma endregion


#pragma region LIGHT
		{
			std::shared_ptr<GraphicsShader> lightShader = std::make_shared<GraphicsShader>();
			lightShader->compile_from_byte_code(eGSStage::Vertex, LightDirection_VS, sizeof(LightDirection_VS));
			lightShader->compile_from_byte_code(eGSStage::Pixel, LightDirection_PS, sizeof(LightDirection_PS));

			lightShader->SetInputLayoutDesc(vecLayoutDesc2D);
			lightShader->CreateInputLayout();

			lightShader->set_rasterizer_state(eRasterizerState::SolidBack);
			lightShader->set_depth_stencil_state(eDepthStencilState::None);
			lightShader->set_blend_state(eBlendState::OneOne);

			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::LightDirShader, lightShader);
		}

		{
			std::shared_ptr<GraphicsShader> lightShader = std::make_shared<GraphicsShader>();
			lightShader->compile_from_byte_code(eGSStage::Vertex, LightPoint_VS, sizeof(LightPoint_VS));
			lightShader->compile_from_byte_code(eGSStage::Pixel, LightPoint_PS, sizeof(LightPoint_PS));

			lightShader->SetInputLayoutDesc(vecLayoutDesc3D);
			lightShader->CreateInputLayout();

			lightShader->set_rasterizer_state(eRasterizerState::SolidFront);
			lightShader->set_depth_stencil_state(eDepthStencilState::None);
			lightShader->set_blend_state(eBlendState::OneOne);

			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::LightPointShader, lightShader);
		}
#pragma endregion

#pragma region MERGE
		{
			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
			LayoutDesc.AlignedByteOffset = 0;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "POSITION";
			LayoutDesc.SemanticIndex = 0;

			std::shared_ptr<GraphicsShader> MergeShader = std::make_shared<GraphicsShader>();
			MergeShader->set_engine_default_res(true);
			MergeShader->compile_from_byte_code(eGSStage::Vertex, Deffered_Merger_VS, sizeof(Deffered_Merger_VS));
			MergeShader->compile_from_byte_code(eGSStage::Pixel, Deffered_Merger_PS, sizeof(Deffered_Merger_PS));

			MergeShader->AddInputLayoutDesc(LayoutDesc);
			MergeShader->CreateInputLayout();

			MergeShader->set_rasterizer_state(eRasterizerState::SolidBack);
			MergeShader->set_depth_stencil_state(eDepthStencilState::None);
			MergeShader->set_blend_state(eBlendState::Default);

			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::MergeShader, MergeShader);
		}
#pragma endregion


#pragma region ANIMATION 3D
		std::shared_ptr<Animation3D_ComputeShader> Anim3DShader = std::make_shared<Animation3D_ComputeShader>();
		Anim3DShader->set_engine_default_res(true);
		Anim3DShader->compile_from_byte_code(Animation3D_CS, sizeof(Animation3D_CS));

		ResourceManager<ComputeShader>::get_inst().insert(name::defaultRes::shader::compute::Animation3D, Anim3DShader);
#pragma endregion

#pragma region PARTICLE SHADER
		{
			std::shared_ptr<GraphicsShader> particleShader = std::make_shared<GraphicsShader>();
			particleShader->set_engine_default_res(true);

			particleShader->compile_from_byte_code(eGSStage::Vertex, Particle_VS, sizeof(Particle_VS));
			particleShader->compile_from_byte_code(eGSStage::Geometry, Particle_GS, sizeof(Particle_GS));
			particleShader->compile_from_byte_code(eGSStage::Pixel, Particle_PS, sizeof(Particle_PS));
			particleShader->SetInputLayoutDesc(vecLayoutDesc2D);
			particleShader->CreateInputLayout();


			particleShader->set_rasterizer_state(eRasterizerState::SolidNone);
			particleShader->set_depth_stencil_state(eDepthStencilState::NoWrite);
			particleShader->set_blend_state(eBlendState::AlphaBlend);

			//TODO: 파티클을 위한 토폴로지 설정 해줘야함
			//particleShader->set_topology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			ResourceManager<GraphicsShader>::get_inst().insert(name::defaultRes::shader::graphics::ParticleShader, particleShader);

			std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
			particleCS->set_engine_default_res(true);
			ResourceManager<ComputeShader>::get_inst().insert(name::defaultRes::shader::compute::ParticleCS, particleCS);
			particleCS->compile_from_byte_code(Particle_CS, sizeof(Particle_CS));
		}
#pragma endregion
	}

}
