#include "PCH_Engine.h"
#include "NormalConvertShader.h"

#include "DefaultShader/SH_Resource.hlsli"
#include "DefaultShader/SH_NormalConvert.hlsli"

#include "Texture.h"
#include "RenderMgr.h"
#include "ConstBuffer.h"

#ifdef _DEBUG
#include "CompiledShaderHeader/CS_NormalConvert_Debug.h"
#else
#include "CompiledShaderHeader/CS_NormalConvert_Release.h"
#endif

namespace mh
{
	NormalConvertShader::NormalConvertShader()
		: ComputeShader(uint3{32, 32, 1})
		, mSrcDesc()
		, mDestTex()
		
	{
		SetEngineDefaultRes(true);
	}

	NormalConvertShader::~NormalConvertShader()
	{
	}

	eResult NormalConvertShader::Load(const std::fs::path& _filePath)
	{
		IRes::Load(_filePath);

		return ComputeShader::CreateByHeader(CS_NormalConvert, sizeof(CS_NormalConvert));
	}

	std::shared_ptr<Texture> NormalConvertShader::Convert(const Desc& _desc)
	{
		if (nullptr == _desc.SrcTex)
			return nullptr;

		mSrcDesc = _desc;
		

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
		D3D11_TEXTURE2D_DESC texDesc = mSrcDesc.SrcTex->GetDesc();
		texDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;

		if (false == (mDestTex->Create(texDesc)))
			return false;

		//원본
		mSrcDesc.SrcTex->BindDataSRV(Register_t_SrcNormalTex, eShaderStageFlag::CS);

		//복사 대상
		mDestTex->BindDataUAV(Register_u_DestNormalTex);

		//데이터 수 계산
		ComputeShader::CalculateGroupCount(uint3{ mDestTex->GetWidth(), mDestTex->GetHeight(), 1u });

		//상수 버퍼에 추가 데이터 입력
		tCB_UniformData data{};
		
//#define DestXYZOrder float4_0
		data.DestXYZOrder = mSrcDesc.DestAxis;

//#define DestXYZSign float4_1
		data.DestXYZSign = mSrcDesc.DestSign;

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::UniformData);
		cb->SetData(&data);
		cb->BindData(eShaderStageFlag::CS);

		return true;
	}

	void NormalConvertShader::UnBindData()
	{
		mSrcDesc.SrcTex->UnBindData();
		mSrcDesc = {};
		
		mDestTex->UnBindData();
	}
}
