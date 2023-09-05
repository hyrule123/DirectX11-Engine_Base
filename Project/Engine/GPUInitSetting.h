#pragma once
#include "ComputeShader.h"
#include "define_Global.h"

namespace mh
{
	class StructBuffer;
	class GPUInitSetting :
		public ComputeShader
	{
	public:
		GPUInitSetting();
		virtual ~GPUInitSetting();


		virtual bool BindData() override;
		virtual void UnBindData() override;

	private:
		std::unique_ptr<StructBuffer> mInitSBuffer;

		//define_Global 헤더에 있는 전역변수 사용중
	};
}


