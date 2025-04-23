#pragma once
#include "Engine/GPU/CommonGPU.h"
#include "Engine/Common.h"
#include "Engine/Util/Singleton.h"

#include "Engine/Manager/SceneRenderAgent.h"

namespace core
{
	class ConstBuffer;
	class StructBuffer;
	class Texture;

	class Com_Camera;
	class Renderer;

	class Light;
	class Light_3D;
	class GameObject;

	class MultiRenderTarget;

	class RenderManager : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;
		friend class GameEngine;

	private:
		RenderManager();
		~RenderManager();

		void load_render_states();
		void init(const tGPUManagerDesc& _Desc);

	public:
		__forceinline ID3D11Device*			Device() { return m_device.Get(); }
		__forceinline ID3D11DeviceContext*	Context() { return m_context.Get(); }

		void render();
		void render_debug();
		void FrameEnd();

		void clear_rendertarget();// 화면 지워주기
		void present(bool _bVSync = false);

		std::shared_ptr<Texture> get_rendertarget_buffer() {
			return m_render_target;
		}
		std::shared_ptr<Texture> get_depth_stencil_buffer() {
			return m_depth_stencil_buffer;
		}

		bool SetResolution(UINT _resX, UINT _resY);
		UINT GetResolutionX() { return m_resolution_x; }
		UINT GetResolutionY() { return m_resolution_y; }
		uint2 GetResolution() { return uint2{ m_resolution_x, m_resolution_y }; }

		SceneRenderAgent& sceneRenderAgent() { return m_sceneRenderAgent; }

		ConstBuffer* get_const_buffer(eCBType _Type) { return m_constBuffers[(int)_Type].get(); }

		ComPtr<ID3D11RasterizerState> get_rasterizer_state(eRasterizerState _Type) { 
			return m_rasterizerStates[(int)_Type]; 
		}
		ComPtr<ID3D11BlendState> get_blend_state(eBlendState _Type) { 
			return m_blendStates[(int)_Type]; 
		}
		ComPtr<ID3D11DepthStencilState> get_depth_stencil_state(eDepthStencilState _Type) { 
			return m_depthStencilStates[(int)_Type]; 
		}

		MultiRenderTarget* get_rendertarget(eMRTType _Type) {
			return m_multi_render_targets[(int)_Type].get();
		}

		void bind_noise_texture();
		void copy_rendertarget();

		void clear_multi_rendertargets();

	private:
		//스왑체인 + 렌더타겟 생성하여 반환
		std::shared_ptr<Texture> create_swapchain(UINT _resX, UINT _resY, UINT _RefreshRate);
		std::shared_ptr<Texture> create_depth_stencil_buffer(UINT _resX, UINT _resY);

		//Application의 창 크기를 따라감
		void create_main_viewport();

		void update_global_const_buffer();

		bool create_multi_rendertargets(UINT _resX, UINT _resY);
		void set_MRT_to_deffered_materials();

		void create_default_render_states();
		void create_rasterizer_states();
		void create_depth_stencil_states();
		void create_blend_states();
		void create_const_buffers();

	private:
		UINT m_resolution_x;
		UINT m_resolution_y;
		UINT m_refreshRate;

		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;
	
		ComPtr<IDXGISwapChain> m_swapChain;
		
		std::shared_ptr<core::Texture> m_render_target;
		std::shared_ptr<core::Texture> m_depth_stencil_buffer;
		
		D3D11_VIEWPORT m_viewport;

		std::array<std::unique_ptr<MultiRenderTarget>, (int)eMRTType::END> m_multi_render_targets;

		std::array<std::unique_ptr<ConstBuffer>, (int)eCBType::END>		m_constBuffers;
		std::array<ComPtr<ID3D11SamplerState>, (int)eSamplerType::END>	m_samplerStates;
		std::array<ComPtr<ID3D11RasterizerState>, (int)eRasterizerState::END>	m_rasterizerStates;
		std::array<ComPtr<ID3D11DepthStencilState>, (int)eDepthStencilState::END>	m_depthStencilStates;
		std::array<ComPtr<ID3D11BlendState>, (int)eBlendState::END>			m_blendStates;

		std::shared_ptr<Texture>				m_postProcessTexture;
		std::shared_ptr<Texture>				m_noiseTexture;

		SceneRenderAgent m_sceneRenderAgent;
	};
}


