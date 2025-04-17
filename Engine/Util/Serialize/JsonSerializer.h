#pragma once
#include "Engine/GlobalVariable.h"
#include "Engine/Common.h"

#include "Engine/Util/type_traits_Ex.h"
#include "Engine/Util/StringConverter.h"

#include "Engine/Util/Serialize/Serializable.h"
#include "Engine/Util/Serialize/json.h"

//CPP에서 include 후 사용해주면 됨(특정 타입을 Json::value 형태로 변환하기 위한 전역 함수 모음임)

namespace core
{
	//템플릿을 통한 1차 타입 분류
	//Json 기본 생성자에서 지원하는 타입의 경우 그대로 추가하고,
	//미지원 타입인 경우 JsonSerializer에서 오버로드된 함수를 호출함.
	//템플릿 특수화를 하고자 하는 타입은 여기 추가 후 특수화해주면 됨.
	template <typename T>
	concept JsonTypeSpecialization =
		type_traits_Ex::PointerTypes<T> ||
		type_traits_Ex::is_enum_class_v<T> ||
		std::is_same_v<float3, T> ||
		std::is_same_v<float4, T> ||
		std::is_same_v<MATRIX, T>;

	template <typename T>
	concept JsonDefaultTypes =
		(!JsonTypeSpecialization<T>) &&
		(requires (T t) { { Json::Value(t) }; });

	template <typename T>
	concept JsonBase64Types =
		(!JsonTypeSpecialization<T>) &&
		(!requires (T t) { { Json::Value(t) }; });

	//template <JsonDefaultTypes T>
	//inline void operator <<(Json::Value& _jVal, const T& _data)
	//{
	//	_jVal = _data;
	//}

	//Template 함수에서 &&는 '보편적 레퍼런스(Universal Reference)'이다.
	//&&을 뗀 T 형태가 우측 레퍼런스일 경우 우측 레퍼런스로, 그렇지 않을 경우 좌측 레퍼런스로 전달한다.
	//https://modoocode.com/228
	template <JsonDefaultTypes T>
	inline void operator <<(Json::Value& _jVal, T&& _data) noexcept
	{
		_jVal = std::forward<T>(_data);
	}

	//특수화하지 않았을 경우 타입을 가리지 않고 포인터 제외 데이터를 Base64로 전환하므로 주의할것.
	//컨테이너는 StringConverter에서 함수를 별도로 만들어줄것.
	//또한 혹시 별도의 처리를 해야 하는 데이터 타입이 있을 경우 이 함수에 대해 템플릿 특수화를 해줄것.
	template <JsonBase64Types T>
	inline void operator <<(Json::Value& _jVal, const T& _data)
	{
		_jVal = StringConverter::Base64Encode(_data);
	}
	

	//enum class
	template <JsonTypeSpecialization T> requires std::is_enum_v<T>
	inline void operator <<(Json::Value& _jVal, T _data)
	{
		_jVal = static_cast<std::underlying_type_t<T>>(_data);
	}

	template <JsonDefaultTypes T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		if constexpr (std::is_enum_v<T>)
		{
			_data = static_cast<T>(_jVal.as<std::underlying_type_t<T>>());
		}
		else
		{
			_data = _jVal.as<T>();
		}
		
	}

	template <JsonBase64Types T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = StringConverter::Base64Decode<T>(_jVal.asString());
	}


	template <JsonTypeSpecialization T> requires std::is_enum_v<T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = static_cast<T>(_jVal.as<std::underlying_type_t<T>>());
	}


#pragma region 특수화
	//c_str
	void operator << (Json::Value& _jVal, const char* _c_str);
	void operator >> (const Json::Value& _jVal, const char* _c_str);

	//float3
	void operator << (Json::Value& _jVal, const float3& _f3);
	void operator >> (const Json::Value& _jVal, float3& _f3);

	//float4
	void operator << (Json::Value& _jVal, const float4& _f4);
	void operator >> (const Json::Value& _jVal, float4& _f4);

	//MATRIX
	void operator << (Json::Value& _jVal, const MATRIX& _m);
	void operator >> (const Json::Value& _jVal, MATRIX& _m);
#pragma endregion //특수화
}
