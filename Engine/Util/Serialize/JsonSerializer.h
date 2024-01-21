#pragma once
#include "Serializer.h"

#include "json.h"
#include "../StringConverter.h"

#define MAKE_JSONPAIR(_var) JsonSerializer::JsonPair(#_var, _var)

namespace ehw
{
	template <typename F> requires std::is_same_v<float, F>
	union FtoI { float f; int i; };

	template <typename F> requires std::is_same_v<double, F>
	union FtoI { double f; std::int64_t i; };

	class JsonSerializer : public Serializer
	{
	public:
		JsonSerializer() {};
		virtual ~JsonSerializer() {};

		virtual eResult SaveFile(std::filesystem::path const& _fullPath) override;
		virtual eResult LoadFile(std::filesystem::path const& _fullPath) override;

		template <NotPointerTypes T>
		struct JsonPair
		{
			JsonPair(const std::string_view _strKey, T& _data)
				: strKey(_strKey)
				, data(_data)
			{}
			
			const std::string_view strKey;
			T& data;
		};


		template <NotPointerTypes T>
		static Json::Value Serialize(const T& _val);

		template <NotPointerTypes T>
		static T DeSerialize(const Json::Value& _jVal);

		template <NotPointerTypes T>
		void operator << (const JsonPair<T>& _jPair);

		template <NotPointerTypes T>
		void operator << (const JsonPair<std::vector<T>>& _jPair);

		template <NotPointerTypes T>
		void operator >> (JsonPair<T>& _jPair);

		template <NotPointerTypes T>
		void operator >> (JsonPair<std::vector<T>>& _jPair);

	private:
		inline bool CheckStringValid(const std::string_view _strKey);
		inline bool CheckValueExists(const std::string_view _strKey);
		

	private:
		Json::Value m_jVal;
	};


	template<NotPointerTypes T>
	inline void JsonSerializer::operator<<(const JsonPair<T>& _jPair)
	{
		if (false == CheckStringValid(_jPair.strKey))
		{
			return;
		}

		m_jVal[_jPair.strKey] = Serialize(_jPair.data);
	}

	template<NotPointerTypes T>
	inline void JsonSerializer::operator<<(const JsonPair<std::vector<T>>& _jPair)
	{
		if (false == CheckStringValid(_jPair.strKey))
		{
			return;
		}

		Json::Value arrVal = Json::Value(Json::arrayValue);
		for (size_t i = 0; i < _jPair.data.size(); ++i)
		{
			arrVal.append(Serialize(_jPair.data[i]));
		}

		m_jVal[_jPair.strKey] = std::move(arrVal);
	}

	template<NotPointerTypes T>
	inline void JsonSerializer::operator>>(JsonPair<T>& _jPair)
	{
		if (false == CheckStringValid(_jPair.strKey) || false == CheckValueExists(_jPair.strKey))
		{
			return;
		}

		
		_jPair.data = DeSerialize<T>(m_jVal[_jPair.strKey]);
	}

	template<NotPointerTypes T>
	inline void JsonSerializer::operator>>(JsonPair<std::vector<T>>& _jPair)
	{
		if (false == CheckStringValid(_jPair.strKey) || false == CheckValueExists(_jPair.strKey))
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

	template<NotPointerTypes T>
	inline Json::Value JsonSerializer::Serialize(const T& _val)
	{
		Json::Value retJval;

		if constexpr (std::is_integral_v<T>)
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
		else if constexpr (std::is_same_v<std::string, T>)
		{
			retJval = _val;
		}
		else
		{
			retJval = StringConverter::Base64Encode<T>(_val);
		}

		return retJval;
	}

	template<NotPointerTypes T>
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
		else if constexpr (std::is_same_v<std::string, T>)
		{
			retVal = _jVal.as<T>;
		}
		else
		{
			retVal = StringConverter::Base64Decode<T>(_jVal.asString());
		}

		return retVal;
	}

	inline bool JsonSerializer::CheckStringValid(const std::string_view _strKey)
	{
		bool ret = !_strKey.empty();
		if (false == ret)
		{
			ERROR_MESSAGE("스트링 키가 존재하지 않습니다.");
		}
		return ret;
	}

	inline bool JsonSerializer::CheckValueExists(const std::string_view _strKey)
	{
		bool ret = m_jVal.isMember(_strKey);
		if (false == ret)
		{
			ERROR_MESSAGE("스트링 키에 대응되는 값이 없습니다.");
		}
		return ret;
	}
}
