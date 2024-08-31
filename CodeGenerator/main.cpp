#include "CodeGenerator/DirTree.h"
#include "Engine/Resource/define_Resource.h"
#include "Engine/GPU/define_GPU.h"

#include <iostream>

void manual_input(std::vector<std::string>& _args);
void create_texture_key();
void create_shader_key();
void create_engine_component_key();
void create_user_component_key();
void create_script_key();
void create_scene_key();

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
        manual_input(args);
    }

    for (size_t i = 0; i < args.size(); ++i)
    {
        StringConv::UpperCase(args[i]);

        if ("ENGINECOMPONENT" == args[i])
        {
            create_engine_component_key();
        }
        else if ("USERCOMPONENT" == args[i])
        {
            create_user_component_key();
        }
        else if ("SHADER" == args[i])
        {
            create_shader_key();
        }

        else if ("SCRIPT" == args[i])
        {
            create_script_key();
        }
        else if ("SCENE" == args[i])
        {
            create_scene_key();
        }
    }
    
    return 0;
}

void create_texture_key()
{
    //CreateBuffer variable name restraints regex
    define_Preset::Regex::g_VarForbiddenChars::create_forbidden_regex();

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
        desc.rootPath = define_Preset::Path::Resources;
        desc.rootPath /= ehw::strKey::path::directory::resource::Texture;
        desc.regex = reg;
        desc.prevTextFileName = "FoundTextures";

        DirTree.SearchRecursive(desc);
    }

    //strKey 작성
    {
        DirTree::tStrKeyWriteDesc desc{};
        desc.filePath = define_Preset::Path::ContentsProj;
        desc.filePath /= "strKey_Texture.h";
        desc.rootNamespace = "texture";
        desc.bEraseExtension = false;
        desc.bWriteChildNamespace = true;
        desc.bAddRelativeDirToString = true;

        DirTree.CreateStrKeyHeader(desc);
    }
}

void manual_input(std::vector<std::string>& _args)
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

void create_shader_key()
{
    DirTree DirTree;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = define_Preset::Path::Shader_Proj;
        desc.regex = std::regex(define_Preset::Regex::AllShader.data());
        desc.prevTextFileName = "FoundShaders";
        DirTree.SearchRecursive(desc);
    }
    
    //쉐이더 StrKey 작성
    {
        stdfs::path strKeyPath = define_Preset::Path::ContentsProj;
        strKeyPath /= "strKey_Shader.h";
        DirTree.CreateShaderStrKey(strKeyPath);
    }
}

void create_engine_component_key()
{
    DirTree DirTree;
    stdfs::path DirPath = define_Preset::Path::EngineComponent;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = DirPath;
        desc.regex = std::regex(define_Preset::Regex::ComponentRegex.data());
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
        Desc.SaveFilePath = DirPath / "ComponentInitializer.cpp";

        Desc.OwnerClassName = "ComponentInitializer";
        
        Desc.BaseType = "iComponent";
        Desc.BasePath_FoundHeader = "Engine/Game/Component";

        Desc.IncludePath_OwnerClass = R"("Engine/Game/Component/ComponentInitializer.h")";
        Desc.IncludePath_BaseTypeHeader = R"("Engine/Game/Component/iComponent.h")";
        Desc.IncludePath_PreCompiledHeader = R"("Engine/PCH_Engine.h")";
        Desc.IncludePath_StrKeyHeader = R"("Engine/Game/Component/strKey_Component.h")";
        Desc.IncludePath_ManagerHeader = R"("Engine/Manager/ComponentManager.h")";

        Desc.MasterNamespace = define_Preset::Keyword::EngineMasterNamespace;
        Desc.UsingNamespace = "";
        Desc.Constructor_T_MacroDefine = 
            R"(ComponentManager::GetInst().AddComponentConstructor<T>(strKey::component::##T))";
        Desc.UserClassMgr_InitFuncName = "Init()";

        DirTree.CreateComponentManagerInitCode(Desc);
    }
}

