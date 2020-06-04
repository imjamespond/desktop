#pragma once
#include <string>
#include <vector>

namespace km
{

	typedef std::vector<CString> Row;
	typedef std::vector<Row> Result;

class Http
{
public:
	Http();

	static Result Test(CString, CString searchText = L"%E8%8B%B9%E6%9E%9C");
	static std::string GetReq(LPCTSTR , LPCTSTR );
};

}

