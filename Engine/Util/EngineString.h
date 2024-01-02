#pragma once

#include "base64.h"

#include <string>

//내부적으로는 전부 utf-8을 사용한다
class EngineString
	: public std::string
{
public:
	//기본 생성자 그대로 사용
	using std::string::string;

	//Constructor
	EngineString(const std::string_view _str);
	EngineString(const std::wstring_view _wstr);


	//새 객체를 만들어 리턴
	EngineString UpperCase() { return EngineString(StaticUpperCase(String())); };

	//기존 객체를 변경
	EngineString& MakeUpperCase()
	{
		EngineString& ret = *this;
		StaticMakeUpperCase(StringRef());
		return ret;
	}
		
#pragma region Multibyte
	//자신의 string 부분을 반환
	const std::string& String() { return *static_cast<std::string*>(this); }


	inline static std::string& StaticMakeUpperCase(std::string& _str);
	inline static std::string StaticUpperCase(const std::string_view _str);

	//T 타입 데이터를 EngineString으로 저장
	template <typename T>
	inline static std::string StaticBase64Encode(const T& _srcT);

	template <typename T>
	inline static T StaticBase64Decode(const std::string& _srcStr);
#pragma endregion Multibyte


#pragma region Unicode Conversion
	//Constructor, assign operator
	EngineString(const std::wstring& _wstr);
	void operator=(const std::wstring& _wstr);

	std::wstring WString();

	// ANSI <-> UniCode
	static std::wstring StaticConvertANSIToUnicode(const std::string_view _src);
	static std::string StaticConvertUnicodeToANSI(const std::wstring_view _src);

	//Unicode <-> UTF8
	static std::string StaticConvertUnicodeToUTF8(const std::wstring_view _src);
	static std::wstring StaticConvertUTF8ToUnicode(const std::string_view _src);

	inline static std::wstring StaticUpperCase(const std::wstring_view _wstr);
	inline static std::wstring& StaticMakeUpperCase(std::wstring& _str);
#pragma endregion Unicode Conversion

private:
	std::string& StringRef() { return *static_cast<std::string*>(this); }
};



inline std::string EngineString::StaticUpperCase(const std::string_view _str)
{
	std::string converted(_str);

	size_t size = converted.size();
	for (size_t i = 0; i < size; ++i)
	{
		//첫 번째 비트가 0일 경우 ASCII 코드
		if ((char)0 == (converted[i] >> 7))
		{
			converted[i] = std::toupper(converted[i]);
		}
	}

	return converted;
}

inline std::wstring EngineString::StaticUpperCase(const std::wstring_view _wstr)
{
	std::wstring converted(_wstr);

	size_t size = converted.size();
	for (size_t i = 0; i < size; ++i)
	{
		converted[i] = (wchar_t)std::toupper((wchar_t)converted[i]);
	}

	return converted;
}

inline std::string& EngineString::StaticMakeUpperCase(std::string& _str)
{
	size_t size = _str.size();
	for (size_t i = 0; i < size; ++i)
	{
		//첫 번째 비트가 0일 경우 ASCII 코드
		if ((char)0 == (_str[i] >> 7))
		{
			_str[i] = std::toupper(_str[i]);
		}
	}

	return _str;
}

inline std::wstring& EngineString::StaticMakeUpperCase(std::wstring& _wstr)
{
	size_t size = _wstr.size();
	for (size_t i = 0; i < size; ++i)
	{
		_wstr[i] = (wchar_t)std::toupper((int)_wstr[i]);
	}

	return _wstr;
}

template<typename T>
inline std::string EngineString::StaticBase64Encode(const T& _srcT)
{
	return base64_encode((const unsigned char*)&_srcT, sizeof(T));
}

template<typename T>
inline T EngineString::StaticBase64Decode(const std::string& _srcStr)
{
	T ReturnResult{};

	std::string decoded = base64_decode(_srcStr);
	memcpy_s((void*)&ReturnResult, sizeof(T), decoded.c_str(), decoded.size());

	return ReturnResult;
}
