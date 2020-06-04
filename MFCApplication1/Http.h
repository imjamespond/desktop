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

	static Result Test();
	static std::string GetReq(LPCTSTR , LPCTSTR );
};

}

