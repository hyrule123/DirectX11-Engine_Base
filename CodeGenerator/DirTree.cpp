#include "PCH_CodeGenerator.h"
#include "DirTree.h"

#include "define_Util.h"
#include "json-cpp/json.h"
#include "CodeWriter.h"
#include "DirTreeNode.h"

#include "../Engine/define_Res.h"
#include "../Engine/GraphicsShader.h"
#include "../Engine/iComputeShader.h"

DirTree::DirTree()
	: m_RootDir()
{
}

DirTree::~DirTree()
{
}

HRESULT DirTree::SearchRecursive(stdfs::path const& _RootPath, std::regex const& _regex)
{
	m_RootDir.Clear();

	return m_RootDir.SearchRecursive( _RootPath, _RootPath, _regex);
}


HRESULT DirTree::CreateStrKeyHeader(stdfs::path const& _FilePath, stdfs::path const& _RootNamespace, bool _bEraseExtension)
{
	CodeWriter Writer;

	Writer.WriteCode(0, define_Preset::Keyword::PragmaOnce::A);
	Writer.WriteCode(0, define_Preset::Keyword::Head::A);

	Writer.WriteCode(0, define_Preset::Keyword::DefineSTRKEY::A);

	//루트 노드의 경우 이름도 알맞게 바꿔줘야 함.
	Writer.WriteCode(0);
	Writer.WriteCode(0);

	std::string strCode = "namespace ehw::strKey::";
	strCode += _RootNamespace.string();
	Writer.WriteCode(0, strCode);

	//이전에 작성했던 파일 리스트가 있는지 확인 및 리스트 로드
	std::unordered_map<stdfs::path, ePrevFileStatus> prevFiles = ReadPrevFilesList(_FilePath);

	HRESULT hr = m_RootDir.WriteStrKeyTree(Writer, _bEraseExtension, prevFiles);
	if (FAILED(hr))
		return hr;

	////////////////////////////////////////////////////////
	//찾은 파일 리스트 저장 및 새로운 파일 발견여부 확인
	bool bModified = CheckDiffAndWritePrevFiles(_FilePath, prevFiles);
	////////////////////////////////////////////////////


	//새 파일 발견되었을 경우 저장
	hr = S_OK;
	if (bModified)
	{
		hr = Writer.SaveAll<char>(_FilePath);
		if (FAILED(hr))
		{
			//저장 실패 시 txt 파일 내용도 제거
			ClearPrevFilesList(_FilePath);
		}
	}
	return hr;
}




HRESULT DirTree::CreateComMgrInitCode(tAddBaseClassDesc const& _Desc)
{
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
		//Writer.WriteCode("cUserClassMgr* pMgr = cUserClassMgr::GetInst();", 1);
	}

	auto prevFiles = ReadPrevFilesList(_Desc.FilePath);
	//노드를 순회돌면서 이름을 정리시킨다.
	m_RootDir.GetAllFiles(prevFiles, false);

	//새 파일 발견했는지 여부 확인. 없을 경우 return
	bool bNewFileDetected = CheckDiffAndWritePrevFiles(_Desc.FilePath, prevFiles);
	if (false == bNewFileDetected)
		return S_OK;
	
	
	//순회를 돌면서 각 버퍼에 코드 작성
	//0번 버퍼: include
	//1번 버퍼: 클래스 생성
	for (const auto& iter : prevFiles)
	{
		//0번 버퍼에 include 작성
		{
			const std::string& FileName = iter.first.filename().string();

			std::string strCode;
			strCode += define_Preset::Keyword::IncludeBegin::A;
			strCode += FileName + ".h";
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
	CodeWriter Writer;

	auto prevFiles = ReadPrevFilesList(_FilePath);
	
	HRESULT hr = m_RootDir.GetAllFiles(prevFiles, false);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	//갱신된 파일이 없을 경우 return
	if (false == CheckDiffAndWritePrevFiles(_FilePath, prevFiles))
	{
		return S_OK;
	}

	std::unordered_map<stdfs::path, tShaderGroup> umapGSGroup;
	std::vector<stdfs::path> vecCS;

	std::regex CSRegex(define_Preset::Regex::CShaderRegex::A);


	//파일 순회돌면서 그래픽 쉐이더 파일 정리
	for(const auto& iter: prevFiles)
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

	std::ifstream ifs(stdfs::path(_filePath).replace_extension(".txt"));
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

bool DirTree::CheckDiffAndWritePrevFiles(const stdfs::path& _filePath, std::unordered_map<stdfs::path, ePrevFileStatus>& _prevFilesList)
{
	bool bModified = false;

	stdfs::path txtPath = stdfs::path(_filePath).replace_extension(".txt");

	//타겟 파일 또는 txt 파일이 존재하지 않을 경우 무조건 생성
	if (false == stdfs::exists(_filePath) || false == stdfs::exists(txtPath))
	{
		bModified = true;
	}

	std::ofstream ofs(txtPath);
	ASSERT(ofs.is_open(), "txt 파일 쓰기 모드로 열기 실패.");

	//이전 파일 리스트가 비어있을 경우에는 무조건 새로 작성한다.
	//이전 파일 리스트가 없고, 새로 발견된 파일이 없으면 갱신을 안하는 경우가 있음.
	//그런데 이 때 코드 파일에는 지웠던 파일들이 있는 경우가 있음 -> 에러 발생
	if (_prevFilesList.empty())
	{
		bModified = true;
	}
	else
	{
		for (auto iter = _prevFilesList.begin(); iter != _prevFilesList.end();)
		{
			switch (iter->second)
			{
			//이전에 있었는데 현재 없을 경우 -> 변경된 것임
			case ePrevFileStatus::NotFound:
			{
				iter = _prevFilesList.erase(iter);
				bModified = true;
			}
			break;

			case ePrevFileStatus::Found:
			{
				ofs << iter->first.string() << std::endl;
				++iter;
			}
			break;

			case ePrevFileStatus::New:
			{
				//새 파일을 발견했는지 확인(하나라도 New == 새 파일 발견)
				bModified = true;

				ofs << iter->first.string() << std::endl;
				++iter;
			}
			break;

			default:
				assert(false);
				break;
			}
		}
	}
	ofs.close();

	return bModified;
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
