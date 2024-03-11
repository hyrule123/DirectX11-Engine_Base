#pragma once
#include "CodeGenerator/CodeWriter.h"


struct tShaderGroup
{
	stdfs::path FileName[(int)ehw::eGSStage::END];
};

enum class ePrevFileStatus
{
	NotFound,
	Found,
	New
};

class DirTreeNode
{
	friend class DirTree;
	friend class DirTreeNode;

private:
	DirTreeNode();
	DirTreeNode(DirTreeNode* _pParent);
	~DirTreeNode();

private:
	//인자가 지속적으로 추가되어 수정 용이성을 위해 Desc 방식으로 변경하였음.
	struct SearchDesc
	{
		const stdfs::path& rootDir;
		const std::regex& regex;
		std::unordered_map<stdfs::path, ePrevFileStatus>& prevFilesList;

		SearchDesc(
			const stdfs::path& _rootDir,
			const std::regex& _regex,
			std::unordered_map<stdfs::path, ePrevFileStatus>& _prevFilesList)
			: rootDir(_rootDir)
			, regex(_regex)
			, prevFilesList(_prevFilesList)
		{}
	};

	HRESULT SearchRecursive(SearchDesc _searchParam, stdfs::path const& _currentDir);

	//찾은 파일명을 기반으로 String Key를 생성한다.
	struct tStrKeyWriteDesc
	{
		CodeWriter& codeWriter;
		const bool bEraseExtension;
		const bool bWriteChildNamespace;
		const bool bAddRelativeDirToString;

		tStrKeyWriteDesc(CodeWriter& _codeWriter, bool _bEraseExtension, bool _bWriteChildNamespace, bool _bAddRelativeDirToString)
			: codeWriter(_codeWriter)
			, bEraseExtension(_bEraseExtension)
			, bWriteChildNamespace(_bWriteChildNamespace)
			, bAddRelativeDirToString(_bAddRelativeDirToString)
		{}
	};
	HRESULT WriteStrKeyTree(tStrKeyWriteDesc _desc);


	void SetParent(DirTreeNode* _pNode) { assert(_pNode); m_pParent = _pNode; }
	void AddChild(DirTreeNode* _pNode) { assert(_pNode); m_vecChild.push_back(_pNode); }

	bool IsRoot() const { return (nullptr == m_pParent); }
	bool IsLeaf() const { return m_vecChild.empty(); }
	bool IsReady() const { return !m_DirName.empty(); }

private:
	DirTreeNode* m_pParent;
	std::vector<DirTreeNode*> m_vecChild;

	stdfs::path	  m_DirName;
	std::vector<stdfs::path>	  m_vecFileName;

	//DirTree에서 사용
private:
	void Clear();
};











