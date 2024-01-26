#include "JsonSerializer.h"


JsonSerializer::JsonSerializer()
	: m_jVal(Json::objectValue)
{
}

JsonSerializer::~JsonSerializer()
{
}
bool JsonSerializer::SaveFile(std::filesystem::path const& _fullPath)
{
	return false;
}

bool JsonSerializer::LoadFile(std::filesystem::path const& _fullPath)
{
	return false;
}
