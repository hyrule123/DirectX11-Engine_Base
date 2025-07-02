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

		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		s_ptr<Texture> Convert(s_ptr<Texture> _srcTex);

	protected:
		virtual bool bind_buffer_to_GPU_register() override;
		virtual void unbind_buffer_from_GPU_register() override;

	private:
		s_ptr<Texture> mSrcTex;
		s_ptr<Texture> mDestTex;
	};
}
