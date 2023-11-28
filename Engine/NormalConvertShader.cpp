#include "PCH_Engine.h"
#include "NormalConvertShader.h"

#include "DefaultShader/Resource.hlsli"
#include "DefaultShader/NormalConverter.hlsli"

#include "Texture.h"
#include "RenderMgr.h"
#include "ConstBuffer.h"

#ifdef _DEBUG
#include "CompiledShaderHeader/CS_NormalConvertMH_Debug.h"
#else
#include "CompiledShaderHeader/CS_NormalConvertMH_Release.h"
#endif

namespace ehw
{
	NormalConvertShader::NormalConvertShader()
		: ComputeShader(uint3{32, 32, 1})
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
		IResource::Load(_filePath);

		return ComputeShader::CreateByHeader(CS_NormalConvertMH, sizeof(CS_NormalConvertMH));
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
		
		mDestTex->SetKey(mSrcTex->GetKey());

		//원본
		mSrcTex->BindDataSRV(Register_t_SrcNormalTex, eShaderStageFlag::CS);

		//복사 대상
		mDestTex->BindDataUAV(Register_u_DestNormalTex);

		//데이터 수 계산
		ComputeShader::CalculateGroupCount(uint3{ mDestTex->GetWidth(), mDestTex->GetHeight(), 1u });

		return true;
	}

	void NormalConvertShader::UnBindData()
	{
		mSrcTex->UnBindData();
		mSrcTex = nullptr;
		
		mDestTex->UnBindData();
	}
}