void create_user_component_key()
{
    DirTree DirTree;
    stdfs::path DirPath = define_Preset::Path::ContentsProj;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = DirPath;
        desc.regex = std::regex(define_Preset::Regex::ComponentRegex.data());
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

    //ContentsInitializer 작성
    {
        tAddBaseClassDesc Desc = {};
        Desc.SaveFilePath = DirPath / "ContentsInitializer_Component.cpp";

        Desc.BaseType = "iComponent";

        Desc.BasePath_FoundHeader = "Contents";

        Desc.OwnerClassName = "ContentsInitializer";

        Desc.IncludePath_OwnerClass = R"("Contents/ContentsInitializer.h")";
        Desc.IncludePath_StrKeyHeader = R"("Contents/strKey_UserComponent.h")";
        Desc.IncludePath_BaseTypeHeader = R"(<Engine/Game/Component/iComponent.h>)";
        Desc.IncludePath_PreCompiledHeader = R"("PCH_Contents.h")";
        Desc.IncludePath_ManagerHeader = R"(<Engine/Manager/ComponentManager.h>)";
        Desc.MasterNamespace = define_Preset::Keyword::EngineMasterNamespace;
        Desc.UsingNamespace = "";
        Desc.Constructor_T_MacroDefine = R"(ComponentManager::GetInst().AddComponentConstructor<T>(strKey::component::##T))";
        Desc.UserClassMgr_InitFuncName = "InitUserComponent()";
        

        DirTree.CreateComponentManagerInitCode(Desc);
    }

}

void create_script_key()
{
    DirTree DirTree;
    stdfs::path DirPath = define_Preset::Path::ContentsProj;

    //탐색
    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = DirPath;
        desc.regex = define_Preset::Regex::ScriptRegex.data();
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
    

    //ContentsInitializer 코드 생성
    {
        tAddBaseClassDesc Desc = {};
        Desc.SaveFilePath = DirPath / "ContentsInitializer_Script.cpp";
        Desc.BasePath_FoundHeader = "Contents";

        Desc.BaseType = "Script";
        
        Desc.OwnerClassName = "ContentsInitializer";
        Desc.IncludePath_OwnerClass = R"("Contents/ContentsInitializer.h")";

        Desc.IncludePath_PreCompiledHeader = R"("PCH_Contents.h")";
        Desc.IncludePath_BaseTypeHeader = R"(<Engine/Game/Component/Script/Script.h>)";
        Desc.IncludePath_StrKeyHeader = R"("Contents/strKey_Script.h")";
        Desc.IncludePath_ManagerHeader = "<Engine/Manager/ComponentManager.h>";
        Desc.MasterNamespace = define_Preset::Keyword::EngineMasterNamespace;
        Desc.UsingNamespace = "";
        Desc.Constructor_T_MacroDefine = R"(ComponentManager::GetInst().AddComponentConstructor<T>(strKey::script::##T))";
        Desc.UserClassMgr_InitFuncName = "InitScript()";
        

        DirTree.CreateComponentManagerInitCode(Desc);
    }
}

void create_scene_key()

//Generate Scene
{
    DirTree DirTree;
    stdfs::path DirPath = define_Preset::Path::ContentsProj;

    {
        DirTree::tSearchDesc desc{};
        desc.rootPath = DirPath;
        desc.regex = std::regex(define_Preset::Regex::SceneRegex.data());
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
        Desc.SaveFilePath = DirPath / "ContentsInitializer_Scene.cpp";
        Desc.BasePath_FoundHeader = "Contents";

        Desc.BaseType = "Scene";
        Desc.OwnerClassName = "ContentsInitializer";

        Desc.IncludePath_OwnerClass = R"("Contents/ContentsInitializer.h")";
        Desc.IncludePath_BaseTypeHeader = R"(<Engine/Game/Scene.h>)";
        Desc.IncludePath_PreCompiledHeader = R"("Contents/PCH_Contents.h")";
        Desc.IncludePath_StrKeyHeader = R"("Contents/strKey_Scene.h")";
        Desc.IncludePath_ManagerHeader = "<Engine/Manager/SceneManager.h>";

        Desc.MasterNamespace = define_Preset::Keyword::EngineMasterNamespace;
        Desc.UsingNamespace = "";
        Desc.Constructor_T_MacroDefine = R"(SceneManager::GetInst().AddSceneConstructor<T>(strKey::scene::##T))";
        Desc.UserClassMgr_InitFuncName = "InitScene()";
        

        DirTree.CreateComponentManagerInitCode(Desc);
    }

}
