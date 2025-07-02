#ifndef NORMAL_CONVERT_HLSLI
#define NORMAL_CONVERT_HLSLI
#include <Engine/DefaultShader/Common_register.hlsli>
#include <Engine/DefaultShader/ConstBuffer.hlsli>

TEXTURE2D(SrcNormalTex, t, 0);
TEXTURE2D_RW(DestNormalTex, u, 1, float4);

#define TexWidth TotalDataCount.x
#define TexHeight TotalDataCount.y

#ifndef __cplusplus


#endif //C++


#endif
