#pragma once
#include <Engine/GPU/CommonGPU.h>
#include <Engine/Common.h>
#include <Engine/Util/Singleton.h>

#include <Engine/Manager/SceneRenderAgent.h>

namespace core
{
	class ConstBuffer;
	class StructBuffer;
	class Texture;

	class Camera;
	class Renderer;

	class Light;
	class Light3D;
	class GameObject;

	class MultiRenderTarget;

	class RenderManager
	{
		friend class GameEngine;

		DECLARE_SINGLETON(RenderManager);
	private:
		RenderManager();
		void init();

		~RenderManager();

		void load_render_states();
		void init(const tGPUManagerDesc& _Desc);

	public:
		__forceinline ComPtr<ID3D11Device>			get_device() { return m_device; }
		__forceinline ComPtr<ID3D11DeviceContext>	get_context() { return m_context; }

		void render();
		void render_debug();
		void FrameEnd();

		void clear_rendertarget();// 화면 지워주기
		void present(bool _bVSync = false);

		s_ptr<Texture> get_rendertarget_buffer() {
			return m_render_target;
		}
		s_ptr<Texture> get_depth_stencil_buffer() {
			return m_depth_stencil_buffer;
		}

		bool set_resolution(UINT _resX, UINT _resY);
		UINT GetResolutionX() { return m_resolution_x; }
		UINT GetResolutionY() { return m_resolution_y; }
		uint2 GetResolution() { return uint2{ m_resolution_x, m_resolution_y }; }

		SceneRenderAgent& sceneRenderAgent() { return m_scene_render_agent; }

		ConstBuffer* get_const_buffer(eCBType _Type) { return m_const_buffers[(int)_Type].get(); }

		ComPtr<ID3D11RasterizerState> get_rasterizer_state(eRasterizerState _Type) { 
			return m_rasterizer_states[(int)_Type]; 
		}
		ComPtr<ID3D11BlendState> get_blend_state(eBlendState _Type) { 
			return m_blend_states[(int)_Type]; 
		}
		ComPtr<ID3D11DepthStencilState> get_depth_stencil_state(eDepthStencilState _Type) { 
			return m_depth_stencil_states[(int)_Type]; 
		}

		MultiRenderTarget* get_render_target(eMRTType _Type) {
			return m_multi_render_targets[(int)_Type].get();
		}

		void bind_noise_texture();
		void copy_rendertarget();

		void clear_multi_rendertargets();

	private:
		//스왑체인 + 렌더타겟 생성하여 반환
		s_ptr<Texture> create_swapchain(UINT _resX, UINT _resY, UINT _RefreshRate);
		s_ptr<Texture> create_depth_stencil_buffer(UINT _resX, UINT _resY);

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
	
		ComPtr<IDXGISwapChain> m_swap_chain;
		
		s_ptr<core::Texture> m_render_target;
		s_ptr<core::Texture> m_depth_stencil_buffer;
		
		D3D11_VIEWPORT m_viewport;

		std::array<std::unique_ptr<MultiRenderTarget>, (int)eMRTType::END> m_multi_render_targets;

		std::array<std::unique_ptr<ConstBuffer>, (int)eCBType::END>		m_const_buffers;
		std::array<ComPtr<ID3D11SamplerState>, (int)eSamplerType::END>	m_sampler_states;
		std::array<ComPtr<ID3D11RasterizerState>, (int)eRasterizerState::END>	m_rasterizer_states;
		std::array<ComPtr<ID3D11DepthStencilState>, (int)eDepthStencilState::END>	m_depth_stencil_states;
		std::array<ComPtr<ID3D11BlendState>, (int)eBlendState::END>			m_blend_states;

		s_ptr<Texture>				m_post_process_texture;
		s_ptr<Texture>				m_noise_texture;

		SceneRenderAgent m_scene_render_agent;
	};
}


