#pragma once
#include <string>

class Base64
{
public:
	static std::string encode(unsigned char const*, unsigned int len);
	static std::string decode(const std::string_view s);
};


