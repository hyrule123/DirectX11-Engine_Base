#include "CodeGenerator/DirTree.h"
#include "Engine/Resource/define_Resource.h"
#include "Engine/GPU/define_GPU.h"

#include <iostream>

void ManualInput(std::vector<std::string>& _args);
void CreateTextureKey();
void CreateShaderKey();
void CreateEngineComponentKey();
void CreateUserComponentKey();
void CreateScriptKey();
void CreateSceneKey();




//시작 지점 = $(SolutionDir) : 상대 경로로 작업해주면 된다.
int main(int argc, char* argv[])
{

    std::vector<std::string>args{};
    for (int i = 1; i < argc; ++i)
    {
        args.push_back(argv[i]);
    }

    //argument가 들어오지 않았을 경우 직접 입력 받는다
    if (args.empty())
    {
        ManualInput(args);
    }


    for (size_t i = 0; i < args.size(); ++i)
    {
        StringConv::UpperCase(args[i]);

        if ("ENGINECOMPONENT" == args[i])
        {
            CreateEngineComponentKey();
        }
        else if ("USERCOMPONENT" == args[i])
        {
            CreateUserComponentKey();
        }
        //else if ("TEXTURE" == args[i])
        //{
        //    CreateTextureKey();
        //}
        else if ("SHADER" == args[i])
        {
            CreateShaderKey();
        }

        else if ("SCRIPT" == args[i])
        {
            CreateScriptKey();
        }
        else if ("SCENE" == args[i])
        {
            CreateSceneKey();
        }
    }
    
    return 0;
}

void CreateTextureKey()
{
    //Create variable name restraints regex
    define_Preset::Regex::g_VarForbiddenChars::CreateVarForbiddenRegex();

    constexpr const char* TexExt[] = { ".bmp", ".png", ".jpg", ".tga", ".dds" };
    constexpr const size_t TexExtSize = sizeof(TexExt) / sizeof(TexExt[0]);

    //Generate Texture Key

    std::string regbase;
    regbase += R"((.+)\.()";

    for (size_t i = 0; i < TexExtSize; ++i)
    {
        std::string temp = TexExt[i];
        size_t pos = temp.find('.');
        if (std::string::npos != pos)
        {
            temp.erase(0, pos + 1);
        }
        regbase += temp;

        if (i == TexExtSize - 1ui64)
            regbase += ")$";
        else
            regbase += "|";
    }

    std::regex reg(regbase, std::regex::icase);

    DirTree DirTree;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = define_Preset::Path::Resources::A;
        desc.rootPath /= ehw::strKey::path::directory::resource::Texture;
        desc.regex = reg;
        desc.prevTextFileName = "FoundTextures";

        DirTree.SearchRecursive(desc);
    }

    //strKey 작성
    {
        DirTree::tStrKeyWriteDesc desc{};
        desc.filePath = define_Preset::Path::ContentsProj::A;
        desc.filePath /= "strKey_Texture.h";
        desc.rootNamespace = "texture";
        desc.bEraseExtension = false;
        desc.bWriteChildNamespace = true;
        desc.bAddRelativeDirToString = true;

        DirTree.CreateStrKeyHeader(desc);
    }
}

void ManualInput(std::vector<std::string>& _args)
{
    std::cout << "[[Manual Mode]]" << std::endl;

    std::cout << "Possible Modes: ";
    std::cout << "ComputeShader, EngineComponent, Component, Script, Scene" << std::endl;
    std::cout << "enter 'q' to end input" << std::endl;

    bool bGetInputs = true;
    while (bGetInputs)
    {
        std::cout << ">>";
        std::string input{};
        std::cin >> input;
        if ("q" == input || input.empty())
        {
            bGetInputs = false;
            continue;
        }

        _args.push_back(input);
    }
}

void CreateShaderKey()
{
    DirTree DirTree;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = define_Preset::Path::Shader_Proj::A;
        desc.regex = std::regex(define_Preset::Regex::AllShader::A);
        desc.prevTextFileName = "FoundShaders";
        DirTree.SearchRecursive(desc);
    }
    
    //쉐이더 StrKey 작성
    {
        stdfs::path strKeyPath = define_Preset::Path::ContentsProj::A;
        strKeyPath /= "strKey_Shader.h";
        DirTree.CreateShaderStrKey(strKeyPath);
    }
}

void CreateEngineComponentKey()
{
    DirTree DirTree;
    stdfs::path DirPath = define_Preset::Path::EngineComponent::A;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = DirPath;
        desc.regex = std::regex(define_Preset::Regex::ComponentRegex::A);
        desc.prevTextFileName = "FoundEngineComponents";
        DirTree.SearchRecursive(desc);
    }


    //strKey 헤더 생성
    {
        DirTree::tStrKeyWriteDesc desc{};
        desc.filePath = DirPath / "strKey_Component.h";
        desc.rootNamespace = "component";
        desc.bEraseExtension = true;
        desc.bWriteChildNamespace = false;
        desc.bAddRelativeDirToString = false;
        DirTree.CreateStrKeyHeader(desc);
    }

    //Component Initializer cpp파일 작성
    {
        tAddBaseClassDesc Desc = {};
        Desc.BaseType = "iComponent";

        //PCH 강제 include 시켜서 필요 없어짐
        Desc.IncludePCH = R"(#include "PCH_Engine.h")";

        Desc.ClassName = "ComponentInitializer";
        Desc.IncludeStrKeyHeaderName = R"(#include "strKey_Component.h")";
        Desc.IncludeManagerHeader = R"(#include "../../Manager/ComponentManager.h")";
        Desc.MasterNamespace = define_Preset::Keyword::EngineMasterNamespace::A;
        Desc.UsingNamespace = "";
        Desc.Constructor_T_MacroDefine = R"(ComponentManager::AddComponentConstructor<T>(strKey::component::##T))";
        Desc.UserClassMgr_InitFuncName = "Init()";
        Desc.FilePath = DirPath / "ComponentInitializer.cpp";

        DirTree.CreateComponentManagerInitCode(Desc);
    }
}

