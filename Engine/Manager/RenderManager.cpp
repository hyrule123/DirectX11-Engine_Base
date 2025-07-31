
#include "RenderManager.h"

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/TimeManager.h>


#include <Engine/GameEngine.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>
#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Shader/ComputeShaders/GPUInitSetting.h>

#include <Engine/Util/AtExit.h>

#include <Engine/GPU/ConstBuffer.h>
#include <Engine/GPU/StructBuffer.h>
#include <Engine/GPU/MultiRenderTarget.h>

#include <Engine/DefaultShader/3D/3D.hlsli>
#include <Engine/DefaultShader/Light/Light.hlsli>


namespace core
{
	RenderManager::RenderManager() 
		: m_resolution_x{}
		, m_resolution_y{}
		, m_refreshRate{}
		, m_device{}
		, m_context{}
		, m_swap_chain{}
		, m_render_target{}
		, m_depth_stencil_buffer{}
		, m_viewport{}
		, m_multi_render_targets{}
		, m_const_buffers{}
		, m_sampler_states{}
		, m_rasterizer_states{}
		, m_depth_stencil_states{}
		, m_blend_states{}
		, m_post_process_texture{}
		, m_scene_render_agent{}
	{}

	void RenderManager::init()
	{
		// Device, Device Context
		uint DeviceFlag{};
#ifdef _DEBUG
		DeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

		D3D_FEATURE_LEVEL MaxSupportedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL FeatureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			//D3D_FEATURE_LEVEL_10_1,
			//D3D_FEATURE_LEVEL_10_0,
			//D3D_FEATURE_LEVEL_9_3,
			//D3D_FEATURE_LEVEL_9_2,
			//D3D_FEATURE_LEVEL_9_1
		};

