#pragma once



#include "Engine/GPU/CommonGPU.h"
#include "Engine/CommonStruct.h"
#include "Engine/Util/SimpleMath.h"


#include "Engine/DefaultShader/Light/Func_Light.hlsli"

namespace ehw
{
	class ConstBuffer;
	class StructBuffer;
	class Texture;
	
	class Com_Camera;

	class iLight;
	class Com_Light3D;
	class GameObject;
	

	class MultiRenderTarget;

	class RenderManager
	{
		friend class GameEngine;
		friend class GPUManager;
	public:
		static void Render();

		static ConstBuffer* GetConstBuffer(eCBType _Type) { return mConstBuffers[(int)_Type].get(); }
		//static void SetDataToConstBuffer(eCBType _Type, void* _pData, uint _dataCount = 1u);

		static inline Com_Camera* GetMainCamera();
		static ComPtr<ID3D11RasterizerState>	GetRasterizerState(eRSType _Type) { return mRasterizerStates[(int)_Type]; }
		static ComPtr<ID3D11BlendState>		GetBlendState(eBSType _Type) { return mBlendStates[(int)_Type]; }
		static ComPtr<ID3D11DepthStencilState> GetDepthStencilState(eDSType _Type) { return mDepthStencilStates[(int)_Type]; }
		
		static void SetMainCamera(Com_Camera* const _pCam);
		static inline Com_Camera* GetCamera(size_t _Idx);

		static void RegisterCamera(Com_Camera* const _pCam);
		static void RemoveCamera(Com_Camera* const _pCam);

		inline static MultiRenderTarget* GetMultiRenderTarget(eMRTType _Type);

		//Renderer
		static void PushLightAttribute(const tLightAttribute& lightAttribute) { mLightAttributes.push_back(lightAttribute); }
		static inline void RegisterLight(Com_Light3D* const _pComLight);
		static void RemoveLight(Com_Light3D* const _pComLight);

		static const std::vector<Com_Light3D*>& GetLights() { return m_lights_3D; }


		static void BindLights();
		static void BindNoiseTexture();
		static void CopyRenderTarget();

		static void ClearMultiRenderTargets();

	private:
		static void UpdateGlobalCBuffer();

		//해상도 변경 시 GPUMgr에서 호출
		static bool SetResolution(UINT _ResolutionX, UINT _ResolutionY);

		static bool CreateMultiRenderTargets(UINT _ResolutionX, UINT _ResolutionY);
		static void SetTexturesToDefferedMaterials();

		static void LoadDefaultMesh();
		static void LoadDefaultMaterial();
		static void LoadDefaultShader();
		static void LoadDefaultTexture();


		static void CreateSamplerStates();
		static void CreateRasterizerStates();
		static void CreateDepthStencilStates();
		static void CreateBlendStates();
		
		static void CreateBuffer();

		static void Init();
		static void Release();

	private:
		static std::array<std::unique_ptr<ConstBuffer>, (int)eCBType::END>		mConstBuffers;
		static std::array<ComPtr<ID3D11SamplerState>, (int)eSamplerType::END>	mSamplerStates;
		static std::array<ComPtr<ID3D11RasterizerState>, (int)eRSType::END>		mRasterizerStates;
		static std::array<ComPtr<ID3D11DepthStencilState>, (int)eDSType::END>	mDepthStencilStates;
		static std::array<ComPtr<ID3D11BlendState>, (int)eBSType::END>			mBlendStates;
		
		static std::vector<Com_Camera*>			m_cameras;
		static size_t						m_mainCamIndex;

		static std::array<std::unique_ptr<MultiRenderTarget>, (int)eMRTType::END> mMultiRenderTargets;

		//생성자-소멸자에서 여기에 등록하기 떄문에 raw pointer를 사용해야 함
		static std::vector<Com_Light3D*>			m_lights_3D;
		static std::vector<tLightAttribute>			mLightAttributes;
		static std::unique_ptr<StructBuffer>		mLightsBuffer;

		static std::shared_ptr<Texture>				mPostProcessTexture;
		static std::shared_ptr<Texture>				mNoiseTex;
	};

	inline Com_Camera* RenderManager::GetMainCamera()
	{
		return GetCamera(m_mainCamIndex);
	}


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

	inline MultiRenderTarget* RenderManager::GetMultiRenderTarget(eMRTType _Type)
	{
		return mMultiRenderTargets[(int)_Type].get();
	}
}


