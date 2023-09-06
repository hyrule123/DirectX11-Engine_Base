#ifndef SH_NORMAL_CONVERT
#define SH_NORMAL_CONVERT
#include "SH_Resource.hlsli"
#include "SH_ConstBuffer.hlsli"

TEXTURE2D(SrcNormalTex, t, 0);
TEXTURE2D_RW(DestNormalTex, u, 0, float4);

#define SrcXYZOrder float4_0
#define SrcXYZSign float4_1

#define TexWidth TotalDataCount.x
#define TexHeight TotalDataCount.y

#ifndef __cplusplus


#endif //C++


#endif
