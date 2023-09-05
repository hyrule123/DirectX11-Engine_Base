#include "PCH_Engine.h"
#include "GPUInitSetting.h"
#include "StructBuffer.h"

#include "CompiledShaderHeader/CS_GPUInitSetting_Debug.h"

namespace mh
{
	GPUInitSetting::GPUInitSetting()
		: ComputeShader(uint3(1u, 1u, 1u))
	{
		SetEngineDefaultRes(true);
	}
	GPUInitSetting::~GPUInitSetting()
	{
	}

	eResult GPUInitSetting::Load(const std::filesystem::path& _filePath)
	{
		IRes::Load(_filePath);

		eResult result = CreateByHeader(CS_GPUInitSetting, sizeof(CS_GPUInitSetting));
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"로드 실패");
		}

		return result;
	}

	bool GPUInitSetting::BindData()
	{
		union CheckEndianness
		{
			UINT32	u32;
			INT8	i8[4];
		};

		{
			CheckEndianness check{};
			check.u32 = (UINT32)1;
			if (check.i8[0])
			{
				gGPUInitSetting.bCPULittleEndian = TRUE;
			}
		}

		
		if (TRUE == gGPUInitSetting.bCPULittleEndian)
		{
			CheckEndianness check{};
			check.i8[0] = (UINT8)1;
			gGPUInitSetting.GPUEndianessTestVar = check.u32;
		}
		else
		{
			CheckEndianness check{};
			check.i8[3] = (UINT8)1;
			gGPUInitSetting.GPUEndianessTestVar = check.u32;
		}


		//SBuffer
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_WRITE;
		desc.REGISLOT_t_SRV = Register_t_gInitSetting;
		desc.REGISLOT_u_UAV = Register_u_gInitSettingRW;
		mInitSBuffer = std::make_unique<StructBuffer>(desc);
		if (FAILED(mInitSBuffer->Create<tGPUInitSetting>(1ui64, &gGPUInitSetting, 1ui64)))
		{
			return false;
		}
		mInitSBuffer->BindDataUAV();

		ComputeShader::CalculateGroupCount(uint3(1u, 1u, 1u));

		return true;
	}

	void GPUInitSetting::UnBindData()
	{
		mInitSBuffer->UnBindData();
		mInitSBuffer->GetData(&gGPUInitSetting);
		mInitSBuffer->BindDataSRV();
	}
}
