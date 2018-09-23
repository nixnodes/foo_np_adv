#include "stdafx.h"
#include "writer.h"

#include <thread>
#include <string>
#include <codecvt>
#include <fstream>

using namespace std;

condition_variable IWriter::cv_quit;
mutex IWriter::cvq_mutex;
atomic<int> IWriter::p_Destroy{ 0 };

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


void CWriter::worker()
{
	while (true)
	{
		write_job j = q.pop();
		if (j.flags & F_WRITER_ABORT) {
			break;
		}
		Write(&j);
	}
}

wstring widen(pfc::string8 utf8) {
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convert;
	return convert.from_bytes(utf8);
}

string unicode2ansi(const wstring &wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

void CWriter::Write(write_job *j) {
	try {
		int flags = ios::out;
		if (j->flags & F_WRITER_APPEND) {
			flags |= ios::app;
		}
		else {
			flags |= ios::trunc;
		}

		if (j->encoding == ENCODING_CP1252) {
			fstream fs;
			fs.open(j->file, flags);
			fs << unicode2ansi(widen(j->data));
			fs.close();
		}
		else {
			wfstream fs;
			fs.imbue(CWriter::lmap[j->encoding]);
			fs.open(j->file, flags);
			fs << widen(j->data);
			fs.close();
		}
	}
	catch (exception const & e) {
		console::complain("NPA: write failed", e);
	}
}
