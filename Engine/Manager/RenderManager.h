#pragma once
#include "Engine/Common.h"
#include "Engine/GPU/CommonGPU.h"
#include "Engine/Util/SimpleMath.h"
#include "Engine/Util/StaticSingleton.h"

#include "Engine/DefaultShader/Light/Func_Light.hlsli"

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
		friend class GPUManager;
	public:
		inline bool IsInitialized() { return m_isInitialized; }

		void Render();

		SceneRenderAgent& sceneRenderAgent() { return m_sceneRenderAgent; }

		ConstBuffer* GetConstBuffer(eCBType _Type) { return m_constBuffers[(int)_Type].get(); }
		//void SetDataToConstBuffer(eCBType _Type, void* _pData, uint _dataCount = 1u);

		ID3D11RasterizerState* GetRasterizerState(eRSType _Type) { return m_rasterizerStates[(int)_Type].Get(); }
		ID3D11BlendState* GetBlendState(eBSType _Type) { return m_blendStates[(int)_Type].Get(); }
		ID3D11DepthStencilState* GetDepthStencilState(eDSType _Type) { return m_depthStencilStates[(int)_Type].Get(); }

		inline MultiRenderTarget* GetMultiRenderTarget(eMRTType _Type);


		void BindNoiseTexture();
		void CopyRenderTarget();

		void ClearMultiRenderTargets();

		StructBuffer* GetLightSBuffer() { return m_lights_SBuffer.get(); }

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
		std::array<ComPtr<ID3D11RasterizerState>, (int)eRSType::END>	m_rasterizerStates;
		std::array<ComPtr<ID3D11DepthStencilState>, (int)eDSType::END>	m_depthStencilStates;
		std::array<ComPtr<ID3D11BlendState>, (int)eBSType::END>			m_blendStates;

		std::array<std::unique_ptr<MultiRenderTarget>, (int)eMRTType::END> m_multiRenderTargets;

		std::shared_ptr<Texture>				m_postProcessTexture;
		std::shared_ptr<Texture>				m_noiseTexture;
		std::unique_ptr<StructBuffer>			m_lights_SBuffer;

		SceneRenderAgent m_sceneRenderAgent;

		bool m_isInitialized;
	};

	inline MultiRenderTarget* RenderManager::GetMultiRenderTarget(eMRTType _Type)
	{
		return m_multiRenderTargets[(int)_Type].get();
	}
}


