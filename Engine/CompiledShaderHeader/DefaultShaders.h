
//컴파일된 쉐이더 헤더 모아놓은 헤더
#ifdef _WIN64

#ifdef _DEBUG
	
#include <Engine/CompiledShaderHeader/Forward3D_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Forward3D_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/Debug_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Debug_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/Grid_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Grid_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/Particle_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Particle_GS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Particle_PS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Particle_CS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/PostProcess_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/PostProcess_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/Sprite_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Sprite_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/UI_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/UI_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/Deffered3D_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Deffered3D_PS_x64_Debug.h>

//#include <Engine/CompiledShaderHeader/Light_VS_x64_Debug.h>
//#include <Engine/CompiledShaderHeader/Light_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/LightDirection_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/LightDirection_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/LightPoint_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/LightPoint_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/Deffered_Merger_VS_x64_Debug.h>
#include <Engine/CompiledShaderHeader/Deffered_Merger_PS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/Animation3D_CS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/GPU_Init_Setting_CS_x64_Debug.h>

#include <Engine/CompiledShaderHeader/NormalConverter_CS_x64_Debug.h>

#else //_DEBUG

#include <Engine/CompiledShaderHeader/Basic3D_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Basic3D_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/Debug_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Debug_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/Grid_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Grid_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/Particle_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Particle_GS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Particle_PS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Particle_CS_x64_Release.h>

#include <Engine/CompiledShaderHeader/PostProcess_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/PostProcess_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/Sprite_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Sprite_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/Triangle_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Triangle_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/UserInterface_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/UserInterface_PS_x64_Release.h>


#include <Engine/CompiledShaderHeader/Deffered_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Deffered_PS_x64_Release.h>

//#include <Engine/CompiledShaderHeader/Light_VS_x64_Release.h>
//#include <Engine/CompiledShaderHeader/Light_PS_x64_Release.h>
#include <Engine/CompiledShaderHeader/LightDir_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/LightDir_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/LightPoint_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/LightPoint_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/Merge_VS_x64_Release.h>
#include <Engine/CompiledShaderHeader/Merge_PS_x64_Release.h>

#include <Engine/CompiledShaderHeader/Animation3D_CS_x64_Release.h>

#include <Engine/CompiledShaderHeader/GPUInitSetting_CS_x64_Release.h>

#include <Engine/CompiledShaderHeader/NormalConverter_CS_x64_Release.h>

#endif //_DEBUG

#else //_WIN64

#error "x64만 지원"

#endif //_WIN64
