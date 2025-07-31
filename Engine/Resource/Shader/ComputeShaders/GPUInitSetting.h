#pragma once
#include <Engine/Resource/Shader/ComputeShader.h>

#include <Engine/Global.h>

namespace core
{
	class StructBuffer;
	class GPUInitSetting :
		public ComputeShader
	{
		CLASS_INFO(GPUInitSetting, ComputeShader);
	public:
		GPUInitSetting();
		virtual ~GPUInitSetting();

		virtual eResult load(const std::fs::path& _base_directory) override;

	protected:
		virtual bool bind_buffer_to_GPU_register() override;
		virtual void unbind_buffer_from_GPU_register() override;

	private:
		std::unique_ptr<StructBuffer> m_init_sbuffer;

		//define_Global 헤더에 있는 전역변수 사용중
	};
}


