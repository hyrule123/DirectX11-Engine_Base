
#include "NormalConvertShader.h"

#include <Engine/DefaultShader/Common_register.hlsli>
#include <Engine/DefaultShader/NormalConverter/NormalConverter.hlsli>

#if defined (_WIN64) && !(NDEBUG)
#include <Engine/CompiledShaderHeader/NormalConverter_CS_x64_Debug.h>
#endif
#if defined (_WIN64) && (NDEBUG)
#include <Engine/CompiledShaderHeader/NormalConverter_CS_x64_Release.h>
#endif

#include <Engine/Resource/Texture.h>

#include <Engine/Manager/RenderManager.h>

#include <Engine/GPU/ConstBuffer.h>


namespace core
{
	NormalConvertShader::NormalConvertShader()
		: ComputeShader(NormalConvertShader::s_static_type_name, uint3{32, 32, 1})
		, m_src_texture()
		, m_dest_texture()
		
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

	s_ptr<Texture> NormalConvertShader::convert(s_ptr<Texture> _srcTex)
	{
		if (nullptr == _srcTex)
			return nullptr;

		m_src_texture = _srcTex;
		
		on_execute();

		s_ptr<Texture> retTex = m_dest_texture;
		m_dest_texture = nullptr;
		return retTex;
	}

	bool NormalConvertShader::bind_buffer_to_GPU_register()
	{
		m_dest_texture = nullptr;
		m_dest_texture = std::make_shared<Texture>();

		//UAV 속성 추가해서 생성
		D3D11_TEXTURE2D_DESC texDesc = m_src_texture->get_texture2D_desc();
		texDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;

		if (false == (m_dest_texture->create(texDesc)))
		{
			m_dest_texture = nullptr;
			return false;
		}
		
		m_dest_texture->set_resource_name(m_src_texture->get_static_type_name());

		//원본
		m_src_texture->bind_buffer_as_SRV(GPU::Register::t::SrcNormalTex, eShaderStageFlag::Compute);

		//복사 대상
		m_dest_texture->bind_buffer_as_UAV(GPU::Register::u::DestNormalTex);

		//데이터 수 계산
		ComputeShader::calculate_group_count(uint3{ m_dest_texture->get_texture_width(), m_dest_texture->get_texture_height(), 1u });

		return true;
	}

	void NormalConvertShader::unbind_buffer_from_GPU_register()
	{
		m_src_texture->unbind_buffer_from_GPU_register();
		m_src_texture = nullptr;
		
		m_dest_texture->unbind_buffer_from_GPU_register();
	}
}
