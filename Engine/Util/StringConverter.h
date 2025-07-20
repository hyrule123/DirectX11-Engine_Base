#pragma once
#include <Engine/Util/Serialize/base64.h>

#include <string>

class StringConverter
{
public:
	// UTF8 <-> Unicode
	static void UTF8_to_UTF16(__in const std::string_view _src, __out std::wstring& _dest);
	static void UTF16_to_UTF8(__in const std::wstring_view _src, __out std::string& _dest);
	static std::string UTF16_to_UTF8(const std::wstring_view _src);
	static std::wstring UTF8_to_UTF16(const std::string_view _src);

	// ANSI <-> UniCode
	static void UTF16_to_ANSI(__in const std::wstring_view _src, __out std::string& _dest);
	static void ANSI_to_UTF16(__in const std::string_view _src, __out std::wstring& _dest);
	static std::wstring ANSI_to_UTF16(const std::string_view _src);
	static std::string UTF16_to_ANSI(const std::wstring_view _src);

	// Strlen(UTF-8 Support)
	static size_t UTF8_strlen(const std::string_view _str);

	//새 string을 만들어서 return 해줌
	inline static std::string upper_case(const std::string_view _str);
	//새 wstring을 만들어서 return 해줌
	inline static std::wstring upper_case(const std::wstring_view _wstr);

	inline static void make_upper_case(std::string& _str);
	inline static void make_upper_case(std::wstring& _wstr);

	//T 타입 데이터를 String으로 저장
	template <typename T>
	inline static std::string base64_encode(const T& _srcT);

	//type specialization을 위해서 위 함수와 똑같이 const reference로 받아준다.
	template <>
	inline static std::string base64_encode(const std::wstring_view& _srcT);

	template <typename T>
	inline static T base64_decode(const std::string_view _srcStr);


private:
	StringConverter() = delete;
	~StringConverter() = delete;
};

inline std::string StringConverter::upper_case(const std::string_view _str)
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

inline std::wstring StringConverter::upper_case(const std::wstring_view _wstr)
{
	std::wstring converted(_wstr);

	size_t size = converted.size();
	for (size_t i = 0; i < size; ++i)
	{
		converted[i] = (wchar_t)std::toupper((wchar_t)converted[i]);
	}

	return converted;
}

inline void StringConverter::make_upper_case(std::string& _str)
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
}

inline void StringConverter::make_upper_case(std::wstring& _wstr)
{
	size_t size = _wstr.size();
	for (size_t i = 0; i < size; ++i)
	{
		_wstr[i] = (wchar_t)std::toupper((int)_wstr[i]);
	}
}

template<typename T>
inline std::string StringConverter::base64_encode(const T& _srcT)
{
	return base64_encode((const unsigned char*)&_srcT, sizeof(T));
}

template<>
inline std::string StringConverter::base64_encode(const std::wstring_view& _srcT)
{
	return Base64::encode(reinterpret_cast<const unsigned char*>(_srcT.data()), (unsigned int)_srcT.size() * 2u);
}


template<typename T>
inline T StringConverter::base64_decode(const std::string_view _srcStr)
{
	T returnResult{};

	base64_decode(_srcStr, reinterpret_cast<unsigned char*>(&returnResult), sizeof(returnResult));

	return returnResult;
}
