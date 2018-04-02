#pragma once

#include <string>
#include <vector>
#include <map>

namespace libtext
{
	using std::string;
	using std::wstring;
	using std::vector;
	using std::map;

	vector<wstring> split(const wstring &str, const wstring &delimiter);
	wstring format(wstring fmt, ...);
	string format(string fmt, ...);

	wstring Int64ToWString(__int64 v);
	__int64 wstringToInt64(const wstring& str);
	wstring IntToWString(int v);
	int wstringToInt(const wstring& str);
	wstring UIntToWString(unsigned int v);
	//��wstringת��Ϊunsinged int,�˷����Ὣ�ַ���"12345ddd"ת��Ϊ����12345����ʱ��ʹ����һ��wstringToUInt����
	unsigned int wstringToUInt(const wstring& str);
	//��wstringת��Ϊunsinged int���˷���ת���ַ���"12345ddd"ʱ������ʧ��false
	bool wstringToUInt(const wstring& str, unsigned int* outResult);
	wstring DoubleToWString(double v);
	double wstringToDouble(wstring str);

	wstring string2wstring(const string &str);
	string wstring2string(const wstring &wstr);
	string UTF8_To_string(const string & str);
	string string_To_UTF8(const string & str);
	wstring UTF8_To_wstring(const string & str);
	string wstring_To_UTF8(const wstring & str);

	bool contains(const wstring& str, const wstring& substr);
	bool startWith(const wstring& str, const wstring& with);
	bool endWith(const wstring& str, const wstring& with);
	wstring trim(const wstring& str);
	wstring trimLeft(const wstring& str, const wstring& trimstr);
	wstring trimRight(const wstring& str, const wstring& trimstr);

	wstring toUpper(wstring str);
	wstring toLower(wstring str);

	wstring replace(const wstring& str, const wstring& src, const wstring& dest);

	wstring startWString(const wstring& str, const int num);
	wstring endWString(const wstring& str, const int num);

	wstring readFromFile(wstring path);
	wstring readFromFileAsSingleLine(wstring path);
	void writeToFile(wstring str, wstring path);

	map<wstring, wstring> readKeyValuesFromFile(wstring path);
	void writeKeyValuesToFile(wstring path, map<wstring, wstring> dict);

	string string_to_URLEncode2(const string& str);
	//����rtsp��url����url�е��û�����������ȡ������������һ���µĲ������û��������url
	bool rtsp_url_password(const string& url, string& name, string& password, string& newUrl);
}