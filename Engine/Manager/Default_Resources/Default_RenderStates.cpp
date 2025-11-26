#include <Engine/Manager/RenderManager.h>

#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Shader/RasterizerState.h"
#include "Engine/Resource/Shader/SamplerState.h"
#include "Engine/Resource/Shader/BlendState.h"
#include "Engine/Resource/Shader/DepthStencilState.h"

namespace core {
	void RenderManager::create_default_render_states()
	{
		//POINT, WRAP
		s_ptr<SamplerState> point_sampler = new_entity<SamplerState>();
		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;;
		bool result = point_sampler->create_sampler_state(sampler_desc);
		ASSERT(result);
		ResourceManager::get_inst().insert_default_resource(point_sampler, "sampler_point_wrap");


		s_ptr<SamplerState> aniso_sampler = new_entity<SamplerState>();
		sampler_desc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		result = aniso_sampler->create_sampler_state(sampler_desc);
		ASSERT(result);
		ResourceManager::get_inst().insert_default_resource(aniso_sampler, "sampler_aniso_wrap");
	}

	void RenderManager::create_rasterizer_states()
	{
		ResourceManager& res_mgr_inst = ResourceManager::get_inst();

#pragma region Rasterizer state
		s_ptr<RasterizerState> solid_back = new_entity<RasterizerState>();

		D3D11_RASTERIZER_DESC rs_desc = {};
		rs_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rs_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rs_desc.DepthClipEnable = TRUE;
		bool result = solid_back->create_rasterizer_state(rs_desc);
		ASSERT(result);
		res_mgr_inst.insert_default_resource(solid_back, "rasterizer_state_solid_back");

		s_ptr<RasterizerState> solid_front = new_entity<RasterizerState>();
		rs_desc = {};
		rs_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rs_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		rs_desc.DepthClipEnable = TRUE;
		result = solid_front->create_rasterizer_state(rs_desc);
		ASSERT(result);
		res_mgr_inst.insert_default_resource(solid_front, "rasterizer_state_solid_front");

		s_ptr<RasterizerState> solid_no_face_culling = new_entity<RasterizerState>();
		rs_desc = {};
		rs_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rs_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rs_desc.DepthClipEnable = TRUE;
		result = solid_no_face_culling->create_rasterizer_state(rs_desc);
		ASSERT(result);
		res_mgr_inst.insert_default_resource(solid_no_face_culling, "rasterizer_state_solid_no_face_culling");

		s_ptr<RasterizerState> wireframe_no_face_culling = new_entity<RasterizerState>();
		rs_desc = {};
		rs_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rs_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rs_desc.DepthClipEnable = FALSE;
		result = wireframe_no_face_culling->create_rasterizer_state(rs_desc);
		ASSERT(result);
		res_mgr_inst.insert_default_resource(wireframe_no_face_culling, "rasterizer_state_wireframe_no_face_culling");
	}

	void RenderManager::create_blend_states()
	{
		s_ptr<BlendState> alpha_blend = new_entity<BlendState>();
		D3D11_BLEND_DESC bs_desc = {};
		bs_desc.AlphaToCoverageEnable = false;
		bs_desc.IndependentBlendEnable = false;
		bs_desc.RenderTarget[0].BlendEnable = true;
		bs_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bs_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bs_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bs_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bs_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bs_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bs_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		bool result = alpha_blend->create_blend_state(bs_desc);
		ASSERT(result);
		ResourceManager::get_inst().insert_default_resource(alpha_blend, "blend_state_alpha_blend");

		s_ptr<BlendState> one_one = new_entity<BlendState>();
		bs_desc = {};
		bs_desc.AlphaToCoverageEnable = false;
		bs_desc.IndependentBlendEnable = false;

		bs_desc.RenderTarget[0].BlendEnable = true;
		bs_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bs_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bs_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bs_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		result = one_one->create_blend_state(bs_desc);
		ASSERT(result);
		ResourceManager::get_inst().insert_default_resource(one_one, "blend_state_one_one");
	}

	void RenderManager::create_depth_stencil_states()
	{
		s_ptr<DepthStencilState> less_equal = new_entity<DepthStencilState>();
		D3D11_DEPTH_STENCIL_DESC ds_desc = {};
		ds_desc.DepthEnable = true;
		ds_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		ds_desc.StencilEnable = false;
		bool result = less_equal->create_depth_stencil_state(ds_desc);
		ResourceManager::get_inst().insert_default_resource(less_equal, "depth_stencil_state_less_equal");

		s_ptr<DepthStencilState> less_equal_no_write = new_entity<DepthStencilState>();
		ds_desc = {};
		ds_desc.DepthEnable = true;
		ds_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		ds_desc.StencilEnable = false;
		result = less_equal_no_write->create_depth_stencil_state(ds_desc);
		ResourceManager::get_inst().insert_default_resource(less_equal_no_write, "depth_stencil_state_less_equal_no_write");
	}
}
