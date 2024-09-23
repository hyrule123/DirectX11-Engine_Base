#include "NormalConvertShader.h"



#include "Engine/DefaultShader/Common_register.hlsli"
#include "Engine/DefaultShader/NormalConverter/NormalConverter.hlsli"

#if defined (_WIN64) && !(NDEBUG)
#include "Engine/CompiledShaderHeader/NormalConverter_CS_x64_Debug.h"
#endif
#if defined (_WIN64) && (NDEBUG)
#include "Engine/CompiledShaderHeader/NormalConverter_CS_x64_Release.h"
#endif

#include "Engine/Resource/Texture.h"

#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/ConstBuffer.h"


namespace ehw
{
	NormalConvertShader::NormalConvertShader()
		: ComputeShader(NormalConvertShader::concrete_class_name, uint3{32, 32, 1})
		, mSrcTex()
		, mDestTex()
		
	{
		set_engine_default_res(true);
	}

	NormalConvertShader::~NormalConvertShader()
	{
	}

	eResult NormalConvertShader::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		return ComputeShader::compile_from_byte_code(NormalConverter_CS, sizeof(NormalConverter_CS));
	}

	std::shared_ptr<Texture> NormalConvertShader::Convert(std::shared_ptr<Texture> _srcTex)
	{
		if (nullptr == _srcTex)
			return nullptr;

		mSrcTex = _srcTex;
		
		on_execute();

		std::shared_ptr<Texture> retTex = mDestTex;
		mDestTex = nullptr;
		return retTex;
	}

	bool NormalConvertShader::bind_buffer_to_GPU_register()
	{
		mDestTex = nullptr;
		mDestTex = std::make_shared<Texture>();

		//UAV 속성 추가해서 생성
		D3D11_TEXTURE2D_DESC texDesc = mSrcTex->GetDesc();
		texDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;

		if (false == (mDestTex->create(texDesc)))
		{
			mDestTex = nullptr;
			return false;
		}
		
		mDestTex->set_resource_name(mSrcTex->get_concrete_class_name());

		//원본
		mSrcTex->bind_data_SRV(GPU::Register::t::SrcNormalTex, eShaderStageFlag::Compute);

		//복사 대상
		mDestTex->bind_buffer_to_UAV(GPU::Register::u::DestNormalTex);

		//데이터 수 계산
		ComputeShader::calculate_group_count(uint3{ mDestTex->GetWidth(), mDestTex->GetHeight(), 1u });

		return true;
	}

	void NormalConvertShader::unbind_buffer_from_GPU_register()
	{
		mSrcTex->unbind_buffer_from_GPU_register();
		mSrcTex = nullptr;
		
		mDestTex->unbind_buffer_from_GPU_register();
	}
}
