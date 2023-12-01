#pragma once

//컴파일된 쉐이더 헤더 모아놓은 헤더
#ifdef _WIN64

#ifdef _DEBUG
	
#include "CompiledShaderHeader/VS_Basic3D_x64_Debug.h"
#include "CompiledShaderHeader/PS_Basic3D_x64_Debug.h"

#include "CompiledShaderHeader/VS_Debug_x64_Debug.h"
#include "CompiledShaderHeader/PS_Debug_x64_Debug.h"

#include "CompiledShaderHeader/VS_Grid_x64_Debug.h"
#include "CompiledShaderHeader/PS_Grid_x64_Debug.h"

#include "CompiledShaderHeader/VS_Particle_x64_Debug.h"
#include "CompiledShaderHeader/GS_Particle_x64_Debug.h"
#include "CompiledShaderHeader/PS_Particle_x64_Debug.h"
#include "CompiledShaderHeader/CS_Particle_x64_Debug.h"

#include "CompiledShaderHeader/VS_PostProcess_x64_Debug.h"
#include "CompiledShaderHeader/PS_PostProcess_x64_Debug.h"

#include "CompiledShaderHeader/VS_Sprite_x64_Debug.h"
#include "CompiledShaderHeader/PS_Sprite_x64_Debug.h"

#include "CompiledShaderHeader/VS_Triangle_x64_Debug.h"
#include "CompiledShaderHeader/PS_Triangle_x64_Debug.h"

#include "CompiledShaderHeader/VS_UserInterface_x64_Debug.h"
#include "CompiledShaderHeader/PS_UserInterface_x64_Debug.h"

#include "CompiledShaderHeader/VS_Deffered_x64_Debug.h"
#include "CompiledShaderHeader/PS_Deffered_x64_Debug.h"

//#include "CompiledShaderHeader/VS_Light_x64_Debug.h"
//#include "CompiledShaderHeader/PS_Light_x64_Debug.h"

#include "CompiledShaderHeader/VS_LightDir_x64_Debug.h"
#include "CompiledShaderHeader/PS_LightDir_x64_Debug.h"

#include "CompiledShaderHeader/VS_LightPoint_x64_Debug.h"
#include "CompiledShaderHeader/PS_LightPoint_x64_Debug.h"

#include "CompiledShaderHeader/VS_Merge_x64_Debug.h"
#include "CompiledShaderHeader/PS_Merge_x64_Debug.h"

#include "CompiledShaderHeader/CS_Animation3D_x64_Debug.h"

#include "CompiledShaderHeader/CS_GPUInitSetting_x64_Debug.h"

#include "CompiledShaderHeader/CS_NormalConverter_x64_Debug.h"

#else //_DEBUG

#include "CompiledShaderHeader/VS_Basic3D_x64_Release.h"
#include "CompiledShaderHeader/PS_Basic3D_x64_Release.h"

#include "CompiledShaderHeader/VS_Debug_x64_Release.h"
#include "CompiledShaderHeader/PS_Debug_x64_Release.h"

#include "CompiledShaderHeader/VS_Grid_x64_Release.h"
#include "CompiledShaderHeader/PS_Grid_x64_Release.h"

#include "CompiledShaderHeader/VS_Particle_x64_Release.h"
#include "CompiledShaderHeader/GS_Particle_x64_Release.h"
#include "CompiledShaderHeader/PS_Particle_x64_Release.h"
#include "CompiledShaderHeader/CS_Particle_x64_Release.h"

#include "CompiledShaderHeader/VS_PostProcess_x64_Release.h"
#include "CompiledShaderHeader/PS_PostProcess_x64_Release.h"

#include "CompiledShaderHeader/VS_Sprite_x64_Release.h"
#include "CompiledShaderHeader/PS_Sprite_x64_Release.h"

#include "CompiledShaderHeader/VS_Triangle_x64_Release.h"
#include "CompiledShaderHeader/PS_Triangle_x64_Release.h"

#include "CompiledShaderHeader/VS_UserInterface_x64_Release.h"
#include "CompiledShaderHeader/PS_UserInterface_x64_Release.h"

#include "CompiledShaderHeader/CS_Paint_x64_Release.h"

#include "CompiledShaderHeader/VS_Deffered_x64_Release.h"
#include "CompiledShaderHeader/PS_Deffered_x64_Release.h"

//#include "CompiledShaderHeader/VS_Light_x64_Release.h"
//#include "CompiledShaderHeader/PS_Light_x64_Release.h"
#include "CompiledShaderHeader/VS_LightDir_x64_Release.h"
#include "CompiledShaderHeader/PS_LightDir_x64_Release.h"

#include "CompiledShaderHeader/VS_LightPoint_x64_Release.h
#include "CompiledShaderHeader/PS_LightPoint_x64_Release.h"

#include "CompiledShaderHeader/VS_Merge_x64_Release.h"
#include "CompiledShaderHeader/PS_Merge_x64_Release.h"

#include "CompiledShaderHeader/CS_Animation3D_x64_Release.h"

#include "CompiledShaderHeader/CS_GPUInitSetting_x64_Release.h"

#include "CompiledShaderHeader/CS_NormalConverter_x64_Release.h"

#endif //_DEBUG

#else //_WIN64

#error "x64만 지원"

#endif //_WIN64
