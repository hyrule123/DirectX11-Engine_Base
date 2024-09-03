#include "Engine/Resource/Shader/ComputeShaders/GPUInitSetting.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/DefaultShader/Common_struct.hlsli"


#if defined (_WIN64) && !(NDEBUG)
#include "Engine/CompiledShaderHeader/CS_GPUInitSetting_x64_Debug.h"
#endif
#if defined(_WIN64) && (NDEBUG)
#include "Engine/CompiledShaderHeader/CS_GPUInitSetting_x64_Release.h"
#endif


namespace ehw
{
	GPUInitSetting::GPUInitSetting()
		: ComputeShader(ClassInfo<GPUInitSetting>::name(), uint3(1u, 1u, 1u))
	{
		SetEngineDefaultRes(true);
	}
	GPUInitSetting::~GPUInitSetting()
	{
	}

	eResult GPUInitSetting::Load(const std::fs::path& _baseDir, const std::fs::path& _key_path)
	{
		eResult result = CreateByHeader(CS_GPUInitSetting, sizeof(CS_GPUInitSetting));

		ASSERT(eResult_success(result), "GPU 초기화 작업 실패");

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
				//BOOL(1)을 넣어줬을 경우 둘 사이의 endian값이 다르면 byte order이 달라지므로 1이 아닌 0xffffffff를 사용한다
				gGPUInitSetting.bCPULittleEndian = 0xffffffff;
			}
		}

		//리틀 엔디안일 경우 앞 쪽에 1을 넣는다.
		if (gGPUInitSetting.bCPULittleEndian)
		{
			CheckEndianness check{};
			check.i8[0] = (UINT8)1;
			gGPUInitSetting.GPUEndianessTestVar = check.u32;
		}

		//빅 엔디안일 경우 뒷 쪽에 1을 넣는다
		else
		{
			CheckEndianness check{};
			check.i8[3] = (UINT8)1;
			gGPUInitSetting.GPUEndianessTestVar = check.u32;
		}

		gGPUInitSetting.debugVar.x = 0x12345678;

		//데이터 전송
		//SBuffer
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_WRITE;
		desc.REGISLOT_t_SRV = GPU::Register::t::gInitSetting;
		desc.REGISLOT_u_UAV = GPU::Register::u::g_initSettings_RW;
		mInitSBuffer = std::make_unique<StructBuffer>();
		eResult result = mInitSBuffer->Init<tGPUInitSetting>(desc, 1ui64, &gGPUInitSetting, 1ui64);
		ASSERT(eResult_success(result), "GPU 초기화용 구조화 버퍼 생성 실패.");

		mInitSBuffer->BindDataUAV();

		ComputeShader::CalculateGroupCount(uint3(1u, 1u, 1u));

		return true;
	}

	void GPUInitSetting::UnBindData()
	{
		mInitSBuffer->UnbindData();
		mInitSBuffer->GetData(&gGPUInitSetting);
		mInitSBuffer->BindDataSRV();
	}
}
