#pragma once
#include <string>


class StringConv
{
public:
	static void ConvertUTF8ToUnicode(__in const std::string_view _src, __out std::wstring& _dest);
	static void ConvertUnicodeToUTF8(__in const std::wstring_view _src, __out std::string& _dest);

	static std::string ConvertUnicodeToUTF8(const std::wstring_view _src);
	static std::wstring ConvertUTF8ToUnicode(const std::string_view _src);

	//새 string을 만들어서 return 해줌
	inline static std::string UpperCaseReturn(const std::string_view _str);
	//새 wstring을 만들어서 return 해줌
	inline static std::wstring UpperCaseReturn(const std::wstring_view _wstr);

	inline static std::string& UpperCase(std::string& _str);
	inline static std::wstring& UpperCase(std::wstring& _wstr);

	//T 타입 데이터를 String으로 저장
	template <typename T>
	inline static std::string Convert_T_to_String(const T& _srcT);

	template <typename T>
	inline static void Convert_T_to_String(const T& _srcT, std::string& _destT);

	template <typename T>
	inline static T Convert_String_to_T(const std::string& _srcStr);

	template <typename T>
	inline static bool Convert_String_to_T(const std::string& _srcStr, const T& _destT);

private:
	StringConv() = delete;
	~StringConv() = delete;
};

inline std::string StringConv::UpperCaseReturn(const std::string_view _str)
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

inline std::wstring StringConv::UpperCaseReturn(const std::wstring_view _wstr)
{
	std::wstring converted(_wstr);

	size_t size = sizeof(converted);
	for (size_t i = 0; i < size; ++i)
	{
		converted[i] = (wchar_t)std::toupper((wchar_t)converted[i]);
	}

	return converted;
}

inline std::string& StringConv::UpperCase(std::string& _str)
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

inline std::wstring& StringConv::UpperCase(std::wstring& _wstr)
{
	size_t size = _wstr.size();
	for (size_t i = 0; i < size; ++i)
	{
		_wstr[i] = (wchar_t)std::toupper((int)_wstr[i]);
	}

	return _wstr;
}

template<typename T>
inline std::string StringConv::Convert_T_to_String(const T& _SrcT)
{
	return std::string((const char*)&_SrcT, sizeof(T));
}

template<typename T>
inline void StringConv::Convert_T_to_String(const T& _srcT, std::string& _destT)
{
	_destT = std::string((const char*)&_srcT, sizeof(T));
}

template<typename T>
inline T StringConv::Convert_String_to_T(const std::string& _SrcStr)
{
	T ReturnResult;
	memset(&ReturnResult, 0, sizeof(T));

	if (_SrcStr.size() >= sizeof(T))
	{
		memcpy((void*)&ReturnResult, _SrcStr.data(), sizeof(T));
	}

	return ReturnResult;
}

template<typename T>
inline bool StringConv::Convert_String_to_T(const std::string& _SrcStr, const T& _DestT)
{
	memset((void*)&_DestT, 0, sizeof(T));

	bool Result = false;
	if (_SrcStr.size() >= sizeof(T))
	{
		memcpy((void*)&_DestT, _SrcStr.data(), sizeof(T));
		Result = true;
	}

	return Result;
}
