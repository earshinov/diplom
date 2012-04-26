#include "unicode.h"

#ifdef _WIN32

#include <vector>

#include <Windows.h>

class UnicodeIOException : public std::exception {
public:

	UnicodeIOException() { }
	explicit UnicodeIOException(const std::string & message) : message(message) { }
	explicit UnicodeIOException(std::string && message) : message(message) { }
	virtual ~UnicodeIOException() throw() { }

	/* override */ const char * what() const throw() { return message.empty() ? 0 : message.c_str(); }

private:

	std::string message;
};

static std::string getLastErrorDescription() {
	char * buf;
	DWORD val = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, GetLastError(), 0, (LPSTR)(&buf), 0, NULL);
	if (val == 0)
		return std::string();
	std::string ret(buf);
	LocalFree(buf);
	return ret;
}

std::string U(const std::string & str) {
	if (str.empty())
		return str;

	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
	if (len == 0)
		throw UnicodeIOException("Unicode IO error: " + getLastErrorDescription());
	std::vector<wchar_t> buf(len);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), &buf.front(), buf.size());

	unsigned int codepage = GetConsoleOutputCP();
	len = WideCharToMultiByte(codepage, 0, &buf.front(), buf.size(), NULL, 0, NULL, NULL);
	if (len == 0)
		throw UnicodeIOException("Unicode IO error: " + getLastErrorDescription());
	std::vector<char> buf2(len);
	WideCharToMultiByte(codepage, 0, &buf.front(), buf.size(), &buf2.front(), buf2.size(), NULL, NULL);

	return std::string(buf2.begin(), buf2.end());
}

#endif