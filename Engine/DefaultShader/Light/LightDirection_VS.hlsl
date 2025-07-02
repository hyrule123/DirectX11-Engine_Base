#include <Engine/DefaultShader/Light/Light.hlsli>


//직사광선은 화면 전체를 덮으므로 
VSOut_LightDir main(VSIn_LightDir In)
{
	VSOut_LightDir Out = (VSOut_LightDir) 0.0f;
    
	//Rect Mesh는 -0.5~0.5 사이의 정사각형 -> 화면 전체를 덮게 하기 위해서는 *2를 해주면 된다.
	Out.Position = float4(In.Position.xy * 2.0f, 0.0f, 1.0f);
	Out.UV = In.UV;
	Out.instance_ID = In.instance_ID;
    
	return Out;
}
