#include "Engine/Resource/Shader/Shader.h"

ehw::Shader::Shader(const std::string_view key, const std::type_info& _typeInfo)
	: Resource(key, _typeInfo)
{
}

ehw::Shader::~Shader()
{
}