		if (FAILED(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			DeviceFlag,
			FeatureLevels,
			sizeof(FeatureLevels) / sizeof(D3D_FEATURE_LEVEL),
			D3D11_SDK_VERSION,
			m_device.ReleaseAndGetAddressOf(),
			&MaxSupportedFeatureLevel,
			m_context.ReleaseAndGetAddressOf())))
		{
			m_device.Reset();
			m_context.Reset();
			ASSERT(false, "Graphics Device 생성에 실패했습니다.");
		}

		AtExit::add_func(RenderManager::destroy_inst);
	}

	RenderManager::~RenderManager()
	{
		m_scene_render_agent.release();

		for (int i = 0; i < (int)eCBType::END; ++i)
		{
			m_const_buffers[i].reset();
		}
		for (int i = 0; i < (int)eSamplerType::END; ++i)
		{
			m_sampler_states[i] = nullptr;
		}
		for (int i = 0; i < (int)eRasterizerState::END; ++i)
		{
			m_rasterizer_states[i] = nullptr;
		}
		for (int i = 0; i < (int)eDepthStencilState::END; ++i)
		{
			m_depth_stencil_states[i] = nullptr;
		}
		for (int i = 0; i < (int)eBlendState::END; ++i)
		{
			m_blend_states[i] = nullptr;
		}

		m_post_process_texture = nullptr;

		for (int i = 0; i < (int)eMRTType::END; ++i)
		{
			m_multi_render_targets[i].reset();
		}

		m_viewport = D3D11_VIEWPORT{};
		m_depth_stencil_buffer = nullptr;
		m_render_target = nullptr;
		m_swap_chain = nullptr;

		//마지막으로 context와 device 제거
		m_context = nullptr;
		m_device = nullptr;
	}

	void RenderManager::load_render_states()
	{
		create_default_render_states();
		create_rasterizer_states();
		create_depth_stencil_states();
		create_blend_states();
		create_const_buffers();
	}

	void RenderManager::create_const_buffers()
	{
#pragma region CONSTANT BUFFER
		m_const_buffers[(uint)eCBType::Global] = std::make_unique<ConstBuffer>((uint)eCBType::Global);
		m_const_buffers[(uint)eCBType::Global]->create(sizeof(tCB_Global));
		m_const_buffers[(uint)eCBType::Global]->set_target_stage(eShaderStageFlag::ALL);

		update_global_const_buffer();

		m_const_buffers[(uint)eCBType::Camera] = std::make_unique<ConstBuffer>((uint)eCBType::Camera);
		m_const_buffers[(uint)eCBType::Camera]->create<tCamera>();

		m_const_buffers[(uint)eCBType::ComputeShader] = std::make_unique<ConstBuffer>((uint)eCBType::ComputeShader);
		m_const_buffers[(uint)eCBType::ComputeShader]->create<tCB_ComputeShader>();
		m_const_buffers[(uint)eCBType::ComputeShader]->set_target_stage(eShaderStageFlag::Compute);

		m_const_buffers[(uint)eCBType::Material] = std::make_unique<ConstBuffer>((uint)eCBType::Material);
		m_const_buffers[(uint)eCBType::Material]->create(sizeof(tSharedMaterialData));

		m_const_buffers[(uint)eCBType::Animation2D] = std::make_unique<ConstBuffer>((uint)eCBType::Animation2D);
		m_const_buffers[(uint)eCBType::Animation2D]->create(sizeof(tCB_Animation2D));

		m_const_buffers[(uint)eCBType::ParticleSystem] = std::make_unique<ConstBuffer>((uint)eCBType::ParticleSystem);
		m_const_buffers[(uint)eCBType::ParticleSystem]->create(sizeof(tCB_ParticleSystem));

		m_const_buffers[(uint)eCBType::Noise] = std::make_unique<ConstBuffer>((uint)eCBType::Noise);
		m_const_buffers[(uint)eCBType::Noise]->create(sizeof(tCB_Noise));

		m_const_buffers[(uint)eCBType::Animation3D] = std::make_unique<ConstBuffer>((uint)eCBType::Animation3D);
		m_const_buffers[(uint)eCBType::Animation3D]->create<tAnimation3D_ComputeShader_Data>();

#pragma endregion
	}

	void RenderManager::init(const tGPUManagerDesc& _Desc)
	{
		if (nullptr == m_device || nullptr == m_context) {
			ASSERT(false, "Device Context가 생성되지 않았습니다. init()를 먼저 호출하세요.");
		}
		m_refreshRate = _Desc.refresh_rate;

		if (false == set_resolution(_Desc.resolution_X, _Desc.resolution_Y))
		{
			ASSERT(false, "GPU 초기화 작업 실패");
		}

		create_main_viewport();

		m_scene_render_agent.init();
	}


	bool RenderManager::set_resolution(UINT _resX, UINT _resY)
	{
		bool bResult = false;
		//1. 스왑체인 및 최종 렌더타겟 생성
		s_ptr<Texture> RenderTex = create_swapchain(_resX, _resY, m_refreshRate);
		if (nullptr == RenderTex)
		{
			ERROR_MESSAGE("렌더타겟 생성 실패");
			return false;
		}
		m_render_target = RenderTex;

		//3. DS 텍스처 다시 생성
		s_ptr<Texture> DSTex = create_depth_stencil_buffer(_resX, _resY);
		if (nullptr == DSTex)
		{
			ERROR_MESSAGE("Depth-Stencil 텍스처 생성 실패");
			return false;
		}
		m_depth_stencil_buffer = DSTex;

		if (false == create_multi_rendertargets(_resX, _resY))
		{
			ERROR_MESSAGE("해상도 변경 실패");
			return false;
		}

		//4. RenderMgr에서 해상도에 영향을 받는 요소들 값 변경
		//RenderManager가 초기화 된 이후(나중에 해상도 변경 시)에만 여기서 실행
		m_scene_render_agent.set_resolution(_resX, _resY);

		g_global_cbuffer.uResolution = uint2(_resX, _resY);
		g_global_cbuffer.fResolution = float2(_resX, _resY);

		return true;
	}

	s_ptr<Texture> RenderManager::create_swapchain(UINT _resX, UINT _resY, UINT _RefreshRate)
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};

		swap_chain_desc.OutputWindow = GameEngine::get_inst().get_HWND();
		swap_chain_desc.Windowed = true;
		swap_chain_desc.BufferCount = 2;
		//DXGI WARNING: IDXGIFactory::CreateSwapChain: Blt-model swap effects (DXGI_SWAP_EFFECT_DISCARD and DXGI_SWAP_EFFECT_SEQUENTIAL) are legacy swap effects that are predominantly superceded by their flip-model counterparts (DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL and DXGI_SWAP_EFFECT_FLIP_DISCARD)
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferDesc.Width = _resX;
		swap_chain_desc.BufferDesc.Height = _resY;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = _RefreshRate;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1u;

		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

		HRESULT result = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
		if (FAILED(result))
		{
			return nullptr;
		}

		result = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf());
		if (FAILED(result))
		{
			return nullptr;
		}

		result = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf());
		if (FAILED(result))
		{
			return nullptr;
		}

		result = pDXGIFactory->CreateSwapChain(m_device.Get(), &swap_chain_desc, m_swap_chain.GetAddressOf());
		if (FAILED(result))
		{
			return nullptr;
		}

		
		//최종 렌더타겟 생성
		s_ptr<Texture> ret = std::make_shared<Texture>();
		Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target = nullptr;

		// Get rendertarget for swapchain
		result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)render_target.GetAddressOf());
		if (FAILED(result))
		{
			ERROR_MESSAGE("스왑체인으로부터 렌더타겟을 받아오는 데 실패했습니다.");
			return nullptr;
		}

		//스왑체인으로 받아온 텍스처 버퍼를 가지고 렌더타겟 텍스처 뷰(RTV) 생성
		if (false == ret->create(render_target))
		{
			ERROR_MESSAGE("렌더타겟 텍스처 생성에 실패했습니다.");
			return nullptr;
		}

		m_resolution_x = _resX;
		m_resolution_y = _resY;
		return ret;
	}


	s_ptr<Texture> RenderManager::create_depth_stencil_buffer(UINT _Width, UINT _Height)
	{
		s_ptr<Texture> DS_texture = std::make_shared<core::Texture>();
		
		if (false == DS_texture->create(_Width, _Height, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL))
		{
			ERROR_MESSAGE("Depth Stencil 버퍼 생성에 실패했습니다.");
			return nullptr;
		}

		return DS_texture;
	}

	void RenderManager::create_main_viewport()
	{
		//창 속에서의 상대적인 좌표이므로 0부터 시작하면 됨
		m_viewport.TopLeftX = (FLOAT)0.f;
		m_viewport.TopLeftY = (FLOAT)0.f;
		m_viewport.MinDepth = (FLOAT)0.f;
		m_viewport.MaxDepth = (FLOAT)1.f;

		int2 window_size = GameEngine::get_inst().get_window_size();

		m_viewport.Width = (FLOAT)window_size.x;
		m_viewport.Height = (FLOAT)window_size.y;

		m_context->RSSetViewports(1u, &m_viewport);
	}



	void RenderManager::clear_rendertarget()
	{
		constexpr FLOAT backgroundColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_context->ClearRenderTargetView(m_render_target->get_RTV().Get(), backgroundColor);
		m_context->ClearDepthStencilView(m_depth_stencil_buffer->get_DSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void RenderManager::present(bool _bVSync)
	{
		m_swap_chain->Present(_bVSync ? 1 : 0, 0u);
	}


	void RenderManager::render()
	{
		clear_multi_rendertargets();

		update_global_const_buffer();

		bind_noise_texture();

		m_scene_render_agent.render();
	}

	void RenderManager::render_debug()
	{
		m_scene_render_agent.render_debug();
	}

	void RenderManager::FrameEnd()
	{
		m_scene_render_agent.frame_end();
	}

	void RenderManager::bind_noise_texture()
	{
		s_ptr<Texture> noise = ResourceManager<Texture>::get_inst().find(name::defaultRes::texture::noise_03);
		noise->bind_buffer_as_SRV(GPU::Register::t::NoiseTexture, eShaderStageFlag::ALL);

		tCB_Noise info = {};
		info.NoiseSize.x = (float)noise->get_texture_width();
		info.NoiseSize.y = (float)noise->get_texture_height();

		static float noiseTime = 10.f;
		noiseTime -= TimeManager::get_inst().delta_time();
		info.NoiseTime = noiseTime;

		ConstBuffer* cb = m_const_buffers[(uint)eCBType::Noise].get();
		cb->set_data(&info);
		cb->bind_buffer_to_GPU_register(eShaderStageFlag::ALL);
	}
	void RenderManager::copy_rendertarget()
	{
		s_ptr<Texture> renderTarget = ResourceManager<Texture>::get_inst().find(name::defaultRes::texture::RenderTarget);

		//render_target->unbind_buffer();

		//ID3D11ShaderResourceView* srv = nullptr;
		//RenderManager::get_inst().Context()->PSSetShaderResources()(eShaderStage::Pixel, 60, &srv);

		ID3D11Texture2D* dest = m_post_process_texture->get_texture().Get();
		ID3D11Texture2D* source = renderTarget->get_texture().Get();

		RenderManager::get_inst().get_context()->CopyResource(dest, source);

		m_post_process_texture->bind_buffer_as_SRV(GPU::Register::t::postProcessTexture, eShaderStageFlag::Pixel);
	}

	void RenderManager::clear_multi_rendertargets()
	{
		for (int i = 0; i < (int)eMRTType::END; ++i)
		{
			if (m_multi_render_targets[i])
			{
				if ((int)eMRTType::Swapchain == i)
				{
					m_multi_render_targets[i]->fill(float4(0.2f));
				}
				else
				{
					m_multi_render_targets[i]->fill(float4(0.f));
				}
			}

		}
	}


	void RenderManager::update_global_const_buffer()
	{
		g_global_cbuffer.DeltaTime = TimeManager::get_inst().delta_time();
		g_global_cbuffer.AccFramePrev = g_global_cbuffer.AccFrame;
		g_global_cbuffer.AccFrame += 1u;

		ConstBuffer* global = get_const_buffer(eCBType::Global);
		global->set_data(&g_global_cbuffer);
		global->bind_buffer_to_GPU_register();
	}


	bool RenderManager::create_multi_rendertargets(UINT _resX, UINT _resY)
	{
		{
			//Swapchain MRT
			s_ptr<Texture> arrRTTex[8] = {};
			s_ptr<Texture> dsTex = nullptr;

			arrRTTex[0] = RenderManager::get_inst().get_rendertarget_buffer();
			dsTex = RenderManager::get_inst().get_depth_stencil_buffer();

			m_multi_render_targets[(UINT)eMRTType::Swapchain] = std::make_unique<MultiRenderTarget>();

			if (false == m_multi_render_targets[(UINT)eMRTType::Swapchain]->create(arrRTTex, dsTex))
			{
				ERROR_MESSAGE("Multi Render Target 생성 실패.");
				return false;
			}
		}

		// Deffered MRT
		{
			s_ptr<Texture> arrRTTex[MRT_MAX] = {};
			s_ptr<Texture> dsTex = nullptr;

			for (int i = 0; i < (int)eMRT_Deffered::END; ++i)
			{
				arrRTTex[i] = std::make_shared<Texture>();
				arrRTTex[i]->create(_resX, _resY, DXGI_FORMAT_R32G32B32A32_FLOAT
					, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
				arrRTTex[i]->get_res_filename(name::eMRT_Deffered_String[i]);
			}


			dsTex = RenderManager::get_inst().get_depth_stencil_buffer();

			m_multi_render_targets[(int)eMRTType::Deffered] = std::make_unique<MultiRenderTarget>();
			if (false == m_multi_render_targets[(int)eMRTType::Deffered]->create(arrRTTex, dsTex))
			{
				ERROR_MESSAGE("Multi Render Target 생성 실패.");
				return false;
			}
		}


		// Light MRT
		{
			s_ptr<Texture> arrRTTex[MRT_MAX] = { };

			for (int i = 0; i < (int)eMRT_Light::END; ++i)
			{
				s_ptr<Texture> defferedTex = std::make_shared<Texture>();
				arrRTTex[i] = defferedTex;
				arrRTTex[i]->create(_resX, _resY, DXGI_FORMAT_R32G32B32A32_FLOAT
					, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
				arrRTTex[i]->get_res_filename(name::eMRT_Light_String[i]);
			}

			m_multi_render_targets[(int)eMRTType::Light] = std::make_unique<MultiRenderTarget>();
			m_multi_render_targets[(int)eMRTType::Light]->create(arrRTTex, nullptr);
		}

		set_MRT_to_deffered_materials();

		return true;
	}

	void RenderManager::set_MRT_to_deffered_materials()
	{
		//Light
		//Light_Diffuse와 Light_Specular을 렌더타겟으로 설정 후(SV_Target0, SV_Target1)
		//Diffuse, Normal, Specular, Emissive, Roughness/Metalic, View Position 렌더타겟을 "텍스처"로 바인딩
		{
			s_ptr<Material> lightDirMtrl = ResourceManager<Material>::get_inst().find(name::defaultRes::material::LightDirMaterial);

			s_ptr<Material> lightPointMtrl = ResourceManager<Material>::get_inst().find(name::defaultRes::material::LightPointMaterial);

			MultiRenderTarget* DefferedMRT = m_multi_render_targets[(uint)eMRTType::Deffered].get();

			{
				//Diffuse Target
				s_ptr<Texture> diffuse =
					DefferedMRT->get_render_target((uint)eMRT_Deffered::Diffuse);
				lightDirMtrl->set_texture(eTextureSlot::rendertarget_diffuse, diffuse);
				lightPointMtrl->set_texture(eTextureSlot::rendertarget_diffuse, diffuse);
			}

			{
				//Specular Target
				s_ptr<Texture> specularTarget =
					DefferedMRT->get_render_target((uint)eMRT_Deffered::Specular);

				lightDirMtrl->set_texture(eTextureSlot::rendertarget_specular, specularTarget);
				lightPointMtrl->set_texture(eTextureSlot::rendertarget_specular, specularTarget);
			}

			{
				//Normal Target
				s_ptr<Texture> normalTarget =
					DefferedMRT->get_render_target((uint)eMRT_Deffered::Normal);

				lightDirMtrl->set_texture(eTextureSlot::rendertarget_normal, normalTarget);
				lightPointMtrl->set_texture(eTextureSlot::rendertarget_normal, normalTarget);
			}

			{
				//View Position Target
				s_ptr<Texture> view_position = 
					DefferedMRT->get_render_target((uint)eMRT_Deffered::ViewPosition);

				lightDirMtrl->set_texture(eTextureSlot::rendertarget_view_position, view_position);
				lightPointMtrl->set_texture(eTextureSlot::rendertarget_view_position, view_position);
			}

			{
				//Routhness/Metalic Target
				s_ptr<Texture> rmt =
					DefferedMRT->get_render_target((uint)eMRT_Deffered::Roughness_Metalic);
				lightDirMtrl->set_texture(eTextureSlot::rendertarget_roughness_metalic, rmt);
				lightPointMtrl->set_texture(eTextureSlot::rendertarget_roughness_metalic, rmt);
			}
		}

		//Merge: Deffered, Light 렌더타겟 전부 바인딩
		{
			s_ptr<Material> mergeMaterial = ResourceManager<Material>::get_inst().find(name::defaultRes::material::MergeMaterial);

			MultiRenderTarget* DefferedMRT = m_multi_render_targets[(uint)eMRTType::Deffered].get();
			{
				s_ptr<Texture> AlbedoRT = DefferedMRT->get_render_target((uint)eMRT_Deffered::Diffuse);
				mergeMaterial->set_texture(eTextureSlot::rendertarget_diffuse, AlbedoRT);
			}

			{
				s_ptr<Texture> NormalRT = DefferedMRT->get_render_target((uint)eMRT_Deffered::Normal);
				mergeMaterial->set_texture(eTextureSlot::rendertarget_normal, NormalRT);
			}

			{
				s_ptr<Texture> SpecularRT = DefferedMRT->get_render_target((uint)eMRT_Deffered::Specular);
				mergeMaterial->set_texture(eTextureSlot::rendertarget_specular, SpecularRT);
			}

			{
				s_ptr<Texture> EmissiveRT = DefferedMRT->get_render_target((uint)eMRT_Deffered::Emissive);
				mergeMaterial->set_texture(eTextureSlot::rendertarget_emissive, EmissiveRT);
			}

			{
				s_ptr<Texture> RoughnessMetailcRT = DefferedMRT->get_render_target((uint)eMRT_Deffered::Roughness_Metalic);
				mergeMaterial->set_texture(eTextureSlot::rendertarget_roughness_metalic, RoughnessMetailcRT);
			}

			{
				s_ptr<Texture> PosRenderTarget = DefferedMRT->get_render_target((uint)eMRT_Deffered::ViewPosition);
				mergeMaterial->set_texture(eTextureSlot::rendertarget_view_position, PosRenderTarget);
			}

			//Light MRT의 0, 1을 6, 7번 텍스처로 바인딩
			MultiRenderTarget* LightMRT = m_multi_render_targets[(uint)eMRTType::Light].get();
			{
				s_ptr<Texture> g_diffuse_light_rendertarget = LightMRT->get_render_target((uint)eMRT_Light::Diffuse_Light);
				mergeMaterial->set_texture(eTextureSlot::rendertarget_diffuse_light, g_diffuse_light_rendertarget);
			}
			{
				s_ptr<Texture> g_specular_light_rendertarget = LightMRT->get_render_target((uint)eMRT_Light::Specular_Light);
				mergeMaterial->set_texture(eTextureSlot::rendertarget_specular_light, g_specular_light_rendertarget);
			}	
		}


	}

}


