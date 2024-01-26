#pragma once
#include "Serializer.h"

#include "json.h"
#include "../StringConverter.h"

#define MAKE_JSONPAIR(_var) JsonSerializer::JsonPair(#_var, _var)
#define JSON_KEY(_varName) #_varName

namespace ehw
{
	class JsonSerializer : public Serializer
	{
	public:
		JsonSerializer();
		virtual ~JsonSerializer();

		virtual eResult SaveFile(std::filesystem::path const& _fullPath) override;
		virtual eResult LoadFile(std::filesystem::path const& _fullPath) override;

		Json::Value& operator[] (const std::string_view _strKey) { return m_jVal[_strKey]; }

	private:
		Json::Value m_jVal;
	};



	template <typename T>
	concept JsonDefaultTypes =
		!type_traits_Ex::PointerTypes<T> &&
		requires (T t)
	{
		{ Json::Value(t) };
	};
	template <typename T>
	concept NotJsonDefaultTypes =
		!type_traits_Ex::PointerTypes<T> &&
		!requires (T t)
	{
		{ Json::Value(t) };
	};


	template <JsonDefaultTypes T>
	inline void operator <<(Json::Value& _jVal, const T& _data)
	{
		_jVal = _data;
	}
	template <JsonDefaultTypes T>
	inline void operator <<(Json::Value& _jVal, T&& _data) noexcept
	{
		_jVal = std::move(_data);
	}

	//타입을 가리지 않고 포인터 제외 데이터를 Base64로 전환하므로 주의할것.
	//컨테이너는 StringConverter에서 함수를 별도로 만들어줄것.
	//또한 혹시 별도의 처리를 해야 하는 데이터 타입이 있을 경우 이 함수에 대해 템플릿 특수화를 해줄것.
	template <NotJsonDefaultTypes T>
	requires (!std::is_enum_v<T>)
	inline void operator <<(Json::Value& _jVal, const T& _data)
	{
		_jVal = StringConverter::Base64Encode(_data);
	}

	template <NotJsonDefaultTypes T>
	requires std::is_enum_v<T>
	inline void operator <<(Json::Value& _jVal, T _data)
	{
		//https://stackoverflow.com/questions/8357240/how-to-automatically-convert-strongly-typed-enum-into-int
		_jVal = static_cast<std::underlying_type_t<T>>(_data);
	}

	inline void operator <<(Json::Value& _jVal, const char* _data)
	{
		if (nullptr != _data)
		{
			_jVal = Json::StaticString(_data);
		}
	}





	template <JsonDefaultTypes T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = _jVal.as<T>();
	}

	template <NotJsonDefaultTypes T>
		requires (!std::is_enum_v<T>)
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = StringConverter::Base64Decode<T>(_jVal.asString());
	}

	template <NotJsonDefaultTypes T>
		requires std::is_enum_v<T>
	inline void operator <<(const Json::Value& _jVal, T& _data)
	{
		//https://stackoverflow.com/questions/8357240/how-to-automatically-convert-strongly-typed-enum-into-int
		_data = static_cast<T>(_jVal.as<std::underlying_type_t<T>>());
	}
}


