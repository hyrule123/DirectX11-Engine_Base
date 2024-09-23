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

		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

	protected:
		virtual bool bind_buffer_to_GPU_register() override;
		virtual void unbind_buffer_from_GPU_register() override;

	private:
		std::unique_ptr<StructBuffer> mInitSBuffer;

		//define_Global 헤더에 있는 전역변수 사용중
	};
}


