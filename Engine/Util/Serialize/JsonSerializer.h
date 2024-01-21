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


	class JsonSerializer : public Serializer
	{
	public:
		JsonSerializer();
		virtual ~JsonSerializer();

		virtual eResult SaveFile(std::filesystem::path const& _fullPath) override;
		virtual eResult LoadFile(std::filesystem::path const& _fullPath) override;
		
		Json::Value& operator[] (const std::string_view _strKey) { return m_jVal[_strKey]; }
		

		template <NotPointerTypes T>
		static Json::Value SerializeBase64(const T& _val)
		{
			return Json::Value(StringConverter::Base64Encode(_val));
		}

		template <typename T> requires std::is_integral_v<T>
		static Json::Value Serialize(T _i) { return Json::Value(_i); }

		template <typename T> requires std::is_integral_v<T>
		static void DeSerialize(const Json::Value& _jVal, T& _retType) { retType = _jVal.as<T>; }

		template <typename T> requires std::is_floating_point_v<T>
		static Json::Value Serialize(T _f)
		{
			Json::Value ret{};
			FtoI<T> conv{};
			conv.f = _f;
			ret = conv.i;

			return ret;
		}
		template <typename T> requires std::is_floating_point_v<T>
		static T DeSerialize(const Json::Value& _jVal, const T& _retType)
		{
			T ret{};
			FtoI<T> conv{};
			conv.i = _jVal.as<decltype(conv.i)>;
			ret = conv.f;
			return ret;
		}

		static Json::Value Serialize(const std::string_view _val) { return Json::Value(_val); };
		static std::string DeSerialize(const Json::Value& _jVal, const std::string& _retType) { return _jVal.asString(); };

		static Json::Value Serialize(const std::wstring_view _val)
		{
			Json::Value ret{};
			if (false == _val.empty())
			{
				ret = base64_encode(reinterpret_cast<const unsigned char*>(_val.data()),
					static_cast<unsigned int>(_val.size()) * 2u);
			}
			return ret;
		}
		static std::wstring DeSerialize(const Json::Value& _jVal)
		{

		}


		

	private:
		inline bool CheckValid(const std::string_view _strKey, bool _bCheckValueExist);
		

	private:
		Json::Value m_jVal;
	};

}
