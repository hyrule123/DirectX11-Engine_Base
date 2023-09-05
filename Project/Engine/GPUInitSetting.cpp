#include "PCH_Engine.h"
#include "GPUInitSetting.h"

namespace mh
{
	GPUInitSetting::GPUInitSetting()
		: ComputeShader(uint3(1u, 1u, 1u))
	{
	}
	GPUInitSetting::~GPUInitSetting()
	{
	}

	bool GPUInitSetting::BindData()
	{
		union CheckEndianness
		{
			UINT32	i32;
			INT8	i8[4];
		};

		CheckEndianness check{};
		check.i32 = (UINT32)1;
		if (check.i8[0])
		{
			gInitSetting.bCPULittleEndian = TRUE;
		}

		ComputeShader::CalculateGroupCount(uint3(1u, 1u, 1u));

		mInitSBuffer = std::make_unique<StructBuffer>();
		

		return false;
	}

	void GPUInitSetting::UnBindData()
	{
	}


}
