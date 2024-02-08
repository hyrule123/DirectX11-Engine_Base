#pragma once
#include "Serializer.h"

#include "../../CommonGlobalVar.h"
#include "../StringConverter.h"
#include "json.h"


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
		const Json::Value& operator[] (const std::string_view _strKey) const { return m_jVal[_strKey]; };
		bool isMember(const std::string_view _strKey) const { return m_jVal.isMember(_strKey); }

	private:
		Json::Value m_jVal;
	};

	//템플릿 특수화를 하고자 하는 타입은 여기 추가 후 특수화해주면 됨.
	template <typename T>
	concept JsonTypeSpecialization = 
		type_traits_Ex::PointerTypes<T> ||
		type_traits_Ex::is_enum_class_v<T>;

	template <typename T>
	concept JsonDefaultTypes =
		!JsonTypeSpecialization<T> &&
		requires (T t) { { Json::Value(t) }; };

	template <typename T>
	concept NotJsonDefaultTypes =
		!JsonTypeSpecialization<T> &&
		!requires (T t) { { Json::Value(t) }; };

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
	inline void operator <<(Json::Value& _jVal, const T& _data)
	{
		_jVal = StringConverter::Base64Encode(_data);
	}
	
	//c_str
	template <JsonTypeSpecialization T> requires type_traits_Ex::PointerTypes<T>
	inline void operator <<(Json::Value& _jVal, T _c_str)
	{
		static_assert(std::is_same_v(T, char), "c_str 문자열만 사용 가능");
		if (nullptr != _c_str)
		{
			_jVal = Json::StaticString(_c_str);
		}
	}

	//enum class
	template <JsonTypeSpecialization T> requires type_traits_Ex::is_enum_class_v<T>
	inline void operator <<(Json::Value& _jVal, T _data)
	{
		_jVal = static_cast<std::underlying_type_t<T>>(_data);
	}

	template <JsonDefaultTypes T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = _jVal.as<T>();
	}

	template <NotJsonDefaultTypes T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = StringConverter::Base64Decode<T>(_jVal.asString());
	}


	template <JsonTypeSpecialization T> requires type_traits_Ex::is_enum_class_v<T>
	inline void operator >>(const Json::Value& _jVal, T& _data)
	{
		_data = static_cast<T>(_jVal.as<std::underlying_type_t<T>>());
	}

	template <JsonTypeSpecialization T> requires type_traits_Ex::PointerTypes<T>
	inline void operator >> (const Json::Value& _jVal, T _c_str)
	{
		static_assert(std::is_same_v<T, const char*>, "c_str 포인터(const char*)만 지원함.");
		const auto& pair = g_cstrArchive.insert(_jVal.asString());
		_c_str = pair.first->c_str();
	}
}
