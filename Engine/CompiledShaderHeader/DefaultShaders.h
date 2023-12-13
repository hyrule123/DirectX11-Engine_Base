
//컴파일된 쉐이더 헤더 모아놓은 헤더
#ifdef _WIN64

#ifdef _DEBUG
	
#include "VS_Basic3D_x64_Debug.h"
#include "PS_Basic3D_x64_Debug.h"

#include "VS_Debug_x64_Debug.h"
#include "PS_Debug_x64_Debug.h"

#include "VS_Grid_x64_Debug.h"
#include "PS_Grid_x64_Debug.h"

#include "VS_Particle_x64_Debug.h"
#include "GS_Particle_x64_Debug.h"
#include "PS_Particle_x64_Debug.h"
#include "CS_Particle_x64_Debug.h"

#include "VS_PostProcess_x64_Debug.h"
#include "PS_PostProcess_x64_Debug.h"

#include "VS_Sprite_x64_Debug.h"
#include "PS_Sprite_x64_Debug.h"

#include "VS_Triangle_x64_Debug.h"
#include "PS_Triangle_x64_Debug.h"

#include "VS_UserInterface_x64_Debug.h"
#include "PS_UserInterface_x64_Debug.h"

#include "VS_Deffered_x64_Debug.h"
#include "PS_Deffered_x64_Debug.h"

//#include "VS_Light_x64_Debug.h"
//#include "PS_Light_x64_Debug.h"

#include "VS_LightDir_x64_Debug.h"
#include "PS_LightDir_x64_Debug.h"

#include "VS_LightPoint_x64_Debug.h"
#include "PS_LightPoint_x64_Debug.h"

#include "VS_Merge_x64_Debug.h"
#include "PS_Merge_x64_Debug.h"

#include "CS_Animation3D_x64_Debug.h"

#include "CS_GPUInitSetting_x64_Debug.h"

#include "CS_NormalConverter_x64_Debug.h"

#else //_DEBUG

#include "VS_Basic3D_x64_Release.h"
#include "PS_Basic3D_x64_Release.h"

#include "VS_Debug_x64_Release.h"
#include "PS_Debug_x64_Release.h"

#include "VS_Grid_x64_Release.h"
#include "PS_Grid_x64_Release.h"

#include "VS_Particle_x64_Release.h"
#include "GS_Particle_x64_Release.h"
#include "PS_Particle_x64_Release.h"
#include "CS_Particle_x64_Release.h"

#include "VS_PostProcess_x64_Release.h"
#include "PS_PostProcess_x64_Release.h"

#include "VS_Sprite_x64_Release.h"
#include "PS_Sprite_x64_Release.h"

#include "VS_Triangle_x64_Release.h"
#include "PS_Triangle_x64_Release.h"

#include "VS_UserInterface_x64_Release.h"
#include "PS_UserInterface_x64_Release.h"


#include "VS_Deffered_x64_Release.h"
#include "PS_Deffered_x64_Release.h"

//#include "VS_Light_x64_Release.h"
//#include "PS_Light_x64_Release.h"
#include "VS_LightDir_x64_Release.h"
#include "PS_LightDir_x64_Release.h"

#include "VS_LightPoint_x64_Release.h"
#include "PS_LightPoint_x64_Release.h"

#include "VS_Merge_x64_Release.h"
#include "PS_Merge_x64_Release.h"

#include "CS_Animation3D_x64_Release.h"

#include "CS_GPUInitSetting_x64_Release.h"

#include "CS_NormalConverter_x64_Release.h"

#endif //_DEBUG

#else //_WIN64

#error "x64만 지원"

#endif //_WIN64
