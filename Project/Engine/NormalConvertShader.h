#pragma once
#include "ComputeShader.h"
#include "Texture.h"



namespace ehw
{
	class NormalConvertShader : public ComputeShader
	{
	public:
		NormalConvertShader();
		virtual ~NormalConvertShader();

		virtual eResult Load(const std::fs::path& _filePath) override;

		std::shared_ptr<Texture> Convert(std::shared_ptr<Texture> _srcTex);

	protected:
		virtual bool BindData() override;
		virtual void UnBindData() override;

	private:
		std::shared_ptr<Texture> mSrcTex;
		std::shared_ptr<Texture> mDestTex;
	};
}
