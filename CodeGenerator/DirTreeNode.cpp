#include "PCH_CodeGenerator.h"

#include "DirTreeNode.h"
#include "define_Util.h"
#include "define_CodeGen.h"
#include "CodeWriter.h"

DirTreeNode::DirTreeNode()
	: m_pParent()
	, m_vecChild()
	, m_DirName()
	, m_vecFileName()
{
}

DirTreeNode::DirTreeNode(DirTreeNode* _pParent)
	: m_pParent(_pParent)
	, m_vecChild()
	, m_DirName()
	, m_vecFileName()
{
	assert(_pParent);
}

DirTreeNode::~DirTreeNode()
{
	size_t size = m_vecChild.size();
	for (size_t i = (size_t)0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
}

void DirTreeNode::Clear()
{
	size_t size = m_vecChild.size();
	for (size_t i = (size_t)0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
	m_vecChild.clear();
	m_DirName.clear();
	m_pParent = nullptr;
}

HRESULT DirTreeNode::SearchRecursive(SearchDesc _searchParam, const stdfs::path& _currentDir)
{
	//들어온 Path 자체가 폴더 경로가 아닐 경우에는 실패 반환
	ASSERT(stdfs::is_directory(_searchParam.rootDir), "루트 디렉토리에는 파일명이 붙어있으면 안 됩니다.");

	
	//자식 노드일 경우 상대적 경로를 등록
	if (false == IsRoot())
	{
		m_DirName = _currentDir.lexically_relative(_searchParam.rootDir);
	}
		

	try
	{
		for (const auto& dirIter : stdfs::directory_iterator(_currentDir))
		{
			//포함 항목 검사
			
			//파일명일 경우 - 확장자 및 파일명을 확인하고, 일치하는 경우에만 파일명을 등록
			if (false == dirIter.is_directory())
			{
				const stdfs::path& filename = dirIter.path().filename();

				//확장자의 경우 정확히 일치하는지 확인 -> 일치할 경우 찾은 것
				if (std::regex_search(filename.string(), _searchParam.regex))
				{
					//일치할 경우 추가
					m_vecFileName.push_back(filename);

					//해당 파일명을 prevFilesList에서 찾아본다.(루트 경로로부터의 상대경로가 키값)
					stdfs::path pathKey = dirIter.path().lexically_relative(_searchParam.rootDir);
					auto iter = _searchParam.prevFilesList.find(pathKey);

					//못찾았다 = 새로 발견
					if (_searchParam.prevFilesList.end() == iter)
					{
						_searchParam.prevFilesList.insert(std::make_pair(pathKey, ePrevFileStatus::New));
					}

					//찾았다
					else
					{
						iter->second = ePrevFileStatus::Found;
					}
				}
			}

			//폴더명일 경우
			else
			{
				//폴더를 발견했을 경우 새 노드를 생성 후 재귀호출
				DirTreeNode* pNode = new DirTreeNode(this);
				HRESULT hr = pNode->SearchRecursive(_searchParam, dirIter.path());
				
				if (ERROR_EMPTY == hr)
				{
					SAFE_DELETE(pNode);
					continue;
				}
				else if (FAILED(hr))
				{
					SAFE_DELETE(pNode);
					return hr;
				}


				AddChild(pNode);
			}
		}

		//순회를 다 돌았는데 자식 노드도 없고 자신에게 파일도 안들어 있을경우 -> ERROR_EMPTY 반환
		//ERROR_EMPTY가 반환되면 해당 노드가 제거됨.
		if (IsLeaf() && m_vecFileName.empty())
			return ERROR_EMPTY;
	}
	catch (stdfs::filesystem_error const& _err)
	{
		ERROR_MESSAGE_A(_err.what());
		return E_ACCESSDENIED;
	}

	return S_OK;
}


HRESULT DirTreeNode::WriteStrKeyTree(tStrKeyWriteDesc _desc)
{
	//자식 네임스페이스 작성이 true이고 + Root 네임스페이스가 아닐경우 -> 자식 네임스페이스 작성
	if (_desc.bWriteChildNamespace && false == IsRoot())
	{
		std::string strCode = "namespace ";

		std::string DirName = m_DirName.filename().string();
		//변수명에 사용할 수 없는 특수문자를 제외
		strCode += std::regex_replace(DirName, define_Preset::Regex::g_VarForbiddenChars::A, "_");

		_desc.codeWriter.WriteCode(0, strCode);
	}

	//자신의 파일목록 작성
	{
		//자식 네임스페이스를 작성했거나 루트 네임스페이스일 경우 중괄호를 열어준다
		if (_desc.bWriteChildNamespace || IsRoot())
		{
			_desc.codeWriter.OpenBracket(0);
		}
		

		size_t size = m_vecFileName.size();
		for (size_t i = 0; i < size; ++i)
		{
			std::string strCode = "STRKEY ";
			{
				std::string varName;
				
				if (_desc.bEraseExtension)
				{
					varName = m_vecFileName[i].filename().replace_extension("").string();
				}
				else
				{
					varName = m_vecFileName[i].filename().string();
				}
					

				//변수명에 사용할 수 없는 특수문자를 제외
				strCode += std::regex_replace(varName, define_Preset::Regex::g_VarForbiddenChars::A, "_");

			}




			strCode += " = \"";

			if (false == IsRoot() && true ==  _desc.bAddRelativeDirToString)
			{
				std::string tempstring = m_DirName.string();
				std::replace(tempstring.begin(), tempstring.end(), '\\', '/');
				strCode += tempstring;
				strCode += "/";
			}

			{
				stdfs::path fileName{};
				if (_desc.bEraseExtension)
				{
					fileName += m_vecFileName[i].replace_extension("");
				}
				else
				{
					fileName += m_vecFileName[i];
				}

				strCode += fileName.string();
			}



			strCode += "\";";


			_desc.codeWriter.WriteCode(0, strCode);
		}
	}

	//자식 노드가 있을 경우 재귀 호출 
	{
		size_t size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			HRESULT hr = m_vecChild[i]->WriteStrKeyTree(_desc);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
		}
	}


	//자식 네임스페이스를 작성했거나 루트 네임스페이스일 경우 중괄호를 닫아준다
	if (_desc.bWriteChildNamespace || IsRoot())
	{
		_desc.codeWriter.CloseBracket(0);
		_desc.codeWriter.WriteCode(0);
	}

	return S_OK;
}
