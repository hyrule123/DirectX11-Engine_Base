#pragma once
#include "Engine/GPU/CommonGPU.h"
#include "Engine/Common.h"
#include "Engine/Util/StaticSingleton.h"

#include "Engine/Manager/SceneRenderAgent.h"

namespace ehw
{
	class ConstBuffer;
	class StructBuffer;
	class Texture;

	class Com_Camera;
	class Renderer;

	class Light;
	class Com_Light3D;
	class GameObject;

	class MultiRenderTarget;

	class RenderManager : public StaticSingleton<RenderManager>
	{
		friend class StaticSingleton<RenderManager>;
		friend class GameEngine;

	public:
		__forceinline ID3D11Device*			Device() { return m_device.Get(); }
		__forceinline ID3D11DeviceContext*	Context() { return m_context.Get(); }

		void Render();

		void ClearRenderTarget();// 화면 지워주기
		inline void Present(bool _bVSync = false);

		inline std::shared_ptr<Texture> GetRenderTargetTex();
		inline std::shared_ptr<Texture> GetDepthStencilBufferTex();

		bool SetResolution(UINT _resX, UINT _resY);
		UINT GetResolutionX() { return m_resolution_x; }
		UINT GetResolutionY() { return m_resolution_y; }
		uint2 GetResolution() { return uint2{ m_resolution_x, m_resolution_y }; }

		SceneRenderAgent& sceneRenderAgent() { return m_sceneRenderAgent; }

		ConstBuffer* GetConstBuffer(eCBType _Type) { return m_constBuffers[(int)_Type].get(); }

		ID3D11RasterizerState* GetRasterizerState(eRSType _Type) { return m_rasterizerStates[(int)_Type].Get(); }
		ID3D11BlendState* GetBlendState(eBSType _Type) { return m_blendStates[(int)_Type].Get(); }
		ID3D11DepthStencilState* GetDepthStencilState(eDSType _Type) { return m_depthStencilStates[(int)_Type].Get(); }

		MultiRenderTarget* GetMultiRenderTarget(eMRTType _Type) {
			return m_multi_render_targets[(int)_Type].get();
		}

		void BindNoiseTexture();
		void CopyRenderTarget();

		void ClearMultiRenderTargets();

		void SetDebugData(const tCB_CustomData& _debugData);
		tCB_CustomData GetDebugData();

		StructBuffer* GetLightSBuffer() { return m_lights_SBuffer.get(); }

	private:
		RenderManager();
		~RenderManager();
		
		bool Init();
		bool Setting(const tGPUManagerDesc& _Desc);
		void Release();
		
		//스왑체인 + 렌더타겟 생성하여 반환
		std::shared_ptr<Texture> CreateSwapChain(UINT _resX, UINT _resY, UINT _RefreshRate);
		std::shared_ptr<Texture> CreateDepthStencil(UINT _resX, UINT _resY);

		//Application의 창 크기를 따라감
		void CreateMainViewPort();

	private:
		void UpdateGlobalCBuffer();

		bool CreateMultiRenderTargets(UINT _resX, UINT _resY);
		void SetTexturesToDefferedMaterials();

		void LoadDefaultMesh();
		void LoadDefaultMaterial();
		void LoadDefaultShader();
		void LoadDefaultTexture();

		void CreateSamplerStates();
		void CreateRasterizerStates();
		void CreateDepthStencilStates();
		void CreateBlendStates();

		void CreateBuffer();

	private:
		UINT m_resolution_x;
		UINT m_resolution_y;
		UINT m_refreshRate;

		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;
	
		ComPtr<IDXGISwapChain> m_swapChain;
		
		std::shared_ptr<ehw::Texture> m_renderTarget_texture;
		std::shared_ptr<ehw::Texture> m_depth_stencil_buffer_texture;
		
		D3D11_VIEWPORT m_viewport;

		std::array<std::unique_ptr<MultiRenderTarget>, (int)eMRTType::END> m_multi_render_targets;

		std::array<std::unique_ptr<ConstBuffer>, (int)eCBType::END>		m_constBuffers;
		std::array<ComPtr<ID3D11SamplerState>, (int)eSamplerType::END>	m_samplerStates;
		std::array<ComPtr<ID3D11RasterizerState>, (int)eRSType::END>	m_rasterizerStates;
		std::array<ComPtr<ID3D11DepthStencilState>, (int)eDSType::END>	m_depthStencilStates;
		std::array<ComPtr<ID3D11BlendState>, (int)eBSType::END>			m_blendStates;

		std::shared_ptr<Texture>				m_postProcessTexture;
		std::shared_ptr<Texture>				m_noiseTexture;
		std::unique_ptr<StructBuffer>			m_lights_SBuffer;

		SceneRenderAgent m_sceneRenderAgent;

		std::unique_ptr<StructBuffer> m_debug_data_buffer;
	};

	inline void RenderManager::Present(bool _bVSync)
	{
		m_swapChain->Present(_bVSync ? 1 : 0, 0u);
	}

	inline std::shared_ptr<ehw::Texture> RenderManager::GetRenderTargetTex()
	{
		return m_renderTarget_texture;
	}

	inline std::shared_ptr<ehw::Texture> RenderManager::GetDepthStencilBufferTex()
	{
		return m_depth_stencil_buffer_texture;
	}
}


