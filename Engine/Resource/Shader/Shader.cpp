#include "Engine/PCH_Engine.h"
#include "Shader.h"

#include "Engine/Util/Serialize/BinarySerializer.h"

namespace core
{
	Shader::Shader()
		: Resource()
	{
	}
	Shader::~Shader()
	{
	}
	eResult Shader::save(const std::fs::path& _full_path) const
	{
		ERROR_MESSAGE("저장 미지원");
		return eResult::Fail;
	}
	eResult Shader::load(const std::fs::path& _full_path)
	{
		return load_file_binary(_full_path);
	}
	eResult Shader::serialize_json(JsonSerializer& _ser) const
	{
		return eResult::Fail;
	}
	eResult Shader::deserialize_json(const JsonSerializer& _ser)
	{
		return eResult::Fail;
	}
	eResult Shader::serialize_binary(BinarySerializer& _ser) const
	{
		return eResult::Fail;
	}
	eResult Shader::deserialize_binary(const BinarySerializer& _ser)
	{
		if (false == load_shader(_ser.get_data()))
		{
			eResult::Fail;
		}
		return eResult::Success;
	}
}
