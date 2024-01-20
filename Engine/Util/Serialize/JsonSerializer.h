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
			JsonPair(const char* _strKey, T& _data)
				: strKey(_strKey)
				, data(_data)
			{}
			JsonPair(const std::string& _strKey, T& _data)
				: strKey(_strKey.c_str())
				, data(_data)
			{}
			
			const char* strKey;
			T& data;
		};


		template <NotPointerTypes T>
		static Json::Value Serialize(const T& _val);

		template <NotPointerTypes T>
		static T DeSerialize(const Json::Value& _jVal);

		void AddJsonValue(const std::string& _strKey, const Json::Value& _jVal) { m_jVal[_strKey] = _jVal; }
		void AddJsonValue(const std::string& _strKey, Json::Value&& _jVal) noexcept { m_jVal[_strKey] = std::move(_jVal); }

		template <NotPointerTypes T>
		void operator << (const JsonPair<T>& _jPair);

		template <NotPointerTypes T>
		void operator << (const JsonPair<std::vector<T>>& _jPair);


		//template <NotPointerTypes T>
		//void Write(const std::string& _strKey, const T& _val);

		//template <NotPointerTypes T>
		//void Write(const std::string& _strKey, const std::vector<T>& _vector);

		//template <NotPointerTypes T>
		//void Read(const std::string& _strKey, T& _val);

		////TODO: 이거 작성
		//template <NotPointerTypes T>
		//void Write(const std::string& _strKey, const std::vector<T>& _vector);

	private:
		Json::Value m_jVal;
	};


	template<NotPointerTypes T>
	inline void JsonSerializer::operator<<(const JsonPair<T>& _jPair)
	{
		if (nullptr == _jPair.strKey)
		{
			ERROR_MESSAGE("스트링 키가 존재하지 않습니다.");
			return;
		}

		m_jVal[_jPair.strKey] = Serialize(_jPair.data);
	}

	template<NotPointerTypes T>
	inline void JsonSerializer::operator<<(const JsonPair<std::vector<T>>& _jPair)
	{
		if (nullptr == _jPair.strKey)
		{
			ERROR_MESSAGE("스트링 키가 존재하지 않습니다.");
			return;
		}

		
	}

	//template<NotPointerTypes T>
	//inline void JsonSerializer::Write(const std::string& _strKey, const T& _val)
	//{
	//	
	//
	//	if (_strKey.empty())
	//	{
	//		ERROR_MESSAGE("스트링 키가 존재하지 않습니다.");
	//		return;
	//	}
	//
	//	m_jVal[_strKey] = GetNewJson(_val);
	//}
	//
	//template<NotPointerTypes T>
	//inline void JsonSerializer::Write(const std::string& _strKey, const std::vector<T>& _vector)
	//{
	//	if (_strKey.empty())
	//	{
	//		ERROR_MESSAGE("스트링 키가 존재하지 않습니다.");
	//		return;
	//	}
	//
	//	Json::Value vectorValue = Json::Value(Json::arrayValue);
	//
	//	for (size_t i = 0; i < _vector.size(); ++i)
	//	{
	//		vectorValue.append(GetNewJson(_vector[i]));
	//	}
	//
	//	m_jVal[_strKey] = std::move(vectorValue);
	//}
	//
	//template<NotPointerTypes T>
	//inline void JsonSerializer::Read(const std::string& _strKey, T& _val)
	//{
	//	if (_strKey.empty())
	//	{
	//		ERROR_MESSAGE("StrKey가 존재하지 않습니다.");
	//		return;
	//	}
	//	else if (false == m_jVal.isMember(_strKey))
	//	{
	//		ERROR_MESSAGE("지정한 StrKey에 대응하는 값을 찾을 수 없습니다.");
	//		return;
	//	}
	//
	//	_val = GetValueFromJson<T>(m_jVal[_strKey]);
	//}


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
}
