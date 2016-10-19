#include "LoginInterpret.h"

CLoginInterpret::CLoginInterpret()
{
	ZeroMemory(m_szMessage, sizeof(m_szMessage));
	ZeroMemory(m_szID, sizeof(m_szID));
	strcpy(m_szMessage[Client_Position::LOGIN_MSG_LOGIN], "AM_LOGIN");
	strcpy(m_szMessage[Client_Position::LOGIN_MSG_CREATE], "AM_LOGIN_CREATE");
}

bool CLoginInterpret::isLogin(const char* pMessage)
{
	char szMessage[SOCKET_MSG_ONE_ARRAY_SIZE];
	char szID[SOCKET_MSG_ONE_ARRAY_SIZE];
	char szPassword[SOCKET_MSG_ONE_ARRAY_SIZE];

	ZeroMemory(szMessage, sizeof(szMessage));
	ZeroMemory(szID, sizeof(szID));
	ZeroMemory(szPassword, sizeof(szPassword));

	HInterpretHelper::ReadSocketMessage(szMessage, pMessage, 0);

	if (strcmp(szMessage, m_szMessage[Client_Position::LOGIN_MSG_LOGIN]) == 0)
	{
		HInterpretHelper::ReadSocketMessage(szID, pMessage, 1);

		char* pRightPassword = GetSingle<CDataBase>()->getPassword(szID);

		if (pRightPassword == nullptr) return false;

		HInterpretHelper::ReadSocketMessage(szPassword, pMessage, 2);

		if (strcmp(pRightPassword, szPassword) == 0)
		{
			ZeroMemory(m_szID, sizeof(m_szID));
			memcpy(m_szID, szID, sizeof(szID));
			Safe_Delete(pRightPassword);
			return true;
		}

		Safe_Delete(pRightPassword);
	}

	return false;
}

char* CLoginInterpret::CreateFirstMessage()
{
	return HInterpretHelper::MakeSuccessMessage(m_szMessage[Client_Position::LOGIN_MSG_LOGIN]);
}

char* CLoginInterpret::CreateTwiceMessage()
{
	m_tClientInfo.Initialize();

	char* szTwiceMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);
	HInterpretHelper::MakeSocketMessage(szTwiceMessage, m_szMessage[Client_Position::LOGIN_MSG_LOGIN], 0);

	char* szNickName = GetSingle<CDataBase>()->getNickName(m_szID);
	HInterpretHelper::MakeSocketMessage(szTwiceMessage, szNickName, 1);
	memcpy(m_tClientInfo.szNickName, szNickName, strlen(szNickName));
	Safe_Delete(szNickName);

	int nPoint = GetSingle<CDataBase>()->getPoint(m_szID);
	int nRank = GetSingle<CDataBase>()->getRank(m_szID);

	m_tClientInfo.nPoint = nPoint;
	m_tClientInfo.nRank = nRank;

	HInterpretHelper::MakeSocketMessage(szTwiceMessage, nPoint, 2);
	HInterpretHelper::MakeSocketMessage(szTwiceMessage, nRank, 3);

	szTwiceMessage[SOCKET_MSG_BUF - 1] = '\n';

	return szTwiceMessage;
}

void CLoginInterpret::SuccessMessage(CClientBase* pMyClient)
{
	char* pFirstMessage = CreateFirstMessage();
	pMyClient->SendNote(pFirstMessage);
	Safe_Delete(pFirstMessage);

	char* pTwiceMessage = CreateTwiceMessage();
    HInterpretHelper::MakeSocketMessageEnd(pTwiceMessage);
	pMyClient->SendNote(pTwiceMessage);
	Safe_Delete(pTwiceMessage);

	pMyClient->SetClientInfo(m_tClientInfo);
}

void CLoginInterpret::FailedMessage(CClientBase* pMyClient)
{
	char* pFirstMessage = HInterpretHelper::MakeFailedMessage(m_szMessage[Client_Position::LOGIN_MSG_LOGIN]);

    HInterpretHelper::MakeSocketMessageEnd(pFirstMessage);

	pMyClient->SendNote(pFirstMessage);

	Safe_Delete(pFirstMessage);
}

bool CLoginInterpret::isCreate(CClientBase* pMyClient, const char* szMessage)
{
	char szOrderMessage[SOCKET_MSG_ONE_ARRAY_SIZE];

	ZeroMemory(szOrderMessage, SOCKET_MSG_ONE_ARRAY_SIZE);

	HInterpretHelper::ReadSocketMessage(szOrderMessage, szMessage, 0);

	if (strcmp(szOrderMessage, m_szMessage[Client_Position::LOGIN_MSG_CREATE]) == 0)
	{
		char* szID = HInterpretHelper::MakeCharArray(SOCKET_MSG_ONE_ARRAY_SIZE);
		char* szPassword = HInterpretHelper::MakeCharArray(SOCKET_MSG_ONE_ARRAY_SIZE);
		char* szNickName = HInterpretHelper::MakeCharArray(SOCKET_MSG_ONE_ARRAY_SIZE);

		HInterpretHelper::ReadSocketMessage(szID, szMessage, 1);
		HInterpretHelper::ReadSocketMessage(szPassword, szMessage, 2);
		HInterpretHelper::ReadSocketMessage(szNickName, szMessage, 3);

		if (GetSingle<CDataBase>()->setUserSignUp(szID, szPassword, szNickName))
			SuccessCreate(pMyClient);

		else FailedMessage(pMyClient);

		Safe_Delete(szID);
		Safe_Delete(szPassword);
		Safe_Delete(szNickName);

		return true;
	}

	return false;
}

void CLoginInterpret::SuccessCreate(CClientBase* pMyClient)
{
	char* szSendMessage = HInterpretHelper::MakeSuccessMessage(m_szMessage[Client_Position::LOGIN_MSG_CREATE]);

    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CLoginInterpret::FailedCreate(CClientBase* pMyClient)
{
	char* szSendMessage = HInterpretHelper::MakeFailedMessage(m_szMessage[Client_Position::LOGIN_MSG_CREATE]);

    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}