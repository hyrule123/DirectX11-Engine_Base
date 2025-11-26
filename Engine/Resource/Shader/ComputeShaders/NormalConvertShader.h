#pragma once
#include <Engine/Resource/Shader/ComputeShader.h>



#include <Engine/Resource/Texture.h>

namespace core
{
	class NormalConvertShader : public ComputeShader
	{
		CLASS_INFO(NormalConvertShader, ComputeShader);
	public:
		NormalConvertShader();
		virtual ~NormalConvertShader();

		virtual eResult load(const std::fs::path& _relative_path) override;

		s_ptr<Texture> convert(s_ptr<Texture> _srcTex);

	protected:
		virtual bool bind() override;
		virtual void unbind_buffer_from_GPU_register() override;

	private:
		s_ptr<Texture> m_src_texture;
		s_ptr<Texture> m_dest_texture;
	};
}
