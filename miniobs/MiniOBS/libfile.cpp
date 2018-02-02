// libfile.cpp : Defines the exported functions for the DLL application.
//

#include <windows.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "libfile.h"
#include "libtext.h"
#include "shlwapi.h"

using namespace std;
#pragma comment(lib,"shlwapi.lib")

namespace libfile
{

bool isFileExist(const wstring& path)
{
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(path.c_str(), &fd); 
	if (hFind==INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return false;
	}
	else
	{
		FindClose(hFind);
		return true;
	}
}

bool isDirectoryExist(const wstring& path)
{
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(path.c_str(), &fd); 
	bool ret = false;
	if (hFind!=INVALID_HANDLE_VALUE && fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
		ret = true;
		FindClose(hFind);
	}
	return ret;
}

vector<wstring> getAllFilesInDirectory(const wstring& path)
{
	vector<wstring> ret;
	long  hFile  =  0; 
	struct _finddata_t fileinfo; 
	string p; 
	string apath = libtext::wstring2string(path);
	if((hFile = _findfirst(p.assign(apath).append("\\*").c_str(),&fileinfo)) != -1) 
	{ 
		do
		{  
			if((fileinfo.attrib & _A_SUBDIR)) 
			{
			} 
			else
			{
				string s = p.assign(apath).append("\\").append(fileinfo.name);
				ret.push_back(libtext::string2wstring(s)); 
			} 
		}
		while(_findnext(hFile, &fileinfo) == 0); 

		_findclose(hFile); 
	}
	return ret;
}

vector<wstring> getAllFolderInDirectory(const wstring& path)
{
	vector<wstring> ret;
	long  hFile  =  0; 
	struct _finddata_t fileinfo; 
	string p; 
	string apath = libtext::wstring2string(path);
	if((hFile = _findfirst(p.assign(apath).append("\\*").c_str(),&fileinfo)) != -1) 
	{ 
		do
		{  
			if((fileinfo.attrib & _A_SUBDIR)) 
			{
				if(strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0) 
				{
					string s = p.assign(apath).append("\\").append(fileinfo.name);
					ret.push_back(libtext::string2wstring(s));
				}
			} 
			else
			{
			} 
		}
		while(_findnext(hFile, &fileinfo) == 0); 

		_findclose(hFile); 
	}
	return ret;
}

void deleteFolder(const wstring& folderPath)
{
	long  hFile  =  0; 
	struct _finddata_t fileinfo; 
	string p; 
	string apath = libtext::wstring2string(folderPath);
	if((hFile = _findfirst(p.assign(apath).append("\\*").c_str(),&fileinfo)) != -1) 
	{ 
		do
		{  
			if((fileinfo.attrib & _A_SUBDIR)) 
			{
				if(strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0) 
				{
					deleteFolder(folderPath + L"\\" + libtext::string2wstring(fileinfo.name));
				}
			} 
			else
			{
				deleteFile(folderPath + L"\\" + libtext::string2wstring(fileinfo.name));
			} 
		}
		while(_findnext(hFile, &fileinfo) == 0); 
		RemoveDirectory(folderPath.c_str());
		_findclose(hFile); 
	}
}

void createFile(const wstring& path)
{
	wofstream logger(path.c_str(), ios::app);
}

void deleteFile(const wstring& path)
{
	DeleteFile(path.c_str());
}

void copyFile(const wstring& src, const wstring& dst)
{
	CopyFile(src.c_str(), dst.c_str(), false);
}

void createFolder(LPCTSTR lpPathName)
{
	if (!PathIsDirectory(lpPathName))
	{
		::CreateDirectory(lpPathName, NULL);
	}
}

}