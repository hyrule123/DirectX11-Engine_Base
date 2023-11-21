#pragma once

#include "PCH_CodeGenerator.h"
#include "CodeWriter.h"
#include "../Engine/define_GPU.h"


struct tShaderGroup
{
	stdfs::path FileName[(int)ehw::define::eGSStage::END];
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

public:
	HRESULT SearchRecursive(stdfs::path const& _rootPath, stdfs::path const& _path, std::regex const& _regex);

	HRESULT GetAllFiles(__out std::unordered_map<stdfs::path, ePrevFileStatus>& _filesList, bool _bAddRelativeDir);


	//_prevFiles : 파일명/이전에 작성했었던 파일인지 여부
	HRESULT WriteStrKeyTree(CodeWriter& _CodeWriter, bool _bEraseExtension, std::unordered_map<stdfs::path, ePrevFileStatus>& _prevFiles);

private:
	DirTreeNode* m_pParent;
	std::vector<DirTreeNode*> m_vecChild;

public:
	void SetParent(DirTreeNode* _pNode) { assert(_pNode); m_pParent = _pNode; }
	void AddChild(DirTreeNode* _pNode) { assert(_pNode); m_vecChild.push_back(_pNode); }

	bool IsRoot() const { return (nullptr == m_pParent); }
	bool IsLeaf() const { return m_vecChild.empty(); }
	bool IsReady() const { return !m_DirName.empty(); }

private:
	stdfs::path	  m_DirName;
	std::vector<stdfs::path>	  m_vecFileName;

	//DirTree에서 사용
private:
	void Clear();
};











