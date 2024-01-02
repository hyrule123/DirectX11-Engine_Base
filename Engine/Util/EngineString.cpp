#include "EngineString.h"

#include <Windows.h>


EngineString::EngineString(const std::string_view _str)
	: std::string(_str)
{
}

EngineString::EngineString(const std::wstring_view _wstr)
	: std::string(StaticConvertUnicodeToUTF8(_wstr))
{
}

EngineString::EngineString(const std::wstring& _wstr)
	: std::string(StaticConvertUnicodeToUTF8(_wstr))
{
}

void EngineString::operator=(const std::wstring& _wstr)
{
	std::string* pStr = static_cast<std::string*>(this);
	(*pStr) = StaticConvertUnicodeToUTF8(_wstr);
}

std::wstring EngineString::WString()
{
	std::string* pStr = static_cast<std::string*>(this);
	return StaticConvertUTF8ToUnicode(*pStr);
}


std::wstring EngineString::StaticConvertUTF8ToUnicode(const std::string_view _src)
{
	std::wstring result;

	int srcsize = (int)_src.size();
	int len = ::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, nullptr, 0);


	result.resize(len);
	::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size());

	return result;
}



std::string EngineString::StaticConvertUnicodeToUTF8(const std::wstring_view _src)
{
	std::string result;

	int srcsize = (int)_src.size();
	int len = ::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

	result.resize(len);

	::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, result.data(), (int)result.size(), nullptr, nullptr);

	return result;
}

std::wstring EngineString::StaticConvertANSIToUnicode(const std::string_view _src)
{
	std::wstring result;

	int srcsize = (int)_src.size();
	int len = ::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, nullptr, 0);

	result.resize(len);
	::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size());

	return result;
}

std::string EngineString::StaticConvertUnicodeToANSI(const std::wstring_view _src)
{
	std::string result;

	int srcsize = (int)_src.size();
	int len = ::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

	result.resize(len);

	::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size(), nullptr, nullptr);

	return result;
}
