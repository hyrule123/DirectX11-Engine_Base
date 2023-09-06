#pragma once
#include "ComputeShader.h"
#include "Texture.h"



namespace mh
{
	class NormalConvertShader : public ComputeShader
	{
	public:
		NormalConvertShader();
		virtual ~NormalConvertShader();

		virtual eResult Load(const std::fs::path& _filePath) override;

		struct Desc
		{
			std::shared_ptr<Texture> SrcTex;
			float4 DestAxis{};
			float4 DestSign{};
		};
		std::shared_ptr<Texture> Convert(const Desc& _desc);



	protected:
		virtual bool BindData() override;
		virtual void UnBindData() override;

	private:
		Desc mSrcDesc;
		std::shared_ptr<Texture> mDestTex;
	};
}
