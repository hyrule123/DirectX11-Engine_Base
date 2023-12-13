#include "PCH_CodeGenerator.h"
#include "DirTree.h"



#include "../Engine/Resource/define_Resource.h"
#include "../Engine/Resource/Shader/GraphicsShader.h"
#include "../Engine/Resource/Shader/iComputeShader.h"


#include "define_Util.h"
#include "json-cpp/json.h"
#include "CodeWriter.h"
#include "DirTreeNode.h"

DirTree::DirTree()
	: m_RootDir()
	, m_PrevFiles()
	, m_PrevFilesSavePath()
	, m_bModified()
{
}

DirTree::~DirTree()
{
}

HRESULT DirTree::SearchRecursive(const tSearchDesc& _desc)
{
	HRESULT hr{};

	Reset();

	m_PrevFilesSavePath = _desc.rootPath / _desc.prevTextFileName;
	m_PrevFilesSavePath.replace_extension("txt");
	//이전 파일 목록을 읽은뒤
	m_PrevFiles = ReadPrevFilesList(m_PrevFilesSavePath);

	//desc를 작성한 후
	DirTreeNode::SearchDesc searchSetting(_desc.rootPath, _desc.regex, m_PrevFiles);

	//실제로 폴더를 재귀형태로 탐색
	hr = m_RootDir.SearchRecursive(searchSetting, _desc.rootPath);

	//실패시 리턴
	if (FAILED(hr))
	{
		ClearPrevFilesList(m_PrevFilesSavePath);
		return hr;
	}

	//완료 되었으면 이전 검사결과와 변경점을 확인
	m_bModified = CheckDiff();

	//이전 파일 목록을 갱신
	WritePrevFilesList(m_PrevFilesSavePath);

	return hr;
}


HRESULT DirTree::CreateStrKeyHeader(const tStrKeyWriteDesc& _desc)
{
	//변경점이 없으면 작성하지 않는다.
	if (false == m_bModified)
	{
		return S_OK;
	}

	CodeWriter Writer;

	Writer.WriteCode(0, define_Preset::Keyword::PragmaOnce::A);
	Writer.WriteCode(0, define_Preset::Keyword::Head::A);

	Writer.WriteCode(0, define_Preset::Keyword::DefineSTRKEY::A);

	//루트 노드의 경우 이름도 알맞게 바꿔줘야 함.
	Writer.WriteCode(0);
	Writer.WriteCode(0);

	std::string strCode = "namespace ehw::strKey::";
	strCode += _desc.rootNamespace;
	Writer.WriteCode(0, strCode);

	DirTreeNode::tStrKeyWriteDesc desc(Writer, _desc.bEraseExtension, _desc.bWriteChildNamespace, _desc.bAddRelativeDirToString);

	HRESULT hr = m_RootDir.WriteStrKeyTree(desc);
	if (FAILED(hr))
		return hr;

	hr = Writer.SaveAll<char>(_desc.filePath);
	if (FAILED(hr))
	{
		//저장 실패 시 txt 파일 내용도 제거
		ClearPrevFilesList(m_PrevFilesSavePath);
	}

	return hr;
}




