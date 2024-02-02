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
		return eResult::Fail_NotImplemented;
	}

}

