#ifndef SH_FUNC_DECODE_COLOR
#define SH_FUNC_DECODE_COLOR
#include "SH_Resource.hlsli"

float4 DecodeColor(float _value)
{
	uint rgba = asint(_value);

	float r = float((rgba & 0xff000000) >> 24) / 255.f;
	float g = float((rgba & 0x00ff0000) >> 16) / 255.f;
	float b = float((rgba & 0x0000ff00) >> 8) / 255.f;
	float a = float((rgba & 0x000000ff) >> 0) / 255.f;

	return float4(r, g, b, a);
}

uint4 DecodeColorUint(uint _u32Src)
{
	uint4 result = (uint4) 0u;
	
	result.r = (_u32Src & gInitSetting[0].u8BitPartInU32Pack[0])
	>> gInitSetting[0].u8BitShiftInU32Pack[0];
	
	result.g = (_u32Src & gInitSetting[0].u8BitPartInU32Pack[1])
	>> gInitSetting[0].u8BitShiftInU32Pack[1];
	
	result.b = (_u32Src & gInitSetting[0].u8BitPartInU32Pack[2])
	>> gInitSetting[0].u8BitShiftInU32Pack[2];
	
	result.a = (_u32Src & gInitSetting[0].u8BitPartInU32Pack[3])
	>> gInitSetting[0].u8BitShiftInU32Pack[3];
	
	return result;
}
#endif
