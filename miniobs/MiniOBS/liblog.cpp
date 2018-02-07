// liblog.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <time.h>
#include <Windows.h>
#include <list>
#include "liblog.h"
#include "libpath.h"
#include "libtext.h"
#include "libfile.h"

using namespace std;

namespace liblog
{

static map<wstring, int> s_ds;
static wstring log_path;
static wstring log_file_name_prefix;

void Log::cleanupLogFiles(wstring the_log_path, wstring the_log_file_name_prefix)
{
	log_path = the_log_path;
	log_file_name_prefix = the_log_file_name_prefix;

	libfile::createFolder(log_path.c_str());

	wstring path = log_path;
	vector<wstring> files = libfile::getAllFilesInDirectory(path);

	list<wstring> logFiles;
	for (size_t i = 0; i < files.size(); i++)
	{
		wstring d = files[i];
		if(libtext::endWith(d, L".log"))
			logFiles.push_back(d);
	}

	while(logFiles.size()>10)
	{
		wstring s = logFiles.front();
		Log::i(L"Log", libtext::format(L"Delete log file %s", s.c_str()));
		libfile::deleteFile(s);
		logFiles.pop_front();
	}
}

void Log::i(const wstring& tag, const wstring& msg, int rate, const wstring& key)
{
	internalLog(tag, L"I", msg, rate, key);
}

void Log::d(const wstring& tag, const wstring& msg, int rate, const wstring& key)
{
	internalLog(tag, L"D", msg, rate, key);
}

void Log::w(const wstring& tag, const wstring& msg, int rate, const wstring& key)
{
	internalLog(tag, L"W", msg, rate, key);
}

void Log::e(const wstring& tag, const wstring& msg, int rate, const wstring& key)
{
	internalLog(tag, L"E", msg, rate, key);
}

void Log::internalLog(const wstring &tag, const wstring& level, const wstring &msg, int &rate, const wstring &key)
{
	{
		bool flag = false;

		if(rate)
		{
			wstring fullKey = tag + key;
			if(s_ds.find(fullKey)==s_ds.end())
			{
				s_ds[fullKey] = 0;
			}
			int counter = s_ds[fullKey];
			counter++;
			s_ds[fullKey] = counter;

			if(counter%rate==0)
			{
				flag = true;
			}
		}
		else
		{
			flag = true;
		}

		if(flag)
		{
			
			wstring path = log_path;
			wstring name = L"";
			{
				time_t t = time(NULL);
				struct tm* ts_info;
				char buffer[26];
				ts_info = localtime(&t);
				strftime(buffer, 26, "%Y%m%d", ts_info);
				name = log_file_name_prefix + libtext::string2wstring("_" + string(buffer) + "_1.log");
			}
			CreateDirectory(path.c_str(), NULL);
			path = libpath::combine(path, name);

			wofstream logger(path.c_str(), ios::app);
			logger.imbue(locale("", locale::all ^ locale::numeric)); 

			time_t t = time(NULL);
			struct tm* ts_info;
			char buffer[26];
			ts_info = localtime(&t);
			strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", ts_info);

			logger<<::GetCurrentProcessId()<<L" "<<GetCurrentThreadId()<<L" "<<buffer<<L" "<<GetTickCount()<<L" /"<<level<<L" "<<tag<<L"\t"<<key<<msg<<endl;
		}
	}
}

}