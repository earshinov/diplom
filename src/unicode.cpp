#include "unicode.h"

#ifdef _WIN32

#include <vector>

#include <Windows.h>

std::string U(const std::string & str) {
	if (str.empty())
		return str;

	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
	std::vector<wchar_t> buf(len);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), &buf.front(), buf.size());

	unsigned int codepage = GetConsoleOutputCP();
	len = WideCharToMultiByte(codepage, 0, &buf.front(), buf.size(), NULL, 0, NULL, NULL);
	std::vector<char> buf2(len);
	WideCharToMultiByte(codepage, 0, &buf.front(), buf.size(), &buf2.front(), buf2.size(), NULL, NULL);

	return std::string(buf2.begin(), buf2.end());
}

#endif