#include "PCH_Engine.h"
#include "NormalMapEditor.h"

#include "DefaultShader/SH_NormalConvert.hlsli"

#include "Texture.h"

#ifdef _DEBUG
#include "CompiledShaderHeader/CS_NormalConvert_Debug.h"
#else
#include "CompiledShaderHeader/CS_NormalConvert_Release.h"
#endif

namespace mh
{
	NormalMapEditor::NormalMapEditor()
		: ComputeShader(uint3{32, 32, 1})
		, mSrcTex()
		, mDestTex()
	{
		SetEngineDefaultRes(true);
	}

	NormalMapEditor::~NormalMapEditor()
	{
	}

	eResult NormalMapEditor::Load(const std::fs::path& _filePath)
	{
		IRes::Load(_filePath);

		return ComputeShader::CreateByHeader(CS_NormalConvert, sizeof(CS_NormalConvert));
	}

	std::shared_ptr<Texture> NormalMapEditor::GetModifiedNormalMap(std::shared_ptr<Texture> _srcTex)
	{
		if (nullptr == _srcTex)
			return nullptr;

		mSrcTex = _srcTex;

		OnExcute();

		return mDestTex;
	}

	bool NormalMapEditor::BindData()
	{
		mDestTex = std::make_shared<Texture>();

		//UAV 속성 추가해서 생성
		D3D11_TEXTURE2D_DESC texDesc = mSrcTex->GetDesc();
		texDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;

		if (false == (mDestTex->Create(texDesc)))
			return false;

		mSrcTex->BindDataSRV(Register_t_SrcNormalTex, eShaderStageFlag::CS);
		mDestTex->BindDataUAV(Register_u_DestNormalTex);

		ComputeShader::CalculateGroupCount(uint3{ mDestTex->GetWidth(), mDestTex->GetHeight(), 1u });

		return true;
	}

	void NormalMapEditor::UnBindData()
	{
		mSrcTex->UnBindData();
		mSrcTex = nullptr;
		
		mDestTex->UnBindData();
		mDestTex = nullptr;
	}
}
