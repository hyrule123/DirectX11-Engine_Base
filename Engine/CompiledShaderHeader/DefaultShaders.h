
//컴파일된 쉐이더 헤더 모아놓은 헤더
#ifdef _WIN64

#ifdef _DEBUG
	
#include "Forward3D_VS_x64_Debug.h"
#include "Forward3D_PS_x64_Debug.h"

#include "Debug_VS_x64_Debug.h"
#include "Debug_PS_x64_Debug.h"

#include "Grid_VS_x64_Debug.h"
#include "Grid_PS_x64_Debug.h"

#include "Particle_VS_x64_Debug.h"
#include "Particle_GS_x64_Debug.h"
#include "Particle_PS_x64_Debug.h"
#include "Particle_CS_x64_Debug.h"

#include "PostProcess_VS_x64_Debug.h"
#include "PostProcess_PS_x64_Debug.h"

#include "Sprite_VS_x64_Debug.h"
#include "Sprite_PS_x64_Debug.h"

#include "UI_VS_x64_Debug.h"
#include "UI_PS_x64_Debug.h"

#include "Deffered3D_VS_x64_Debug.h"
#include "Deffered3D_PS_x64_Debug.h"

//#include "Light_VS_x64_Debug.h"
//#include "Light_PS_x64_Debug.h"

#include "LightDirection_VS_x64_Debug.h"
#include "LightDirection_PS_x64_Debug.h"

#include "LightPoint_VS_x64_Debug.h"
#include "LightPoint_PS_x64_Debug.h"

#include "Deffered_Merger_VS_x64_Debug.h"
#include "Deffered_Merger_PS_x64_Debug.h"

#include "Animation3D_CS_x64_Debug.h"

#include "GPU_Init_Setting_CS_x64_Debug.h"

#include "NormalConverter_CS_x64_Debug.h"

#else //_DEBUG

#include "Basic3D_VS_x64_Release.h"
#include "Basic3D_PS_x64_Release.h"

#include "Debug_VS_x64_Release.h"
#include "Debug_PS_x64_Release.h"

#include "Grid_VS_x64_Release.h"
#include "Grid_PS_x64_Release.h"

#include "Particle_VS_x64_Release.h"
#include "Particle_GS_x64_Release.h"
#include "Particle_PS_x64_Release.h"
#include "Particle_CS_x64_Release.h"

#include "PostProcess_VS_x64_Release.h"
#include "PostProcess_PS_x64_Release.h"

#include "Sprite_VS_x64_Release.h"
#include "Sprite_PS_x64_Release.h"

#include "Triangle_VS_x64_Release.h"
#include "Triangle_PS_x64_Release.h"

#include "UserInterface_VS_x64_Release.h"
#include "UserInterface_PS_x64_Release.h"


#include "Deffered_VS_x64_Release.h"
#include "Deffered_PS_x64_Release.h"

//#include "Light_VS_x64_Release.h"
//#include "Light_PS_x64_Release.h"
#include "LightDir_VS_x64_Release.h"
#include "LightDir_PS_x64_Release.h"

#include "LightPoint_VS_x64_Release.h"
#include "LightPoint_PS_x64_Release.h"

#include "Merge_VS_x64_Release.h"
#include "Merge_PS_x64_Release.h"

#include "Animation3D_CS_x64_Release.h"

#include "GPUInitSetting_CS_x64_Release.h"

#include "NormalConverter_CS_x64_Release.h"

#endif //_DEBUG

#else //_WIN64

#error "x64만 지원"

#endif //_WIN64
