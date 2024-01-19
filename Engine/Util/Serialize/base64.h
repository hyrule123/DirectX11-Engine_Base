#pragma once
#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(const std::string_view s);

