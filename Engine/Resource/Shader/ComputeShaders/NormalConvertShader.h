#pragma once
#include "Resource/Shader/iComputeShader.h"



#include "Resource/Texture.h"

namespace ehw
{
	class NormalConvertShader : public iComputeShader
	{
	public:
		NormalConvertShader();
		virtual ~NormalConvertShader();

		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;

		std::shared_ptr<Texture> Convert(std::shared_ptr<Texture> _srcTex);

	protected:
		virtual bool BindData() override;
		virtual void UnBindData() override;

	private:
		std::shared_ptr<Texture> mSrcTex;
		std::shared_ptr<Texture> mDestTex;
	};
}
