#pragma once
//트리 관리자 클래스
#include "CodeGenerator/DirTreeNode.h"

#include <Engine/define_Enum.h>
#include "json-cpp/json-forwards.h"

struct tAddBaseClassDesc
{
	//파일 경로
	stdfs::path SaveFilePath;						

	//어떤 클래스의 함수인지?
	std::string OwnerClassName;						

	//어떤 Base Type을 다룰것인지?
	std::string BaseType;							

	//찾은 헤더 앞에 붙일 Base Path 경로(없을 시 비워도 됨)
	std::string BasePath_FoundHeader;	

	//cpp 파일의 헤더 경로
	std::string IncludePath_OwnerClass;

	//Base Type의 헤더 경로
	std::string IncludePath_BaseTypeHeader;

	//미리 컴파일된 헤더 경로(없을 시 비워도 됨)
	std::string IncludePath_PreCompiledHeader;	

	//Manager 클래스의 헤더 경로
	std::string IncludePath_ManagerHeader;

	//StrKey의 헤더 경로
	std::string IncludePath_StrKeyHeader;
	
	//매크로 정의
	std::string Constructor_T_MacroDefine;

	//가장 바깥의 네임스페이스
	std::string MasterNamespace;

	//using namespace 필요시 작성(없을 시 비워도 됨)
	std::string UsingNamespace;

	//User 컴포넌트 매니저의 경우 함수 명을 등록
	std::string UserClassMgr_InitFuncName;
};

class DirTree
{
public:
	DirTree();
	~DirTree();

	//시스템 탐색
	struct tSearchDesc
	{
		stdfs::path rootPath;
		std::regex regex;
		stdfs::path prevTextFileName;
	};
	HRESULT SearchRecursive(const tSearchDesc& _desc);

	//파일 명을 헤더 파일에 등록(편의용도)
	struct tStrKeyWriteDesc
	{
		stdfs::path filePath;
		std::string rootNamespace;
		bool bEraseExtension;
		bool bWriteChildNamespace;
		bool bAddRelativeDirToString;
	};
	HRESULT CreateStrKeyHeader(const tStrKeyWriteDesc& _desc);

	//ContentsInitializer에 자신의 클래스를 등록하는 코드를 생성하는 함수
	HRESULT CreateComponentManagerInitCode(tAddBaseClassDesc const& _Desc);

	HRESULT CreateShaderStrKey(stdfs::path const& _FilePath);

private:
	void Reset() { m_RootDir.Clear(); m_PrevFiles.clear(); m_PrevFilesSavePath.clear(); m_bModified = false; }

	std::unordered_map<stdfs::path, ePrevFileStatus> ReadPrevFilesList(const stdfs::path& _filePath);

	//변경점을 확인하고 발견되지 않은 파일을 map에서 제거
	bool CheckDiff();

	void WritePrevFilesList(const stdfs::path& _filePath);

	//에러 발생시 처리용
	inline void ClearPrevFilesList(const stdfs::path& _filePath);
private:
	DirTreeNode m_RootDir;
	std::unordered_map<stdfs::path, ePrevFileStatus> m_PrevFiles;
	stdfs::path m_PrevFilesSavePath;
	bool m_bModified;
};

inline void DirTree::ClearPrevFilesList(const stdfs::path& _filePath)
{
	std::ofstream ofs(stdfs::path(_filePath).replace_extension(".txt"), std::ios::trunc);
	ofs.close();
}
