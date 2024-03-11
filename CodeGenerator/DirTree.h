#pragma once
//트리 관리자 클래스
#include "CodeGenerator/DirTreeNode.h"

#include "../Engine/define_Enum.h"
#include "json-cpp/json-forwards.h"

struct tAddBaseClassDesc
{
	stdfs::path FilePath;
	std::string ClassName;
	std::string BaseType;
	std::string IncludePCH;
	std::string IncludeManagerHeader;
	std::string IncludeStrKeyHeaderName;
	std::string IncludeBaseTypeHeader;
	std::string Constructor_T_MacroDefine;
	std::string MasterNamespace;
	std::string UsingNamespace;
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

	//UserContentsInitializer에 자신의 클래스를 등록하는 코드를 생성하는 함수
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








