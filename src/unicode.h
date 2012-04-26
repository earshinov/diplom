#pragma once

#include <string>

#ifdef _WIN32
std::string U(const std::string & str);
#else
inline std::string U(const std::string & str) { return str; }
#endif