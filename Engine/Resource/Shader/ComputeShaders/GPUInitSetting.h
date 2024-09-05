#pragma once
#include "Engine/Resource/Shader/ComputeShader.h"

#include "Engine/GlobalVariable.h"

namespace ehw
{
	class StructBuffer;
	class GPUInitSetting :
		public ComputeShader
	{
		CLASS_NAME(GPUInitSetting);
	public:
		GPUInitSetting();
		virtual ~GPUInitSetting();

		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;

	protected:
		virtual bool bind_data() override;
		virtual void UnBindData() override;

	private:
		std::unique_ptr<StructBuffer> mInitSBuffer;

		//define_Global 헤더에 있는 전역변수 사용중
	};
}


