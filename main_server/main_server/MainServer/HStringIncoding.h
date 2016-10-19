#pragma once

#include "stdafx.h"

class HStringIncoding
{
public:
    static void ANSIToUniCode(TCHAR* strDest, char* strSource)
    {
        MultiByteToWideChar(CP_ACP, NULL, strSource, -1, strDest, (strlen(strSource) * 2));
    }

    static void UniCodeToANSI(char* strDest, TCHAR* strSource)
    {
        WideCharToMultiByte(CP_ACP, NULL, strSource, -1, strDest, 10, NULL, FALSE);
    }

    static char* ANSIToUTF8(const char* pszCode)
    {
        int     nLength, nLength2;
        BSTR    bstrCode;
        char*   pszUTFCode = NULL;

        nLength = MultiByteToWideChar(CP_ACP, 0, pszCode, strlen(pszCode), NULL, NULL);
        bstrCode = SysAllocStringLen(NULL, nLength);
        MultiByteToWideChar(CP_ACP, 0, pszCode, strlen(pszCode), bstrCode, nLength);

        nLength2 = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL, NULL);
        pszUTFCode = new char[nLength2 + 1];
        WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, nLength2, NULL, NULL);

        return pszUTFCode;
    }

    static char* UTF8ToANSI(const char* pszCode)
    {
        BSTR bstrWide;
        char* pszAnsi;
        int nLength;

        nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
        bstrWide = SysAllocStringLen(NULL, nLength);

        MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);

        nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
        pszAnsi = new char[nLength];

        WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
        SysFreeString(bstrWide);

        return pszAnsi;
    }

    static char* UnicodeToUTF8(TCHAR* strSource)
    {
        int nLen = WideCharToMultiByte(CP_UTF8, 0, strSource, lstrlen(strSource), NULL, 0, NULL, NULL);

        char* szUTF8 = new char[nLen];
        ZeroMemory(szUTF8, nLen);

        WideCharToMultiByte(CP_UTF8, 0, strSource, lstrlen(strSource), szUTF8, nLen, NULL, NULL);


        return szUTF8;
    }

    static TCHAR* UTF8ToUnicode(char* strSource)
    {
        int nLen = MultiByteToWideChar(CP_UTF8, 0, strSource, strlen(strSource), NULL, NULL);

        TCHAR* strUnicode = new TCHAR[nLen];

        MultiByteToWideChar(CP_UTF8, 0, strSource, strlen(strSource), strUnicode, nLen);

        for (int i = 0; i < nLen; ++i)
        {
            if (strUnicode[i] == UNICODE_GARBAGE)
            {
                strUnicode[i] = NULL;
                break;
            }
        }

        return strUnicode;
    }

    static char* ANSIToUniCodeToUTF8(char* szSource)
    {
        TCHAR tBuffer[SOCKET_MSG_ONE_ARRAY_SIZE];

        ANSIToUniCode(tBuffer, szSource);

        return UnicodeToUTF8(tBuffer);
    }

    static char* UTF8ToUniCodeToANSI(char* szSource)
    {
        TCHAR* tBuffer;

        tBuffer = UTF8ToUnicode(szSource);

        char* szReturn = new char[SOCKET_MSG_ONE_ARRAY_SIZE];

        UniCodeToANSI(szReturn, tBuffer);

        Safe_Delete(tBuffer);

        return szReturn;
    }
};

