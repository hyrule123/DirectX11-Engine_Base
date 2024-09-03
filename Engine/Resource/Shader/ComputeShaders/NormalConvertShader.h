#pragma once
#include "Engine/Resource/Shader/ComputeShader.h"



#include "Engine/Resource/Texture.h"

namespace ehw
{
	class NormalConvertShader : public ComputeShader
	{
		REGISTER_CLASS_INSTANCE_ABLE(NormalConvertShader);
	public:
		NormalConvertShader();
		virtual ~NormalConvertShader();

		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;

		std::shared_ptr<Texture> Convert(std::shared_ptr<Texture> _srcTex);

	protected:
		virtual bool BindData() override;
		virtual void UnBindData() override;

	private:
		std::shared_ptr<Texture> mSrcTex;
		std::shared_ptr<Texture> mDestTex;
	};
}
