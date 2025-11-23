#include "GraphicsPipeline.h"

#include "Engine/Global.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

#include "Engine/Resource/Shader/VertexShader.h"
#include "Engine/Resource/Shader/HullShader.h"
#include "Engine/Resource/Shader/DomainShader.h"
#include "Engine/Resource/Shader/GeometryShader.h"
#include "Engine/Resource/Shader/PixelShader.h"
#include "Engine/Resource/Shader/RasterizerState.h"
#include "Engine/Resource/Shader/BlendState.h"
#include "Engine/Resource/Shader/DepthStencilState.h"

namespace core
{
	GraphicsPipeline::GraphicsPipeline()
		: Resource()
	{
		m_vertex_shader = new_entity<VertexShader>();
		m_hull_shader = new_entity<HullShader>();
		m_domain_shader = new_entity<DomainShader>();
		m_geometry_shader = new_entity<GeometryShader>();
		m_pixel_shader = new_entity<PixelShader>();

		m_sampler_state = new_entity<SamplerState>();
		m_rasterizer_state = new_entity<RasterizerState>();
		m_blend_state = new_entity<BlendState>();
		m_depth_stencil_state = new_entity<DepthStencilState>();
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
	}

	eResult GraphicsPipeline::save(const std::fs::path& _full_path) const
	{
		ASSERT_MESSAGE_RELEASE(false, "미구현");
		return eResult::Fail;
	}

	eResult GraphicsPipeline::load(const std::fs::path& _full_path)
	{
		ASSERT_MESSAGE_RELEASE(false, "미구현");
		return eResult::Fail;
	}

	void GraphicsPipeline::bind()
	{
		auto context = RenderManager::get_inst().get_context();

		m_vertex_shader->bind(context);
		m_hull_shader->bind(context);
		m_domain_shader->bind(context);
		m_geometry_shader->bind(context);
		m_pixel_shader->bind(context);
		
		m_rasterizer_state->bind(context);
		m_blend_state->bind(context);
		m_depth_stencil_state->bind(context);
	}

	void GraphicsPipeline::unbind_all()
	{
		auto pContext = RenderManager::get_inst().get_context();

		pContext->IASetInputLayout(nullptr);
		pContext->VSSetShader(nullptr, nullptr, 0);
		pContext->HSSetShader(nullptr, nullptr, 0);
		pContext->DSSetShader(nullptr, nullptr, 0);
		pContext->GSSetShader(nullptr, nullptr, 0);
		pContext->PSSetShader(nullptr, nullptr, 0);

		pContext->RSSetState(nullptr);
		pContext->OMSetDepthStencilState(nullptr, 10u);
		pContext->OMSetBlendState(nullptr, nullptr, UINT_MAX);
	}
