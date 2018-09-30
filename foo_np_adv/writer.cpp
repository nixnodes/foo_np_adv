#include "stdafx.h"

#include <codecvt>
#include <string>

using namespace std;

condition_variable IWriter::cv_quit;
mutex IWriter::cvq_mutex;
atomic<bool> IWriter::p_Destroy{ false };
CWriter *IWriter::m_Writer = nullptr;

locale CWriter::lmap[ENCODING_COUNT] = {
	locale{},
	locale(locale{},
		new codecvt_utf8<wchar_t, 0x10ffff>),
	locale(locale{},
		new codecvt_utf8<wchar_t, 0x10ffff, generate_header>),
	locale(locale{},
		new codecvt_utf16<wchar_t, 0x10ffff>),
	locale(locale{},
		new codecvt_utf16<wchar_t, 0x10ffff, little_endian>),
	locale(locale{},
		new codecvt_utf16<wchar_t, 0x10ffff, generate_header>),
	locale(locale{},
		new codecvt_utf16<wchar_t, 0x10ffff, (codecvt_mode)(generate_header | little_endian)>)
};

using namespace std;

void CWriter::worker()
{
	while (true)
	{
		const write_job j = q.pop();
		if (j.flags & F_WRITER_ABORT) {
			break;
		}
		Write(j);
	}
}

static wstring widen(const pfc::string8 &utf8) {
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convert;
	return convert.from_bytes(utf8);
}

static string unicode2ansi(const wstring &wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

void CWriter::Write(const write_job &j)
{
	int flags = ios::out;
	if (j.flags & F_WRITER_APPEND) {
		flags |= ios::app;
	}
	else {
		flags |= ios::trunc;
	}

	try {
		if (j.encoding == ENCODING_ANSI) {
			fstream fs(j.file, flags);
			fs << unicode2ansi(widen(j.data));
			fs.close();
		}
		else {
			wfstream fs;
			fs.imbue(CWriter::lmap[j.encoding]);
			fs.open(j.file, flags);
			fs << widen(j.data);
			fs.close();
		}
	}
	catch (exception const & e) {
		console::complain("NPA: write failed", e);
	}
}

void CWriter::QueueWrite(const write_job &j)
{
	try {
		q.push(j);
	}
	catch (std::exception &e) {
		console::complain("NPA: unable to push write job", e);
	}
}