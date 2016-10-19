#pragma once

#include "stdafx.h"

class HInterpretHelper
{
public:
	static char* MakeCharArray(int nSize)
	{
		char* szArray = new char[nSize];

		ZeroMemory(szArray, nSize);

		return szArray;
	}

	static void MakeSocketMessage(char* szDest, const char* szSource, const int& n)
	{
		if (!n)
		{
			memcpy(szDest, szSource, strlen(szSource));
			szDest[strlen(szSource)] = '#';
			return;
		}

		else
		{
			SOCKET_CREATE_MSG_LOOP(n)
			{
				int nMatchIndex = SOCKET_MATCH_INDEX(n);
				int nIndex = i - SOCKET_MATCH_INDEX(n);
				int nLen = strlen(szSource);

				if (szSource[i - SOCKET_MATCH_INDEX(n)] == NULL)
					break;

				szDest[i] = szSource[i - SOCKET_MATCH_INDEX(n)]; 
			}

			szDest[strlen(szSource) + SOCKET_MATCH_INDEX(n)] = '#';
		}
	}

    static void MakeSocketMessageEnd(char* szDest)
    {
        szDest[SOCKET_MSG_BUF - 1] = '|';
    }

	static void MakeSocketMessageWhile(char* szDest, const char* szSource, const int& n)
	{
		int nIndex = 0;
		SOCKET_CREATE_MSG_WHILE(n)
		{
			if (szSource[i - SOCKET_MATCH_INDEX(n)] == '#')
				break;

			++nIndex;
			szDest[i] = szSource[i - SOCKET_MATCH_INDEX(n)];
		}

		szDest[nIndex + SOCKET_MATCH_INDEX(n)] = '#';
	}

	static char* ReadEndLine(const char* szSource, const int& n)
	{
		char* szDest = MakeCharArray(SOCKET_MSG_BUF);

		SOCKET_CREATE_MSG_WHILE(n)
		{
			szDest[i - SOCKET_MATCH_INDEX(n)] = szSource[i];
			szDest[(i + 1) - SOCKET_MATCH_INDEX(n)] = '#';
			if (szSource[i] == '#') break;
		}

		return szDest;
	}

	static void MakeSocketMessage(char* szDest, const int& nSource, const int& n)
	{
		char* szSource = MakeCharArray(SOCKET_MSG_ONE_ARRAY_SIZE);
		itoa(nSource, szSource, 10);
        
		MakeSocketMessage(szDest, szSource, n);
		Safe_Delete(szSource);
	}

	static void ReadSocketMessage(char* szDest, const char* szSource, const int& n)
	{
		if (!n)
		{
			for (int i = 0; i <= SOCKET_MSG_ONE; ++i)
			{
				if (szSource[i] == '#')
					break;

				szDest[i] = szSource[i];
			}

			return;
		}

		SOCKET_CREATE_MSG_LOOP(n)
		{
			if (szSource[i] == '#')
				break;

			szDest[i - SOCKET_MATCH_INDEX(n)] = szSource[i];
		}
	}

	static char* MakeSuccessMessage(const char* szMessage)
	{
		char* szMakeMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);
		MakeSocketMessage(szMakeMessage, szMessage, 0);
		MakeSocketMessage(szMakeMessage, 1, 1);
        MakeSocketMessageEnd(szMakeMessage);
		return szMakeMessage;
	}

	static char* MakeFailedMessage(const char* szMessage)
	{
		char* szMakeMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);
		MakeSocketMessage(szMakeMessage, szMessage, 0);
		MakeSocketMessage(szMakeMessage, 0, 1);
        MakeSocketMessageEnd(szMakeMessage);
		return szMakeMessage;
	}

	static char* ClearMessage()
	{
		char* szMakeMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);
		
		strcpy(szMakeMessage, "AM_CLEAR");

		szMakeMessage[strlen(szMakeMessage)] = '#';

        MakeSocketMessageEnd(szMakeMessage);

		return szMakeMessage;
	}
};

