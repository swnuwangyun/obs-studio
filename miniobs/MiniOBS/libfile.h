#pragma once

#include <string>
#include <vector>

using std::wstring;
using std::string;
using std::vector;

namespace libfile
{
	bool isFileExist(const wstring& path);
	bool isDirectoryExist(const wstring& path);
	vector<wstring> getAllFilesInDirectory(const wstring& path);
	void createFile(const wstring& path);
	void deleteFile(const wstring& path);
	void copyFile(const wstring& src, const wstring& dst);
	void createFolder(LPCTSTR lpPathName);
	vector<wstring> getAllFolderInDirectory(const wstring& path);
	void deleteFolder(const wstring& path);
}