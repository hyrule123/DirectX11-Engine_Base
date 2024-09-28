#ifndef FUNC_LIGHT_HLSLI
#define FUNC_LIGHT_HLSLI


#ifndef __cplusplus//HLSL
#include "Engine/DefaultShader/Light/Light.hlsli"

//2D
void CalculateLight2D(in out tLightColor pLightColor, float3 position, int idx)
{
	if (0 == g_light_attributes[idx].lightType)
	{
		pLightColor.diffuse += g_light_attributes[idx].color.diffuse;
	}
	else if (1 == g_light_attributes[idx].lightType)
	{
		float length = distance(g_light_attributes[idx].position.xy, position.xy);
        
		if (length < g_light_attributes[idx].radius)
		{
			float ratio = 1.0f - (length / g_light_attributes[idx].radius);
			pLightColor.diffuse += g_light_attributes[idx].color.diffuse * ratio;
		}
        
	}
	else
	{
        
	}
}

//3D
tLightColor calculate_light_3D(
in tLightAttribute _light_attr
, in float3 _view_position
, in float3 _view_normal)
{
	tLightColor ret = (tLightColor) 0;
    
    //view space 상에서 빛의 세기를 구함
	float3 light_direction = (float3) 0.0f;
	
	float fDiffPow = 0.f;//전체 사용
	float fDistPow = 1.f;//PointLight, SpotLight에 사용(거리에 따른 빛의 세기)
	float fAnglePow = 1.f;//SpotLight에 사용
    
    // Directional
	if (LIGHT_TYPE_DIRECTIONAL == _light_attr.lightType)
	{
        // Light 의 ViewSpace 에서의 '방향'(w값 0)
		light_direction = normalize(mul(float4(_light_attr.direction.xyz, 0.f), g_CB_camera.view)).xyz;
	}
    // point
	else if (LIGHT_TYPE_POINT == _light_attr.lightType)
	{
		// Light 의 ViewSpace 에서의 '위치'(w값 1)
		float4 LightViewPos = mul(float4(_light_attr.position.xyz, 1.f), g_CB_camera.view);
       
        // 표면위치 - 광원 위치 하면 빛의 방향 벡터를 구할 수 있음.
		light_direction = _view_position - LightViewPos.xyz;
        
        // 거리에 따른 세기 변화를 계산하고
		float light_surface_dist = length(light_direction);
		
		//방향 벡터는 정규화 해준다.
		light_direction = normalize(light_direction);
		
		//노멀 <-> 광원 사이의 거리 / 빛의 범위 한 값을 0~1 사이로 제한
		fDistPow = 1.f - saturate(light_surface_dist / _light_attr.radius);
	}
	//SpotLight
	else if (LIGHT_TYPE_SPOT == _light_attr.lightType)
	{
	}
	
	//빛과 노멀 벡터 사이의 cos값
	//만약 빛이 표면에 닿았다면 노멀 벡터와 빛 벡터는 서로 반대 방향을 보고 있을 것이다.
	//여기서는 각도를 구해야 하므로 빛 벡터를 노멀 벡터와 같은 방향으로 정하고 
	//내적을 통해 사잇각을 구한다.(두 벡터는 모두 normalize 되어 있음.)
	float cosLight = dot(-light_direction, _view_normal);
	
	// Diffuse Power
	fDiffPow = saturate(cosLight);
    
    // Specular 계산
	float3 vViewReflect = normalize(light_direction + 2.f * cosLight * _view_normal);
    
    // 카메라에서 픽셀 지점을 바라보는 시선 벡터
	float3 vEye = -normalize(_view_position);
    
    // 반사광 세기          
	float fRelfectPow = pow(saturate(dot(vViewReflect, vEye)), 10);
	
	ret.diffuse = _light_attr.color.diffuse * fDiffPow * fDistPow;
	ret.specular = _light_attr.color.specular * fRelfectPow * fDistPow;
	ret.ambient = _light_attr.color.ambient;
	
	return ret;
}

/*
tLightColor test2(tLightAttribute _attr, float3 _view_pos, float3 _view_normal)
{
	tLightColor color = (tLightColor) 0;
       
	float3 ViewLightDir = (float3) 0.f;
	float fDiffPow = (float) 0.f;
	float fSpecPow = (float) 0.f;
    
    // Directional Light
	if (_attr.lightType == 0)
	{
		ViewLightDir = normalize(mul(float4(_attr.direction.xyz, 0.f), g_CB_camera.view));
    
        // View 스페이스 상에서 표면의 빛의 세기를 구함
		fDiffPow = saturate(dot(-ViewLightDir, _view_normal));
    
        // 반사광 세기를 구함
        // 표면의 빛의 반사벡터       
		float3 vViewReflect = normalize(ViewLightDir + 2.f * dot(-ViewLightDir, _view_normal) * _view_normal);
        
        // 시점에서 표면을 향하는 벡터
		float3 vEye = normalize(_view_pos);
        
        // 시선 벡터와 반사벡터를 내적해서 반사광의 세기를 구함
		fSpecPow = saturate(dot(-vEye, vViewReflect));
		fSpecPow = pow(fSpecPow, 20);
	}
    
    // Point Light
	else if (_attr.lightType == 1)
	{
        // ViewSpace 상에서 광원의 위치를 알아낸다.
		float3 vLightViewPos = mul(float4(_attr.position.xyz, 1.f), g_CB_camera.view);
        
        // 광원의 위치에서 표면을 향하는 벡터를 구한다.
		ViewLightDir = _view_pos - vLightViewPos;
        
        // 광원에서 표면까지의 거리를 구한다.
		float fDist = length(ViewLightDir);
        
        // 광원에서 표면을 향하는 단위벡터를 구한다.        
		ViewLightDir = normalize(ViewLightDir);
        
        // 반경대비 거리에 따른 빛의 세기 비율
        //float fRatio = saturate(1.f - (fDist / lightinfo.fRadius));        
		float fRatio = cos(saturate(fDist / _attr.radius) * 3.1415926535f * 0.5f);
                        
        // View 스페이스 상에서 표면의 빛의 세기를 구함        
		fDiffPow = saturate(dot(-ViewLightDir, _view_normal)) * fRatio;

        // 반사광 세기를 구함
        // 표면의 빛의 반사벡터       
		float3 vViewReflect = normalize(ViewLightDir + 2.f * dot(-ViewLightDir, _view_normal) * _view_normal);
        
        // 시점에서 표면을 향하는 벡터
		float3 vEye = normalize(_view_pos);
        
        // 시선 벡터와 반사벡터를 내적해서 반사광의 세기를 구함
		fSpecPow = saturate(dot(-vEye, vViewReflect));
		fSpecPow = pow(fSpecPow, 20);
	}
    
    // SpotLight
	else
	{
        
	}
    
    // 최종 난반사광
	color.diffuse += _attr.color.diffuse * fDiffPow;
    
    // 최종 반사광 
	color.specular += _attr.color.specular * fSpecPow;

    // 최종 환경광
	color.ambient += _attr.color.ambient;
	
	return color;
}
*/
#endif//HLSL
#endif
