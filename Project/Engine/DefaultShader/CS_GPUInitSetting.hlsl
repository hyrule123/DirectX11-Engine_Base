#include "SH_Resource.hlsli"

SBUFFER_RW(InitSettingRW, tInitSetting, u, 0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (all(uint3(0u, 0u, 0u) != DTid))
		return;

	//초기화해야 하는 값이 있으면 여기서 초기화
	if (1u == InitSettingRW[0].bGPULittleEndian)
	{
		for (uint i = 0u; i < 4u; ++i)
		{
			uint shift = 8u * i;
			InitSettingRW[0].u8BitPartInU32Pack[i] = 0x000000FFu << shift;
			InitSettingRW[0].u8BitShiftInU32Pack[i] = shift;
		}
	}
	else
	{
		for (uint i = 0u; i < 4u; ++i)
		{
			uint shift = 8u * (3u - i);
			InitSettingRW[0].u8BitPartInU32Pack[i] = 0x000000FFu << shift;
			InitSettingRW[0].u8BitShiftInU32Pack[i] = shift;
		}
	}
}
