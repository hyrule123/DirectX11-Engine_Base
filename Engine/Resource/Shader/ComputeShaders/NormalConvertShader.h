#pragma once
#include "Engine/Resource/Shader/ComputeShader.h"



#include "Engine/Resource/Texture.h"

namespace ehw
{
	class NormalConvertShader : public ComputeShader
	{
		CLASS_NAME(NormalConvertShader);
	public:
		NormalConvertShader();
		virtual ~NormalConvertShader();

		virtual eResult load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;

		std::shared_ptr<Texture> Convert(std::shared_ptr<Texture> _srcTex);

	protected:
		virtual bool bind_data() override;
		virtual void UnBindData() override;

	private:
		std::shared_ptr<Texture> mSrcTex;
		std::shared_ptr<Texture> mDestTex;
	};
}
