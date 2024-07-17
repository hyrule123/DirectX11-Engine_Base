#pragma once
#include "Engine/GPU/CommonGPU.h"
#include "Engine/Common.h"
#include "Engine/Util/SimpleMath.h"
#include "Engine/Util/StaticSingleton.h"


#include "Engine/DefaultShader/Light/Func_Light.hlsli"

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
		friend class GPUManager;
	public:
		inline bool IsInitialized() { return m_isInitialized; }

		void Render();
		void FrameEnd();

		ConstBuffer* GetConstBuffer(eCBType _Type) { return m_constBuffers[(int)_Type].get(); }
		//void SetDataToConstBuffer(eCBType _Type, void* _pData, uint _dataCount = 1u);

		inline Com_Camera* GetMainCamera() { return GetCamera(m_mainCamIndex); }
		ID3D11RasterizerState*	GetRasterizerState(eRSType _Type) { return m_rasterizerStates[(int)_Type].Get(); }
		ID3D11BlendState*		GetBlendState(eBSType _Type) { return m_blendStates[(int)_Type].Get(); }
		ID3D11DepthStencilState* GetDepthStencilState(eDSType _Type) { return m_depthStencilStates[(int)_Type].Get(); }
		
		void SetMainCamera(Com_Camera* const _pCam);
		inline Com_Camera* GetCamera(size_t _Idx);

		void RegisterCamera(Com_Camera* const _pCam);
		void RemoveCamera(Com_Camera* const _pCam);

		inline void EnqueueRenderer(Renderer* const _renderer);
		inline const std::vector<Renderer*>& GetRenderers() { return m_renderers; }

		inline MultiRenderTarget* GetMultiRenderTarget(eMRTType _Type);

		//Renderer
		void PushLightAttribute(const tLightAttribute& lightAttribute) { m_lightAttributes.push_back(lightAttribute); }
		inline void RegisterLight(Com_Light3D* const _pComLight);
		void RemoveLight(Com_Light3D* const _pComLight);

		const std::vector<Com_Light3D*>& GetLights() { return m_lights_3D; }

		void BindLights();
		void BindNoiseTexture();
		void CopyRenderTarget();

		void ClearMultiRenderTargets();
		

		//void EraseIfDestroyed_Renderer();
		void EraseIfDestroyed_Camera(bool _callRender);

	private:
		RenderManager();
		~RenderManager();
		void Init();
		void Release();

		void UpdateGlobalCBuffer();

		//해상도 변경 시 GPUMgr에서 호출
		bool SetResolution(UINT _ResolutionX, UINT _ResolutionY);

		bool CreateMultiRenderTargets(UINT _ResolutionX, UINT _ResolutionY);
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
		std::array<std::unique_ptr<ConstBuffer>, (int)eCBType::END>		m_constBuffers;
		std::array<ComPtr<ID3D11SamplerState>, (int)eSamplerType::END>	m_samplerStates;
		std::array<ComPtr<ID3D11RasterizerState>, (int)eRSType::END>		m_rasterizerStates;
		std::array<ComPtr<ID3D11DepthStencilState>, (int)eDSType::END>	m_depthStencilStates;
		std::array<ComPtr<ID3D11BlendState>, (int)eBSType::END>			m_blendStates;
		
		std::vector<Com_Camera*>			m_cameras;
		size_t							m_mainCamIndex;

		std::vector<Renderer*>			m_renderers;

		std::array<std::unique_ptr<MultiRenderTarget>, (int)eMRTType::END> m_multiRenderTargets;

		//생성자-소멸자에서 여기에 등록하기 떄문에 raw pointer를 사용해야 함
		std::vector<Com_Light3D*>			m_lights_3D;
		std::vector<tLightAttribute>		m_lightAttributes;
		std::unique_ptr<StructBuffer>		m_lights_SBuffer;

		std::shared_ptr<Texture>				m_postProcessTexture;
		std::shared_ptr<Texture>				m_noiseTexture;

		bool m_isInitialized;
	};


	inline Com_Camera* RenderManager::GetCamera(size_t _Idx)
	{
		Com_Camera* pCam = nullptr;
		if (m_cameras.size() > (size_t)_Idx)
		{
			pCam = m_cameras[_Idx];
		}
			

		return pCam;
	}

	inline void RenderManager::RegisterLight(Com_Light3D* const _pComLight)
	{
		if (_pComLight)
		{
			m_lights_3D.push_back(_pComLight);
		}
	}

	inline void RenderManager::EnqueueRenderer(Renderer* const _renderer)
	{
		if (_renderer) { m_renderers.push_back(_renderer); }
	};

	inline MultiRenderTarget* RenderManager::GetMultiRenderTarget(eMRTType _Type)
	{
		return m_multiRenderTargets[(int)_Type].get();
	}
}


