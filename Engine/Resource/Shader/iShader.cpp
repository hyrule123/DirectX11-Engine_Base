#include "Engine/Resource/Shader/iShader.h"

ehw::iShader::iShader(const std::type_info& _typeInfo)
	: iResource(_typeInfo)
{
}

ehw::iShader::~iShader()
{
}
