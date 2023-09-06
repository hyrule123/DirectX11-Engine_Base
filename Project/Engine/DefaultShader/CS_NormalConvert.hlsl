#include "SH_NormalConvert.hlsli"

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
	
	//color.r = ConvertColor(_dtID.xy, CB_UniformData.DestXYZSign.r, CB_UniformData.DestXYZOrder.r);
	//color.g = ConvertColor(_dtID.xy, CB_UniformData.DestXYZSign.g, CB_UniformData.DestXYZOrder.g);
	//color.b = ConvertColor(_dtID.xy, CB_UniformData.DestXYZSign.b, CB_UniformData.DestXYZOrder.b);
	
	//color.rgb = SrcNormalTex[_dtID.xy].rgb;
	
	//노말을 구해줌(-1.f ~ 1.f)
	normal.r = SrcNormalTex[_dtID.xy].r * 2.f - 1.f;
	
	//몬헌 노말은 -y Normal임
	normal.b = SrcNormalTex[_dtID.xy].g * 2.f - 1.f;
	normal.b = -normal.b;
	
	normal.g = 1.f - (pow(normal.r, 2.f) + pow(normal.b, 2.f));
	normal.g = sqrt(normal.g);
	
	//다시 Color 값으로 변환(0 ~ 1)
	normal.xyz += 1.f;
	normal.xyz *= 0.5f;
	normal.xyz = normalize(normal).xyz;
	
	normal.a = SrcNormalTex[_dtID.xy].a;

	DestNormalTex[_dtID.xy] = normal;
	
	
	//color.g = cross(color.r, color.b);
	
	//uint destX = (uint)round(CB_UniformData.DestXYZOrder.x);
	//if (CB_UniformData.DestXYZSign.x mad 0)
	//color.r= CB_UniformData.DestXYZSign.x * SrcNormalTex[DTid.xy][destX];
	
	//uint destY = (uint)round(CB_UniformData.DestXYZOrder.y);
	//color.g = CB_UniformData.DestXYZSign.y * SrcNormalTex[DTid.xy][destY];
	
	//uint destZ = (uint)round(CB_UniformData.DestXYZOrder.z);
	//color.b = CB_UniformData.DestXYZSign.z * SrcNormalTex[DTid.xy][destZ];
	
	//color.a = SrcNormalTex[_dtID.xy].a;
    
	//DestNormalTex[_dtID.xy] = color;
}
