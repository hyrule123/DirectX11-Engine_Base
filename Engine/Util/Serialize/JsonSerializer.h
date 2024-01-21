#pragma once
#include "Serializer.h"

#include "json.h"
#include "../StringConverter.h"

#define MAKE_JSONPAIR(_var) JsonSerializer::JsonPair(#_var, _var)
#define JSON_KEY(_varName) #_varName

namespace ehw
{
	template <typename F> requires std::is_same_v<float, F>
	union FtoI { float f; int i; };

	template <typename F> requires std::is_same_v<double, F>
	union FtoI { double f; std::int64_t i; };

	template <typename T>
	concept JsonAllowedString =
		(std::is_same_v<const char*, T> || std::is_same_v<std::string, T> || std::is_same_v<std::string_view, T>);

	template <typename T>
	concept JsonAllowedTypes =
		(std::is_same_v<const char*, T> || NotPointerTypes<T>) &&
		std::is_integral_v<T> &&
		std::is_floating_point_v<T> &&
		std::is_;

	class JsonSerializer : public Serializer
	{
	public:
		JsonSerializer();
		virtual ~JsonSerializer();

		virtual eResult SaveFile(std::filesystem::path const& _fullPath) override;
		virtual eResult LoadFile(std::filesystem::path const& _fullPath) override;

		
		template <JsonAllowedTypes T>
		struct JsonPair
		{
			JsonPair(const std::string_view _strKey, T& _data)
				: strKey(_strKey)
				, data(_data)
			{}
			
			const std::string_view strKey;
			T& data;
		};


		template <JsonAllowedTypes T>
		static Json::Value Serialize(const T& _val);

		template <JsonAllowedTypes T>
		static T DeSerialize(const Json::Value& _jVal);

		template <JsonAllowedTypes T>
		bool operator << (const JsonPair<T>& _jPair);

		template <JsonAllowedTypes T>
		bool operator << (const JsonPair<std::vector<T>>& _jPair);

		template <JsonAllowedTypes T>
		bool operator >> (JsonPair<T>& _jPair);

		template <JsonAllowedTypes T>
		bool operator >> (JsonPair<std::vector<T>>& _jPair);
		

	private:
		inline bool CheckValid(const std::string_view _strKey, bool _bCheckValueExist);
		

	private:
		Json::Value m_jVal;
	};


	template<JsonAllowedTypes T>
	inline bool JsonSerializer::operator<<(const JsonPair<T>& _jPair)
	{
		if (false == CheckValid(_jPair.strKey, false))
		{
			return;
		}
		m_jVal[_jPair.strKey] = Serialize(_jPair.data);
	}

	template<JsonAllowedTypes T>
	inline bool JsonSerializer::operator<<(const JsonPair<std::vector<T>>& _jPair)
	{
		if (false == CheckValid(_jPair.strKey))
		{
			return false;
		}
		Json::Value arrVal = Json::Value(Json::arrayValue);
		for (size_t i = 0; i < _jPair.data.size(); ++i)
		{
			arrVal.append(Serialize(_jPair.data[i]));
		}

		m_jVal[_jPair.strKey] = std::move(arrVal);
		return true;
	}

	template<JsonAllowedTypes T>
	inline bool JsonSerializer::operator>>(JsonPair<T>& _jPair)
	{
		if (false == CheckValid(_jPair.strKey, true))
		{
			return false;
		}

		
		_jPair.data = DeSerialize<T>(m_jVal[_jPair.strKey]);
	}

	template<JsonAllowedTypes T>
	inline bool JsonSerializer::operator>>(JsonPair<std::vector<T>>& _jPair)
	{
		if (false == CheckValid(_jPair.strKey, true))
		{
			return;
		}

		const Json::Value& arrVal = m_jVal[_jPair.strKey];
		if (false == arrVal.isArray())
		{
			ERROR_MESSAGE("키값에 대응되는 데이터가 배열 형식이 아닙니다.");
			return;
		}

		auto iter = arrVal.begin();
		for (iter; iter != arrVal.end(); ++iter)
		{
			_jPair.data.emplace_back(DeSerialize<T>((*iter)));
		}
	}

	template<JsonAllowedTypes T>
	inline Json::Value JsonSerializer::Serialize(const T& _val)
	{
		Json::Value retJval;

		if constexpr ()
		else if constexpr (std::is_integral_v<T>)
		{
			retJval = _val;
		}
		else if constexpr (std::is_enum_v<T>)
		{
			retJval = (int)_val;
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			//int 형태로 저장
			FtoI<T> ftoi{};
			ftoi.f = _val;
			retJval = ftoi.i;
		}
		else if constexpr (JsonAllowedString<T>)
		{
			retJval = _val;
		}
		else
		{
			retJval = StringConverter::Base64Encode<T>(_val);
		}

		return retJval;
	}

	template<JsonAllowedTypes T>
	inline T JsonSerializer::DeSerialize(const Json::Value& _jVal)
	{
		T retVal{};

		if constexpr (std::is_integral_v<T>)
		{
			retVal = _jVal.as<T>();
		}
		else if constexpr (std::is_enum_v<T>)
		{
			retVal = static_cast<T>(_jVal.asInt());
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			//int 형태로 저장
			FtoI<T> ftoi{};
			ftoi.i = _jVal.as<T>();
			retVal = ftoi.f;
		}
		else if constexpr (JsonAllowedString<T>)
		{
			retVal = _jVal.as<T>;
		}
		else
		{
			retVal = StringConverter::Base64Decode<T>(_jVal.asString());
		}

		return retVal;
	}

}
