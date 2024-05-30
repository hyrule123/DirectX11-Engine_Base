#pragma once

#include <regex>
#include <filesystem>

namespace stdfs = std::filesystem;


//#define PRESET constexpr const char*

#define PRESET(_VarNameBase, _String) \
namespace _VarNameBase {\
constexpr const char* A = _String;\
constexpr const wchar_t* W = L##_String;\
}


namespace define_Preset
{
	namespace Path
	{
		PRESET(ContentsProj, "./Project/Contents");
		PRESET(EngineProj, "./Project/Base/Engine");
		PRESET(EngineComponent, "./Project/Base/Engine/Scene/Component");
		PRESET(Shader_Proj, "./Project/Shader");


#ifdef _DEBUG
		PRESET(Resources, "./Output/Debug/Res");
#else
		PRESET(Resources, "./Output/Release/Res");
#endif
	}
	
	namespace Keyword
	{
		PRESET(EngineMasterNamespace, "ehw");

		PRESET(arrNumThreads, "arrNumThreads");
		
		PRESET(ScriptPrefix, "Script_");

		PRESET(ConstexprInlineConstChar, "PRESET(");
		PRESET(EqualDoubleQuotation, ", \"");
		PRESET(strKey, "strKey_");

		PRESET(DefineSTRKEY, 
R"(#ifndef STRKEY
#define STRKEY constexpr const char*
#endif
)");

		PRESET(PragmaOnce, R"(#pragma once
)");

		PRESET(Head, 
R"(//=========================================================
//This Code is Automatically generated by CodeGenerator
//=========================================================

)");
		PRESET(Define_Constructor_T,
			R"(#define CONSTRUCTOR_T(T) )"
		);

		PRESET(Constructor_T, "CONSTRUCTOR_T(");
	}




	namespace Regex
	{

		constexpr const char* arrCharsVarForbiddenA[] =
		{
			"\\(","\\)",
			"\\{","\\}",
			"\\[","\\]",
			"\\.","\\s"
		};


		namespace g_VarForbiddenChars
		{
			extern std::regex A;
			extern std::wregex W;
			void CreateVarForbiddenRegex();
		}

		PRESET(AllShader, R"(\w+\.hlsl$)");
		PRESET(CShaderRegex, R"(CS_\w+\.hlsl$)");
		PRESET(ScriptRegex, R"(Script_\w+\.h)");
		PRESET(ComponentRegex, R"(^Com_\w+\.h)");
		PRESET(SceneRegex, R"(Scene_\w+\.h)");
		
		//[   numthreads   ( %d, %d, %d )   ]
		//[ ] 안에 둘러싸여 있고, 공백 갯수에 상관없이 숫자 3개를 추출
		PRESET(Numthread, R"(.*\((\d+),(\d+),(\d+\)).*)");
	}
}


#include "Engine/GPU/define_GPU.h"
namespace define_ShaderGen
{
	struct tShaderSetting
	{
		stdfs::path FileName[(int)ehw::eGSStage::END];
	};
}

