#pragma once

#include <string>
#include <sstream>
#include <cstdio>

class Utils
{
public:
	Utils(void);
	~Utils(void);

	static std::string widechar2str(const wchar_t* pWchar);
	static std::string cstringToStd(const CString& cstr);
	static std::string formatTime(UINT secs);
	//模版函数声明处
	template <typename T>
	static T stringToNumber( const std::string &text )//text not by const reference so that the function can be used with a character array as argument
	{
		std::stringstream ss(text);
		T result;
		return ss >> result ? result : 0;
	};
	template <typename T>
	static std::string numberToString ( T num )
	{
		std::stringstream ss;
		ss << num;
		return ss.str();
	};
};

