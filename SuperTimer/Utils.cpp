#include "stdafx.h"
#include "Utils.h"


Utils::Utils(void)
{
}


Utils::~Utils(void)
{
}

#define BUFFER_SIZE 100
std::string
	Utils::widechar2str(const wchar_t* pWchar)
{
	char* pBuffer = (char*)malloc(BUFFER_SIZE);
	size_t   i;
	memset(pBuffer, '\0', sizeof pBuffer);
	//std::wcstombs(buffer, pWchar, sizeof(buffer));
	wcstombs_s(&i, pBuffer, (size_t)BUFFER_SIZE, pWchar, (size_t)BUFFER_SIZE );

	std::string str(pBuffer);
	return str;
}

std::string 
	Utils::cstringToStd(const CString& cstr)
{
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString (cstr);
	// construct a std::string using the LPCSTR input
	std::string strStd (pszConvertedAnsiString);
	return strStd;
}

std::string 
	Utils::formatTime(UINT secs)
{
	UINT minutes = secs / 60;
	UINT seconds = secs % 60;

	char timeStr[8];
	_snprintf_s(timeStr, 7,"%d:%d",minutes,seconds);//7 + null terminator
	return std::string(timeStr);
}
