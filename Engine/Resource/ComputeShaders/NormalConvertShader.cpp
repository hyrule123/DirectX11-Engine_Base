#include "../../PCH_Engine.h"
#include "../../Resource/ComputeShaders/NormalConvertShader.h"



#include "../../DefaultShader/Resource.hlsli"
#include "../../DefaultShader/NormalConverter/NormalConverter.hlsli"

#if defined (_WIN64) && !(NDEBUG)
#include "../../CompiledShaderHeader/CS_NormalConverter_x64_Debug.h"
#endif
#if defined (_WIN64) && (NDEBUG)
#include "../../CompiledShaderHeader/CS_NormalConverter_x64_Releaseh"
#endif

#include "../../Resource/Texture.h"

#include "../../Manager/RenderManager.h"

#include "../../GPU/ConstBuffer.h"


namespace ehw
{
	NormalConvertShader::NormalConvertShader()
		: iComputeShader(typeid(NormalConvertShader), uint3{32, 32, 1})
		, mSrcTex()
		, mDestTex()
		
	{
		SetEngineDefaultRes(true);
	}

	NormalConvertShader::~NormalConvertShader()
	{
	}

	eResult NormalConvertShader::Load(const std::fs::path& _filePath)
	{
		iResource::Load(_filePath);

		return iComputeShader::CreateByHeader(CS_NormalConverter, sizeof(CS_NormalConverter));
	}

	std::shared_ptr<Texture> NormalConvertShader::Convert(std::shared_ptr<Texture> _srcTex)
	{
		if (nullptr == _srcTex)
			return nullptr;

		mSrcTex = _srcTex;
		
		OnExcute();

		std::shared_ptr<Texture> retTex = mDestTex;
		mDestTex = nullptr;
		return retTex;
	}

	bool NormalConvertShader::BindData()
	{
		mDestTex = nullptr;
		mDestTex = std::make_shared<Texture>();

		//UAV 속성 추가해서 생성
		D3D11_TEXTURE2D_DESC texDesc = mSrcTex->GetDesc();
		texDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;

		if (false == (mDestTex->Create(texDesc)))
		{
			mDestTex = nullptr;
			return false;
		}
		
		mDestTex->SetStrKey(mSrcTex->GetStrKey());

		//원본
		mSrcTex->BindDataSRV(Register_t_SrcNormalTex, eShaderStageFlag::Compute);

		//복사 대상
		mDestTex->BindDataUAV(Register_u_DestNormalTex);

		//데이터 수 계산
		iComputeShader::CalculateGroupCount(uint3{ mDestTex->GetWidth(), mDestTex->GetHeight(), 1u });

		return true;
	}

	void NormalConvertShader::UnBindData()
	{
		mSrcTex->UnBindData();
		mSrcTex = nullptr;
		
		mDestTex->UnBindData();
	}
}
