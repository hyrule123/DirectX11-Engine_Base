#include "Engine/DefaultShader/NormalConverter/NormalConverter.hlsli"

float ConvertColor(uint2 _texPos, float _signFactor, float _indexFactor)
{
	float retColor = SrcNormalTex[_texPos][(uint)round(_indexFactor)];
	if(_signFactor < 0.f)
	{
		retColor = 1.f - retColor;
	}
	return saturate(retColor);
}

//  SV_GroupID  : 스레드에 속한 그룹의 좌표
//  SV_GroupThreadID : 그룹 내에서, 스레드의 좌표
//  SV_GroupIndex : 그룹 내에서, 스레드의 인덱스 좌표(1차원)
//  SV_DispatchThreadID : 전체 스레드기준으로 호출된 좌표

[numthreads(32, 32, 1)] // 그룹당 쓰레드 개수 ( 최대 1024개까지 지정가능 )
void main( uint3 _dtID : SV_DispatchThreadID )
{
	if (CB_ComputeShader.TexWidth < _dtID.x || CB_ComputeShader.TexHeight < _dtID.y)
		return;

	float4 normal = (float4) 0;
	
	//X Normal
	normal.x = SrcNormalTex[_dtID.xy].r * 2.f - 1.f;
	
	//Y Normal
	normal.y = SrcNormalTex[_dtID.xy].g * 2.f - 1.f;
	normal.y = -normal.y;
	
	//Z Normal
	normal.z = saturate(1.f - normal.x * normal.x - normal.y * normal.y);
	normal.z = sqrt(normal.z);
	
	normal.xyz *= 0.5f;
	normal.xyz += 0.5f;
	
	normal.a = SrcNormalTex[_dtID.xy].a;

	DestNormalTex[_dtID.xy] = normal;
}