HRESULT DirTree::CreateComponentManagerInitCode(tAddBaseClassDesc const& _Desc)
{
	if (false == m_bModified)
	{
		return S_OK;
	}


	CodeWriter Writer;

	Writer.WriteCode(0, define_Preset::Keyword::Head::A);


	{
		Writer.WriteCode(0, _Desc.IncludePCH);
		Writer.WriteCode(0);
		
		std::string includeHeader = "#include \"";
		includeHeader += _Desc.ClassName;
		includeHeader += R"(.h")";
		Writer.WriteCode(0, includeHeader);

		Writer.WriteCode(0, _Desc.IncludeManagerHeader);
	}
	{
		Writer.WriteCode(0, _Desc.IncludeStrKeyHeaderName);
	}

	{
		Writer.WriteCode(0, _Desc.IncludeBaseTypeHeader);
		Writer.WriteCode(0);
	}

	//1번 버퍼에 CS 생성 코드를 작성
	{
		Writer.WriteCode(1, "");

		{
			//CONSTRUCTOR_T(T) 
			std::string strCode = define_Preset::Keyword::Define_Constructor_T::A;
			strCode += _Desc.Constructor_T_MacroDefine;
			Writer.WriteCode(1, strCode);

			Writer.WriteCode(1, "");
		}

		{
			Writer.WriteCode(1, _Desc.MasterNamespace);
			Writer.OpenBracket(1);
			Writer.WriteCode(1, _Desc.UsingNamespace);
		}

		std::string strCode = "void ";
		strCode += _Desc.ClassName;
		strCode += "::";
		strCode += _Desc.UserClassMgr_InitFuncName;

		Writer.WriteCode(1, strCode);
		Writer.OpenBracket(1);
	}
	
	//순회를 돌면서 각 버퍼에 코드 작성
	//0번 버퍼: include
	//1번 버퍼: 클래스 생성
	for (const auto& iter : m_PrevFiles)
	{
		//0번 버퍼에 include 작성
		{
			std::string strCode;
			strCode += define_Preset::Keyword::IncludeBegin::A;
			strCode += iter.first.string();
			strCode += "\"";
			Writer.WriteCode(0, strCode);
		}

		//1번 버퍼에 클래스 생성 코드 작성
		{
			const std::string& ClassName = iter.first.filename().replace_extension("").string();

			std::string strCode;
			strCode += define_Preset::Keyword::Constructor_T::A;
			strCode += ClassName;
			strCode += ");";
			Writer.WriteCode(1, strCode);
		}
	}

	Writer.CloseBracket(1, false);
	Writer.CloseBracket(1, false);


	HRESULT hr = Writer.SaveAll<char>(_Desc.FilePath);
	if (FAILED(hr))
	{
		ClearPrevFilesList(_Desc.FilePath);
		DEBUG_BREAK;
		return hr;
	}




	return hr;
}





HRESULT DirTree::CreateShaderStrKey(stdfs::path const& _FilePath)
{
	if (false == m_bModified)
	{
		return S_OK;
	}


	CodeWriter Writer;

	std::unordered_map<stdfs::path, tShaderGroup> umapGSGroup;

	std::vector<stdfs::path> vecCS;
	std::regex CSRegex(define_Preset::Regex::CShaderRegex::A);
	


	//파일 순회돌면서 그래픽 쉐이더 파일 정리
	for(const auto& iter: m_PrevFiles)
	{
		//std::string으로 변경
		std::string strFileName = iter.first.string();

		bool bIsGS = false;
		for (int j = 0; j < (int)ehw::eGSStage::END; ++j)
		{
			size_t Pos = strFileName.find(ehw::strKey::ArrGSPrefix[j]);
			if (std::string::npos != Pos)
			{
				//쉐이더별로 지정한 접두사가 있을 경우 해당 접두사를 제거한다.
				strFileName.erase(0, Pos + strlen(ehw::strKey::ArrGSPrefix[j]));

				//파일의 확장자를 json으로 변경하고, 키값으로 사용.
				stdfs::path ShaderGroupName = strFileName;
				ShaderGroupName.replace_extension(ehw::strKey::Ext_ShaderSetting);

				//쉐이더 그룹명에 각 쉐이더 파이프라인의 데이터를 담은 파일의 이름을 추가한다.
				umapGSGroup[ShaderGroupName].FileName[j] = iter.first;

				bIsGS = true;

				break;
			}
		}

		//그래픽 쉐이더가 아닐 경우 컴퓨트쉐이더인지 검사
		
		if (false == bIsGS)
		{
			//컴퓨트쉐이더일 경우 컴퓨트쉐이더 벡터에 값을 추가
			if(std::regex_search(strFileName, CSRegex))
			{
				vecCS.push_back(stdfs::path(iter.first).replace_extension());
			}
		}
	}


	//코드 작성 시작
	Writer.WriteCode(0, define_Preset::Keyword::PragmaOnce::A);
	Writer.WriteCode(0, define_Preset::Keyword::Head::A);
	Writer.WriteCode(0, define_Preset::Keyword::DefineSTRKEY::A);

	Writer.WriteCode(0);
	Writer.WriteCode(0);

	{
		std::string strCode;
		strCode +=  "namespace ";
		strCode += define_Preset::Keyword::strKey::A;
		strCode += "Shader";
		Writer.WriteCode(0, strCode);
		Writer.OpenBracket(0);
	}

	{
		std::string strCode;
		strCode += "namespace Graphics";
		Writer.WriteCode(0, strCode);
		Writer.OpenBracket(0);
	}

	for (const auto& iter : umapGSGroup)
	{
		std::string strCode;
		strCode +=  "STRKEY ";
		strCode += iter.first.filename().replace_extension("").string();
		strCode += " = \"";
		strCode += iter.first.string();
		strCode += "\";";
		Writer.WriteCode(0, strCode);
	}
	Writer.CloseBracket(0, true);

	{
		Writer.AddIndentation(1);
		std::string strCode;
		strCode += "namespace Compute";
		Writer.WriteCode(1, strCode);
		Writer.OpenBracket(1);
	}

	for (size_t i = 0; i < vecCS.size(); ++i)
	{
		std::string strCode;
		strCode += "STRKEY ";
		strCode += vecCS[i].filename().replace_extension("").string();
		strCode += " = \"";
		strCode += vecCS[i].filename().replace_extension(".cso").string();
		strCode += "\";";
		Writer.WriteCode(1, strCode);
	}
	Writer.CloseBracket(1, true);
	Writer.CloseBracket(1);

	//Writer.CloseBracketAll(0);

	return Writer.SaveAll<char>(_FilePath);
}

