#include "JsonSerializer.h"

namespace ehw
{
	JsonSerializer::JsonSerializer()
		: m_jVal(Json::objectValue)
	{
	}

	JsonSerializer::~JsonSerializer()
	{
	}
	eResult JsonSerializer::SaveFile(std::filesystem::path const& _fullPath)
	{
		return eResult::Fail_NotImplemented;
	}

	eResult JsonSerializer::LoadFile(std::filesystem::path const& _fullPath)
	{
		std::string str = "123";
		int a = 3;
		auto pair = JsonPair("str", a);
		(*this) << JsonPair{ "str", a };

		std::vector<int> intvec = { 1, 2, 3 };
		(*this) << JsonPair("str", intvec);

		return eResult::Fail_NotImplemented;
	}

	void JsonSerializer::operator<<(JsonPair&& _jPair) noexcept
	{
		m_jVal[_jPair.strKey] = std::move(_jPair.data);
	}

	bool JsonSerializer::CheckValid(const std::string_view _strKey, bool _bCheckValueExist)
	{
		if (_strKey.empty())
		{
			ERROR_MESSAGE("스트링 키가 존재하지 않습니다.");
			return false;
		}
		else if (_bCheckValueExist && false == m_jVal.isMember(_strKey))
		{
			ERROR_MESSAGE("스트링 키에 대응되는 값이 존재하지 않습니다.");
			return false;
		}
		return true;
	}
}
