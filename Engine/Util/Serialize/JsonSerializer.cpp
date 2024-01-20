#include "JsonSerializer.h"

namespace ehw
{
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

}
