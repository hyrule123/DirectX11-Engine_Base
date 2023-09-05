#ifndef SH_NORMAL_CONVERT
#define SH_NORMAL_CONVERT
#include "SH_CommonStruct.hlsli"

TEXTURE2D(SrcNormalTex, t, 0);
TEXTURE2D_RW(DestNormalTex, u, 0, float4);


#define SrcXYZOrder float4_0
#define SrcXYZSign float4_1

#define DestXYZOrder float4_2
#define DestXYZSign float4_3

#define TexSizeX int_0
#define TexSizeY int_1

#ifndef __cplusplus


#endif //C++


#endif
