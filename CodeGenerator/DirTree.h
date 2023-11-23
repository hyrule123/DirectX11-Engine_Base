#pragma once
//트리 관리자 클래스
#include "DirTreeNode.h"

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
	HRESULT SearchRecursive(stdfs::path const& _RootPath, std::regex const& _regex);

	//파일 명을 헤더 파일에 등록(편의용도)
	HRESULT CreateStrKeyHeader(stdfs::path const& _FilePath, stdfs::path const& _RootNamespace, bool _bEraseExtension);

	//ContentsClassInitializer에 자신의 클래스를 등록하는 코드를 생성하는 함수
	HRESULT CreateComMgrInitCode(tAddBaseClassDesc const& _Desc);

	HRESULT CreateShaderStrKey(stdfs::path const& _FilePath);

private:
	std::unordered_map<stdfs::path, ePrevFileStatus> ReadPrevFilesList(const stdfs::path& _filePath);
	//PrevFiles 목록과 비교해서 변경사항을 확인
	bool CheckDiffAndWritePrevFiles(const stdfs::path& _filePath, std::unordered_map<stdfs::path, ePrevFileStatus>& _prevFilesList);
	inline void ClearPrevFilesList(const stdfs::path& _filePath);

private:
	DirTreeNode m_RootDir;
};

inline void DirTree::ClearPrevFilesList(const stdfs::path& _filePath)
{
	std::ofstream ofs(stdfs::path(_filePath).replace_extension(".txt"), std::ios::trunc);
	ofs.close();
}