void CreateUserComponentKey()
{
    DirTree DirTree;
    stdfs::path DirPath = define_Preset::Path::ContentsProj::A;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = DirPath;
        desc.regex = std::regex(define_Preset::Regex::ComponentRegex::A);
        desc.prevTextFileName = "FoundUserComponents";
        DirTree.SearchRecursive(desc);
    }
    
    //strKey 작성
    {
        DirTree::tStrKeyWriteDesc desc{};
        desc.filePath = DirPath / "strKey_UserComponent.h";
        desc.rootNamespace = "component";
        desc.bEraseExtension = true;
        desc.bWriteChildNamespace = false;
        desc.bAddRelativeDirToString = false;
        DirTree.CreateStrKeyHeader(desc);
    }

    //UserContentsInitializer 작성
    {
        tAddBaseClassDesc Desc = {};
        Desc.BaseType = "iComponent";
        Desc.IncludePCH = R"(#include "PCH_UserContents.h")";
        Desc.ClassName = "UserContentsInitializer";
        Desc.IncludeStrKeyHeaderName = R"(#include "strKey_UserComponent.h")";
        Desc.IncludeManagerHeader = R"(#include <Base/Engine/Manager/ComponentManager.h>)";
        Desc.MasterNamespace = define_Preset::Keyword::EngineMasterNamespace::A;
        Desc.UsingNamespace = "";
        Desc.Constructor_T_MacroDefine = R"(ComponentManager::AddComponentConstructor<T>(strKey::component::##T))";
        Desc.UserClassMgr_InitFuncName = "InitUserComponent()";
        Desc.FilePath = DirPath / "UserContentsInitializer_Component.cpp";

        DirTree.CreateComponentManagerInitCode(Desc);
    }

}

void CreateScriptKey()
{

    DirTree DirTree;
    stdfs::path DirPath = define_Preset::Path::ContentsProj::A;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = DirPath;
        desc.regex = define_Preset::Regex::ScriptRegex::A;
        desc.prevTextFileName = "FoundScripts";
        DirTree.SearchRecursive(desc);
    }
    
    //strKey 생성
    {
        DirTree::tStrKeyWriteDesc desc{};
        desc.filePath = DirPath;
        desc.filePath /= "strKey_Script.h";
        desc.rootNamespace = "script";
        desc.bEraseExtension = true;
        desc.bWriteChildNamespace = false;
        desc.bAddRelativeDirToString = false;
        DirTree.CreateStrKeyHeader(desc);
    }
    

    //UserContentsInitializer 코드 생성
    {
        tAddBaseClassDesc Desc = {};
        Desc.BaseType = "iScript";
        Desc.IncludePCH = R"(#include "PCH_UserContents.h")";
        Desc.ClassName = "UserContentsInitializer";
        Desc.IncludeStrKeyHeaderName = R"(#include "strKey_Script.h")";
        Desc.IncludeManagerHeader = "#include <Base/Engine/Manager/ComponentManager.h>";
        Desc.MasterNamespace = define_Preset::Keyword::EngineMasterNamespace::A;
        Desc.UsingNamespace = "";
        Desc.Constructor_T_MacroDefine = R"(ComponentManager::AddComponentConstructor<T>(strKey::script::##T))";
        Desc.UserClassMgr_InitFuncName = "InitScript()";
        Desc.FilePath = DirPath / "UserContentsInitializer_Script.cpp";

        DirTree.CreateComponentManagerInitCode(Desc);
    }
}

void CreateSceneKey()

//Generate Scene
{
    DirTree DirTree;
    stdfs::path DirPath = define_Preset::Path::ContentsProj::A;

    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = DirPath;
        desc.regex = std::regex(define_Preset::Regex::SceneRegex::A);
        desc.prevTextFileName = "FoundScenes";
        DirTree.SearchRecursive(desc);
    }


    {
        DirTree::tStrKeyWriteDesc desc{};
        desc.filePath = DirPath;
        desc.filePath /= "strKey_Scene.h";
        desc.rootNamespace = "scene";
        desc.bEraseExtension = true;
        desc.bWriteChildNamespace = false;
        desc.bAddRelativeDirToString = false;
        DirTree.CreateStrKeyHeader(desc);
    }
    
    {
        tAddBaseClassDesc Desc = {};
        Desc.BaseType = "iScene";
        Desc.IncludePCH = R"(#include "PCH_UserContents.h")";
        Desc.ClassName = "UserContentsInitializer";
        Desc.IncludeStrKeyHeaderName = R"(#include "strKey_Scene.h")";
        Desc.IncludeManagerHeader = "#include <Base/Engine/Manager/SceneManager.h>";
        Desc.MasterNamespace = define_Preset::Keyword::EngineMasterNamespace::A;
        Desc.UsingNamespace = "";
        Desc.Constructor_T_MacroDefine = R"(SceneManager::AddSceneConstructor<T>(strKey::scene::##T))";
        Desc.UserClassMgr_InitFuncName = "InitScene()";
        Desc.FilePath = DirPath / "UserContentsInitializer_Scene.cpp";

        DirTree.CreateComponentManagerInitCode(Desc);
    }

}
