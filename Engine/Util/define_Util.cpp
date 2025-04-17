#include "define_Util.h"

#include "Engine/Util/StringConverter.h"

#include "Engine/GameEngine.h"






std::filesystem::path core::WinAPI::FileDialog(const std::filesystem::path& _baseDirectory, const std::filesystem::path& _extension)
{
	std::vector<std::filesystem::path> extensions = { _extension };
	return FileDialog(_baseDirectory, extensions);
}

std::filesystem::path core::WinAPI::FileDialog(const std::filesystem::path& _baseDirectory, const std::vector<std::filesystem::path>& _extensions)
{
	//풀경로를 받아올 주소 변수를 만들어주고
	std::wstring stringPath;

	//프로그램 주소를 넣어놓는다.
	stringPath = _baseDirectory.wstring();
	std::wstring extension{};

	//파일명이 안붙어있을 경우 가상의 파일명을 생성
	if (false == _baseDirectory.has_extension())
	{
		stringPath += L"\\file";
	}
	else
	{
		extension = _baseDirectory.extension().wstring();
		StringConverter::MakeUpperCase(extension);
	}
	
	stringPath.resize(MAX_PATH);

	//파일 열기 창에 전달할 설정 구조체를 설정해준다.
	OPENFILENAMEW OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);	//구조체 크기
	OpenFile.hwndOwner = core::GameEngine::get_inst().GetHwnd();	//관리 핸들

	std::vector<wchar_t> extensionFilters;
	for (size_t i = 0; i < _extensions.size(); ++i)
	{
		//해당 확장자의 이름 설정
		const std::wstring& ext = _extensions[i].wstring();
		std::copy(ext.begin(), ext.end(), std::back_inserter(extensionFilters));
		std::wstring fileName = TEXT(" File");
		std::copy(fileName.begin(), fileName.end(), std::back_inserter(extensionFilters));

		//확장자 포맷 설정
		extensionFilters.push_back(L'\0');

		extensionFilters.push_back(L'*');
		
		std::copy(ext.begin(), ext.end(), std::back_inserter(extensionFilters));
		extensionFilters.push_back(L'\0');

		//기본 확장자가 들어왔을 경우 해당 확장자와 일치하는 필터를 기본 표시되도록 설정
		if (false == extension.empty() && extension == StringConverter::UpperCase(_extensions[i].wstring()))
		{
			//인덱스번호는 1을 더해줘야함(1부터 시작함)
			OpenFile.nFilterIndex = (DWORD)i + (DWORD)1u;
		}
	}
	std::wstring allFiles = L"All Files";
	std::copy(allFiles.begin(), allFiles.end(), std::back_inserter(extensionFilters));
	extensionFilters.push_back(L'\0');
	allFiles = L"*.*";
	std::copy(allFiles.begin(), allFiles.end(), std::back_inserter(extensionFilters));

	extensionFilters.push_back(L'\0');
	extensionFilters.push_back(L'\0');

	OpenFile.lpstrFilter = extensionFilters.data();
	OpenFile.lpstrFile = stringPath.data();	//경로가 저장될 변수 주소
	OpenFile.nMaxFile = MAX_PATH;	//최대 경로 자릿수

	//이 플래그를 넣어주지 않으면 std::current_path가 불러온 주소로 바껴버림
	OpenFile.Flags = OFN_NOCHANGEDIR;

	if ((DWORD)0 == OpenFile.nFilterIndex)
	{
		OpenFile.nFilterIndex = (DWORD)_extensions.size() + (DWORD)1;
	}

	//만들어진 풀 경로를 FullPath에 보낸다.
	if (FALSE == GetOpenFileNameW(&OpenFile))
	{
		stringPath.clear();
	}


	size_t pos = stringPath.find(L'\0');
	if (pos != std::wstring::npos)
	{
		stringPath.resize(pos);
	}

	std::filesystem::path retPath = stringPath;
	//필터가 설정되어 있을 경우 확장자를 입력해준다
	if ((UINT)(OpenFile.nFilterIndex) - 1u  < (UINT)_extensions.size())
	{
		retPath.replace_extension(_extensions[(UINT)OpenFile.nFilterIndex - 1u]);
	}

	return retPath;
}
