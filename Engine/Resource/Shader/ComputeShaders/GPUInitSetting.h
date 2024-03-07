#pragma once
#include "Resource/Shader/iComputeShader.h"

#include "CommonGlobalVar.h"


namespace ehw
{
	class StructBuffer;
	class GPUInitSetting :
		public iComputeShader
	{
	public:
		GPUInitSetting();
		virtual ~GPUInitSetting();

		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;

	protected:
		virtual bool BindData() override;
		virtual void UnBindData() override;

	private:
		std::unique_ptr<StructBuffer> mInitSBuffer;

		//define_Global 헤더에 있는 전역변수 사용중
	};
}


