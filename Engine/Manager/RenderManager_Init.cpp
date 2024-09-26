#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/GPU/ConstBuffer.h"

namespace ehw {
	void RenderManager::create_sampler_states()
	{

#pragma region sampler state
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;


		RenderManager::get_inst().Device()->CreateSamplerState
		(
			&samplerDesc
			, m_samplerStates[(uint)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		RenderManager::get_inst().Device()->CreateSamplerState
		(
			&samplerDesc
			, m_samplerStates[(uint)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		RenderManager::get_inst().Device()->CreateSamplerState
		(
			&samplerDesc
			, m_samplerStates[(uint)eSamplerType::Anisotropic].GetAddressOf()
		);


		RenderManager::get_inst().Context()->PSSetSamplers((uint)eSamplerType::Point
			, 1, m_samplerStates[(uint)eSamplerType::Point].GetAddressOf());

		RenderManager::get_inst().Context()->PSSetSamplers((uint)eSamplerType::Linear
			, 1, m_samplerStates[(uint)eSamplerType::Linear].GetAddressOf());

		RenderManager::get_inst().Context()->PSSetSamplers((uint)eSamplerType::Anisotropic
			, 1, m_samplerStates[(uint)eSamplerType::Anisotropic].GetAddressOf());

#pragma endregion
	}

	void RenderManager::create_rasterizer_states()
	{
#pragma region Rasterizer state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rsDesc.DepthClipEnable = TRUE;

		RenderManager::get_inst().Device()->CreateRasterizerState(&rsDesc
			, m_rasterizerStates[(uint)eRasterizerState::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		rsDesc.DepthClipEnable = TRUE;

		RenderManager::get_inst().Device()->CreateRasterizerState(&rsDesc
			, m_rasterizerStates[(uint)eRasterizerState::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		RenderManager::get_inst().Device()->CreateRasterizerState(&rsDesc
			, m_rasterizerStates[(uint)eRasterizerState::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		RenderManager::get_inst().Device()->CreateRasterizerState(&rsDesc
			, m_rasterizerStates[(uint)eRasterizerState::WireframeNone].GetAddressOf());
#pragma endregion
	}

	void RenderManager::create_blend_states()
	{

#pragma region Blend State
		//None
		m_blendStates[(uint)eBlendState::Default] = nullptr;

		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		RenderManager::get_inst().Device()->CreateBlendState(&bsDesc, m_blendStates[(uint)eBlendState::AlphaBlend].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		RenderManager::get_inst().Device()->CreateBlendState(&bsDesc, m_blendStates[(uint)eBlendState::OneOne].GetAddressOf());

#pragma endregion
	}

	void RenderManager::create_default_const_buffers()
	{
#pragma region CONSTANT BUFFER
		m_constBuffers[(uint)eCBType::Global] = std::make_unique<ConstBuffer>((uint)eCBType::Global);
		m_constBuffers[(uint)eCBType::Global]->create(sizeof(tCB_Global));
		m_constBuffers[(uint)eCBType::Global]->SetPresetTargetStage(eShaderStageFlag::ALL);

		update_global_const_buffer();

		m_constBuffers[(uint)eCBType::Camera] = std::make_unique<ConstBuffer>((uint)eCBType::Camera);
		m_constBuffers[(uint)eCBType::Camera]->create<tCamera>();

		m_constBuffers[(uint)eCBType::ComputeShader] = std::make_unique<ConstBuffer>((uint)eCBType::ComputeShader);
		m_constBuffers[(uint)eCBType::ComputeShader]->create<tCB_ComputeShader>();
		m_constBuffers[(uint)eCBType::ComputeShader]->SetPresetTargetStage(eShaderStageFlag::Compute);

		m_constBuffers[(uint)eCBType::Material] = std::make_unique<ConstBuffer>((uint)eCBType::Material);
		m_constBuffers[(uint)eCBType::Material]->create(sizeof(tSharedMaterialData));

		m_constBuffers[(uint)eCBType::Animation2D] = std::make_unique<ConstBuffer>((uint)eCBType::Animation2D);
		m_constBuffers[(uint)eCBType::Animation2D]->create(sizeof(tCB_Animation2D));

		m_constBuffers[(uint)eCBType::ParticleSystem] = std::make_unique<ConstBuffer>((uint)eCBType::ParticleSystem);
		m_constBuffers[(uint)eCBType::ParticleSystem]->create(sizeof(tCB_ParticleSystem));

		m_constBuffers[(uint)eCBType::Noise] = std::make_unique<ConstBuffer>((uint)eCBType::Noise);
		m_constBuffers[(uint)eCBType::Noise]->create(sizeof(tCB_Noise));

		m_constBuffers[(uint)eCBType::Animation3D] = std::make_unique<ConstBuffer>((uint)eCBType::Animation3D);
		m_constBuffers[(uint)eCBType::Animation3D]->create<tAnimation3D_ComputeShaderData>();

#pragma endregion
	}

	void RenderManager::create_depth_stencil_states()
	{
#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		RenderManager::get_inst().Device()->CreateDepthStencilState(&dsDesc
			, m_depthStencilStates[(uint)eDepthStencilState::Less].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		RenderManager::get_inst().Device()->CreateDepthStencilState(&dsDesc
			, m_depthStencilStates[(uint)eDepthStencilState::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		RenderManager::get_inst().Device()->CreateDepthStencilState(&dsDesc
			, m_depthStencilStates[(uint)eDepthStencilState::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		RenderManager::get_inst().Device()->CreateDepthStencilState(&dsDesc
			, m_depthStencilStates[(uint)eDepthStencilState::None].GetAddressOf());
#pragma endregion
	}
}
