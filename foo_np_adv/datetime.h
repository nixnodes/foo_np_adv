#pragma once

#include <map>
#include <time.h>

class CDateTime {
public:
	pfc::string8 datetime() 
	{
		return fmt("%c");
	}
	pfc::string8 time() 
	{
		return fmt("%T");
	}
	pfc::string8 fmt(pfc::string8 fmt) 
	{
		datetime(fmt);
		return pfc::string8(buffer);
	}

	static bool is_fs_allowed(char c)
	{
		return allowed_fmt_specifiers.count(c) > 0;
	}
private:
	size_t datetime(const char *fmt) 
	{
		time_t rawtime;
		struct tm timeinfo;

		::time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		return strftime(buffer, sizeof(buffer), fmt, &timeinfo);
	}

	char buffer[256];

	static std::map<char, bool> allowed_fmt_specifiers;
};