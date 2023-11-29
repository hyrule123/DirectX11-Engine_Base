#ifndef S_H_FUNC_BITWISE
#define S_H_FUNC_BITWISE

#include "Resource.hlsli"

//==========================================
//  한 변수를 둘로 나누는 작업 시에는 반드시 엔디안을 고려할것
//  이 함수를 사용하는 경우 : 여러 개의 데이터를 뭉쳐서 전달하는 경우(UINT8 4개 배열을 uint로 전달하는 경우)
//==========================================

//CPU와 GPU의 엔디안 형태가 다를 경우 엔디안을 변환
inline uint MatchEndianness(uint _src)
{
	uint result = 0u;
	
	if (gInitSetting[0].bCPULittleEndian != gInitSetting[0].bGPULittleEndian)
	{
		result =
		((_src >> 24) & 0x000000ff) | //0xff000000을 24만큼 오른쪽으로 민다음(0x000000ff) 0x000000ff와 &연산
		((_src << 24) & 0xff000000) | //0x000000ff를 24만큼 왼쪽으로 민다음(0xff000000) 0xff000000과 &연산
		((_src >> 8) & 0x0000ff00) | //0x00ff0000을 8만큼 오른쪽으로 민다음 0x0000ff00와 &연산
		((_src << 8) & 0x00ff0000); //0x0000ff00을 8만큼 왼쪽으로 민다음 0x00ff0000과 &연산
	}
	else
	{
		result = _src;
	}
	
	return result;
}


//_uIdxinU16 범위 : 0 ~ 7
//UINT16 8개를 UINT32_4에 넣어놓은 데이터에서 특정 인덱스의 UINT16을 하나 꺼내준다
uint UnpackUINT16FromUINT32_4(in uint4 _u3_42Src, uint _uIdxinU16)
{
	uint result = 0u;
	
	//uint4의 몇번째 값인지를 계산(비트를 1만큼 우측으로 밀어주면 /2 와 같은 결과이다)
	//UINT16 변수 8개를 배열로 넣은 것이므로 여기서는 endianess와 상관 없음
	uint Idx = (_uIdxinU16 >> 1u) & 0x07;
	
	//둘의 Endianness가 다를 경우에는 endianness를 변환해서 일치시킨다.
	result = MatchEndianness(_u3_42Src[Idx]);
	
	//비트의 앞쪽인지 뒤쪽인지를 계산. 2로 나눈 나머지를 구해준다(1과 비트 &연산을 하면 나머지를 구할수 있음)
	//0 = 0xffff0000, 1 = 0x0000ffff
	uint FrontOrBack = _uIdxinU16 & 1u;
	
	//Endianness를 고려해서 앞쪽/뒷쪽의 값을 가져온다.
	result &= gInitSetting[0].u16BitPartInU32Pack[FrontOrBack];
	result >>= gInitSetting[0].u16BitShiftInU32Pack[FrontOrBack];
	
	return result;
}

//_uIdxInU16 범위 : 0 ~ 15
uint UnpackUINT16FromUINT32_8(in uintx8 _u32_8Src, uint _uIdxInU16)
{
	uint result = 0u;
	
	//row == 8로 나눈 결과, 0 또는 1
	uint row = (_uIdxInU16 >> 3u) & 0x1;
	
	//col == 8로 나눈 나머지를 2로 나눠준다.(비트 연산)
	uint col = (_uIdxInU16 & 7u) >> 1u;
	
	//앞부분인지 뒷부분인지 확인(2로 나눈 나머지)
	uint FrontOrBack = _uIdxInU16 & 0x1;
	
	result = MatchEndianness(_u32_8Src[row][col]);
	
	result &= gInitSetting[0].u16BitPartInU32Pack[FrontOrBack];
	result >>= gInitSetting[0].u16BitShiftInU32Pack[FrontOrBack];
	
	return result;
}

//_uIdxInU16 범위 : 0 ~ 3
uint UnpackUINT8FromUINT32(in uint _u32Src, uint _uIdxInU8)
{
	uint result = 0u;
	
	//0~3 까지의 범위로 한정
	_uIdxInU8 &= 0x03;
	
	result = MatchEndianness(_u32Src);
	
	result &= gInitSetting[0].u8BitPartInU32Pack[_uIdxInU8];
	result >>= gInitSetting[0].u8BitShiftInU32Pack[_uIdxInU8];
	
	return result;
}


uint4 UnpackUINT8FromUINT32_ALL(in uint _u32Src)
{
	uint4 result = (uint4) 0u;

	uint matched = MatchEndianness(_u32Src);

	result.r = (matched & gInitSetting[0].u8BitPartInU32Pack[0])
		>> gInitSetting[0].u8BitShiftInU32Pack[0];
	
	result.g = (matched & gInitSetting[0].u8BitPartInU32Pack[1])
		>> gInitSetting[0].u8BitShiftInU32Pack[1];
	
	result.b = (matched & gInitSetting[0].u8BitPartInU32Pack[2])
		>> gInitSetting[0].u8BitShiftInU32Pack[2];
	
	result.a = (matched & gInitSetting[0].u8BitPartInU32Pack[3])
		>> gInitSetting[0].u8BitShiftInU32Pack[3];
	
	return result;
}

uint2 UnpackUINT16FromUINT32_ALL(in uint _u32Src)
{
	uint2 result = (uint2) 0u;
	
	uint matched = MatchEndianness(_u32Src);

	result.x = (matched & gInitSetting[0].u16BitPartInU32Pack[0]) >> gInitSetting[0].u16BitShiftInU32Pack[0];
	
	result.y = (matched & gInitSetting[0].u16BitPartInU32Pack[1]) >> gInitSetting[0].u16BitShiftInU32Pack[1];
	
	return result;
}



#endif
