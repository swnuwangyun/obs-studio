#ifndef _LIB_LIBLOG_H_
#define _LIB_LIBLOG_H_

#include <string>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

namespace liblog
{
	class Log
	{
	public:
		static void cleanupLogFiles(wstring path, wstring prefix);
		static void i(const wstring& tag, const wstring& msg, int rate=0, const wstring& key=L"");
		static void d(const wstring& tag, const wstring& msg, int rate=0, const wstring& key=L"");
		static void w(const wstring& tag, const wstring& msg, int rate=0, const wstring& key=L"");
		static void e(const wstring& tag, const wstring& msg, int rate=0, const wstring& key=L"");

	private:
		static void internalLog(const wstring &tag, const wstring& level, const wstring &msg, int &rate, const wstring &key);
	};
}

#endif //_LIB_LIBLOG_H_