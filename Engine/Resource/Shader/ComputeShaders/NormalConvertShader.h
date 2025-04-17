#pragma once
#include "Engine/Resource/Shader/ComputeShader.h"



#include "Engine/Resource/Texture.h"

namespace core
{
	class NormalConvertShader : public ComputeShader
	{
		CLASS_NAME(NormalConvertShader);
	public:
		NormalConvertShader();
		virtual ~NormalConvertShader();

		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		std::shared_ptr<Texture> Convert(std::shared_ptr<Texture> _srcTex);

	protected:
		virtual bool bind_buffer_to_GPU_register() override;
		virtual void unbind_buffer_from_GPU_register() override;

	private:
		std::shared_ptr<Texture> mSrcTex;
		std::shared_ptr<Texture> mDestTex;
	};
}