std::unordered_map<stdfs::path, ePrevFileStatus> DirTree::ReadPrevFilesList(const stdfs::path& _filePath)
{
	std::unordered_map<stdfs::path, ePrevFileStatus> prevFiles{};

	std::ifstream ifs(_filePath);
	if (ifs.is_open())
	{
		std::string fileName;
		while (std::getline(ifs, fileName))
		{	
			prevFiles.insert(std::make_pair(stdfs::path(fileName), ePrevFileStatus::NotFound));
		}

		ifs.close();
	}


	return prevFiles;
}

bool DirTree::CheckDiff()
{
	bool bModified = false;

	//비어있을 경우에는 무조건 작성
	//비어있을 경우에는 텍스트파일도 비어있음 -> 비어있을 경우에는 코드 작성이 안 되어있는 상태더라도 새로 작성하지 않음
	//->에러 발생
	if (m_PrevFiles.empty())
	{
		bModified = true;
	}

	else
	{
		for (auto iter = m_PrevFiles.begin(); iter != m_PrevFiles.end();)
		{
			switch (iter->second)
			{
			case ePrevFileStatus::NotFound:
			{
				bModified = true;
				iter = m_PrevFiles.erase(iter);
				continue;
			}

			case ePrevFileStatus::Found:
			{
				++iter;
				continue;
			}

			case ePrevFileStatus::New:
			{
				bModified = true;
				++iter;
				continue;
			}

			default:
				ASSERT(false, "이상한 값이 들어왔습니다");
				break;
			}
		}
	}

	return bModified;
}

void DirTree::WritePrevFilesList(const stdfs::path& _filePath)
{
	std::ofstream ofs(_filePath);
	ASSERT(ofs.is_open(), "txt 파일 쓰기 모드로 열기 실패.");

	//이전 파일 리스트가 비어있을 경우에는 무조건 새로 작성한다.
	//이전 파일 리스트가 없고, 새로 발견된 파일이 없으면 갱신을 안하는 경우가 있음.
	//그런데 이 때 코드 파일에는 지웠던 파일들이 있는 경우가 있음 -> 에러 발생
	for (const auto& iter : m_PrevFiles)
	{
		ofs << iter.first.string() << std::endl;
	}

	ofs.close();
}






namespace ehw
{
	eResult SaveJson(Json::Value* _pjVal)
	{
		return eResult();
	}

	eResult LoadJson(const Json::Value* _pjVal)
	{
		return eResult();
	}
}
