#include "pch.h"
#include "Http.h"

#include <afxinet.h>
#include <codecvt>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


// Short alias for this namespace
namespace pt = boost::property_tree; 

using namespace km;
using namespace std;


Http::Http()
{
}

Result Http::Test(CString strServer, CString searchText)
{
    CString strObjectName = L"";
    strObjectName.Format(L"/sug?code=utf-8&q=%s", searchText);
    string jsonStr = Http::GetReq(strServer.GetString(), strObjectName.GetString());

    //http://www.cochoy.fr/boost-property-tree/
    //https://blog.csdn.net/yqmfly/article/details/6914590
    //string jsonStr = R"({"people": [
    //    { "firstName": "Brett", "lastName" : "McLaughlin", "email" : "aaaa" },
    //    { "firstName": "Jason", "lastName" : "Hunter", "email" : "bbbb"},
    //    { "firstName": "Elliotte", "lastName" : "Harold", "email" : "cccc" }
    //]})";
    //string stra, strc; 
    pt::ptree root, p1, p2, p3;
    stringstream jsonStream;

    jsonStream << jsonStr;
    pt::read_json<pt::ptree>(jsonStream, root);
    //p1 = root.get_child("people");
    //for (pt::ptree::iterator it = p1.begin(); it != p1.end(); ++it)
    //{
    //    p2 = it->second; //first为空
    //    stra = p2.get<string>("firstName");
    //    TRACE("%s\n",stra.c_str());
    //}

    Result list;

    p1 = root.get_child("result");
    for (pt::ptree::iterator it = p1.begin(); it != p1.end(); ++it)
    {
        Row row;

        p2 = it->second;
        for (pt::ptree::iterator it2 = p2.begin(); it2 != p2.end(); ++it2)
        { 
            p3 = it2->second;
            string str = p3.data();

            std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
            wstring wstr = wconv.from_bytes(str.c_str()).c_str();
            TRACE(L"%s\n", wstr.c_str());
             
            row.push_back(wstr.c_str());
        }
        list.push_back(row);
    }

    return list;
}

string Http::GetReq(LPCTSTR pstrServer, LPCTSTR pstrObjectName)
{

    string result;

    // 通过 http GET 协议来获取并保存文件
    CInternetSession session;
    session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * 20);
    session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000);
    session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

    CHttpConnection* pConnection = session.GetHttpConnection(pstrServer, (INTERNET_PORT)443);
    CHttpFile* pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, pstrObjectName);

    CString szHeaders = L"Accept: audio/x-aiff, audio/basic, audio/midi,\
                                  audio/mpeg, audio/wav, image/jpeg, image/gif, image/jpg, image/png,\
                                  image/mng, image/bmp, text/plain, text/html, text/htm\r\n";

    pFile->AddRequestHeaders(szHeaders);
    pFile->SendRequest();

    DWORD dwRet;
    pFile->QueryInfoStatusCode(dwRet);

    if (dwRet != HTTP_STATUS_OK)
    {
        CString errText;
        errText.Format(L"POST出错，错误码：%d", dwRet);
        AfxMessageBox(errText);
    }
    else
    {
        int len = static_cast<int> (pFile->GetLength()); 
        char buf[2000];
        int numread;
        //CString filepath;
        //CString strFile = L"response.txt";
        //filepath.Format(L".\\%s", strFile);
        //CFile myfile(filepath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
        while ((numread = pFile->Read(buf, sizeof(buf) - 1)) > 0)
        {
            buf[numread] = '\0'; 
            result += buf;
            //strFile += buf;//?
            //myfile.Write(buf, numread);
        }
        //myfile.Close();

        //TRACE("size：%d, body：%s\n", result.length(), result.c_str() );
    }

    session.Close();//you can skip, this gets called when we exist the function
    pFile->Close();
    delete pFile;
    delete pConnection;//don't skip

    return result;
}

//std::wstring UTF8_to_wchar(const char* in)
//{
//    std::wstring out;
//    unsigned int codepoint;
//    while (*in != 0)
//    {
//        unsigned char ch = static_cast<unsigned char>(*in);
//        if (ch <= 0x7f)
//            codepoint = ch;
//        else if (ch <= 0xbf)
//            codepoint = (codepoint << 6) | (ch & 0x3f);
//        else if (ch <= 0xdf)
//            codepoint = ch & 0x1f;
//        else if (ch <= 0xef)
//            codepoint = ch & 0x0f;
//        else
//            codepoint = ch & 0x07;
//        ++in;
//        if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
//        {
//            if (sizeof(wchar_t) > 2)
//                out.append(1, static_cast<wchar_t>(codepoint));
//            else if (codepoint > 0xffff)
//            {
//                out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
//                out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
//            }
//            else if (codepoint < 0xd800 || codepoint >= 0xe000)
//                out.append(1, static_cast<wchar_t>(codepoint));
//        }
//    }
//    return out;
//}
//
//std::string wchar_to_UTF8(const wchar_t* in)
//{
//    std::string out;
//    unsigned int codepoint = 0;
//    for (in; *in != 0; ++in)
//    {
//        if (*in >= 0xd800 && *in <= 0xdbff)
//            codepoint = ((*in - 0xd800) << 10) + 0x10000;
//        else
//        {
//            if (*in >= 0xdc00 && *in <= 0xdfff)
//                codepoint |= *in - 0xdc00;
//            else
//                codepoint = *in;
//
//            if (codepoint <= 0x7f)
//                out.append(1, static_cast<char>(codepoint));
//            else if (codepoint <= 0x7ff)
//            {
//                out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
//                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
//            }
//            else if (codepoint <= 0xffff)
//            {
//                out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
//                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
//                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
//            }
//            else
//            {
//                out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
//                out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
//                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
//                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
//            }
//            codepoint = 0;
//        }
//    }
//    return out;
//}