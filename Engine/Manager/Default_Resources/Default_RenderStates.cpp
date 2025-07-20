#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/ResourceManager.h>

namespace core {
	void RenderManager::create_default_render_states()
	{
#pragma region sampler state
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;


		get_device()->CreateSamplerState
		(
			&samplerDesc
			, m_sampler_states[(uint)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		get_device()->CreateSamplerState
		(
			&samplerDesc
			, m_sampler_states[(uint)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		get_device()->CreateSamplerState
		(
			&samplerDesc
			, m_sampler_states[(uint)eSamplerType::Anisotropic].GetAddressOf()
		);


		get_context()->PSSetSamplers((uint)eSamplerType::Point
			, 1, m_sampler_states[(uint)eSamplerType::Point].GetAddressOf());

		get_context()->PSSetSamplers((uint)eSamplerType::Linear
			, 1, m_sampler_states[(uint)eSamplerType::Linear].GetAddressOf());

		get_context()->PSSetSamplers((uint)eSamplerType::Anisotropic
			, 1, m_sampler_states[(uint)eSamplerType::Anisotropic].GetAddressOf());

#pragma endregion
	}

	void RenderManager::create_rasterizer_states()
	{
#pragma region Rasterizer state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rsDesc.DepthClipEnable = TRUE;

		get_device()->CreateRasterizerState(&rsDesc
			, m_rasterizer_states[(uint)eRasterizerState::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		rsDesc.DepthClipEnable = TRUE;

		get_device()->CreateRasterizerState(&rsDesc
			, m_rasterizer_states[(uint)eRasterizerState::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		get_device()->CreateRasterizerState(&rsDesc
			, m_rasterizer_states[(uint)eRasterizerState::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		get_device()->CreateRasterizerState(&rsDesc
			, m_rasterizer_states[(uint)eRasterizerState::WireframeNone].GetAddressOf());
#pragma endregion
	}

	void RenderManager::create_blend_states()
	{

#pragma region Blend State
		//None
		m_blend_states[(uint)eBlendState::Default] = nullptr;

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

		get_device()->CreateBlendState(&bsDesc, m_blend_states[(uint)eBlendState::AlphaBlend].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		get_device()->CreateBlendState(&bsDesc, m_blend_states[(uint)eBlendState::OneOne].GetAddressOf());

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
		get_device()->CreateDepthStencilState(&dsDesc
			, m_depth_stencil_states[(uint)eDepthStencilState::Less].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		get_device()->CreateDepthStencilState(&dsDesc
			, m_depth_stencil_states[(uint)eDepthStencilState::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		get_device()->CreateDepthStencilState(&dsDesc
			, m_depth_stencil_states[(uint)eDepthStencilState::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		get_device()->CreateDepthStencilState(&dsDesc
			, m_depth_stencil_states[(uint)eDepthStencilState::None].GetAddressOf());
#pragma endregion
	}
}
