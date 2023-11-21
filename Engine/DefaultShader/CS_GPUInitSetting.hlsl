#include "SH_Resource.hlsli"


//초기화해야 하는 값이 있으면 여기서 초기화
//현재 하는 작업은 Endianness 확인 하나임.
[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (all(uint3(0u, 0u, 0u) != DTid))
		return;
	
	if(TRUE == gInitSettingRW[0].bCPULittleEndian)
	{
		if (gInitSettingRW[0].GPUEndianessTestVar & 0x000000ff)
		{
			gInitSettingRW[0].bGPULittleEndian = TRUE;
		}
	}
	else
	{
		if (gInitSettingRW[0].GPUEndianessTestVar & 0xff000000)
		{
			gInitSettingRW[0].bGPULittleEndian = TRUE;
		}
	}

	
	if (TRUE == gInitSettingRW[0].bGPULittleEndian)
	{
		for (uint i = 0u; i < 4u; ++i)
		{
			uint shift = 8u * i;
			gInitSettingRW[0].u8BitPartInU32Pack[i] = 0x000000FFu << shift;
			gInitSettingRW[0].u8BitShiftInU32Pack[i] = shift;
		}
	}
	else
	{
		for (uint i = 0u; i < 4u; ++i)
		{
			uint shift = 8u * (3u - i);
			gInitSettingRW[0].u8BitPartInU32Pack[i] = 0x000000FFu << shift;
			gInitSettingRW[0].u8BitShiftInU32Pack[i] = shift;
		}
	}
}
