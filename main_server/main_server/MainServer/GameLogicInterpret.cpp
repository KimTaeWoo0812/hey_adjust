#include "GameLogicInterpret.h"


CGameLogicInterpret::CGameLogicInterpret()
{
	
	for (int i = 0; i < Game::GAME_MSG_COUNT; ++i)
		ZeroMemory(m_szMessage[i], SOCKET_MSG_ONE_ARRAY_SIZE);

	strcpy(m_szMessage[Game::GAME_MSG_CHAT], "AM_ROOM_CHAT");//
	strcpy(m_szMessage[Game::GAME_MSG_TIME_RIGHT], "AM_GAME_TIME_RIGHT");//
	strcpy(m_szMessage[Game::GAME_MSG_END], "AM_ROOM_END");//
	strcpy(m_szMessage[Game::GAME_MSG_OUT], "AM_ROOM_OUT");//
	strcpy(m_szMessage[Game::GAME_MSG_USER_INFO], "AM_ROOM_USER_INFO");//
	strcpy(m_szMessage[Game::GAME_MSG_PREV], "AM_ROOM_PREV");//
	strcpy(m_szMessage[Game::GAME_MSG_PICK_TOPIC], "AM_GAME_PICK_TOPIC");
	strcpy(m_szMessage[Game::GAME_MSG_TIME_START], "AM_GAME_TIME_START");//
	strcpy(m_szMessage[Game::GAME_MSG_TIME_OUT], "AM_GAME_TIME_OUT");//
	strcpy(m_szMessage[Game::GAME_MSG_ROOM_INFO], "AM_ROOM_ROOM_INFO");//
    strcpy(m_szMessage[Game::GAME_MSG_SUCCESS], "AM_GAME_SUCCESS");
    strcpy(m_szMessage[Game::GAME_MSG_RIGHT], "AM_GAME_RIGHT");
    strcpy(m_szMessage[Game::GAME_MSG_CATEGORY], "AM_GAME_CATEGORY");
}

CGameLogicInterpret::~CGameLogicInterpret()
{
}

const Game::Game_Message CGameLogicInterpret::InterpretMessage(const char* szMessage)
{
	char szOrderMessage[SOCKET_MSG_ONE_ARRAY_SIZE];

	ZeroMemory(szOrderMessage, SOCKET_MSG_ONE_ARRAY_SIZE);

	HInterpretHelper::ReadSocketMessage(szOrderMessage, szMessage, 0);

	for (int i = 0; i < Game::GAME_MSG_COUNT; ++i)
	{
		if (strcmp(szOrderMessage, m_szMessage[i]) == 0)
			return Game::Game_Message(i);
	}

	return Game::GAME_MSG_COUNT;
}

char* CGameLogicInterpret::ChatMessage(const CClientBase* pMyClient, const char* szMessage) const
{
	//1. Message
	//2. NickName
	//3. Chat Message
	char* szChat = HInterpretHelper::ReadEndLine(szMessage, 1);

	char* szChatMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szChatMessage, m_szMessage[Game::GAME_MSG_CHAT], 0);
	HInterpretHelper::MakeSocketMessage(szChatMessage, pMyClient->GetClientInfo().szNickName, 1);
	HInterpretHelper::MakeSocketMessageWhile(szChatMessage, szChat, 2);

	Safe_Delete(szChat);

	return szChatMessage;
}

void CGameLogicInterpret::SendOutOfGame(const CClientBase* pMyClient) const
{
	char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_OUT], 0);

    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CGameLogicInterpret::SendGameUserInfo(const CClientBase* pMyClient, const CClientBase* pYourClient, const int& nPositionIndex) const
{
	char szSendMessage[SOCKET_MSG_BUF];
	ZeroMemory(szSendMessage, SOCKET_MSG_BUF);

	Client::CLIENT_INFO tInfo = pYourClient->GetClientInfo();

	printf("%d\n", pMyClient->getIndex());

	HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_USER_INFO], 0);
	HInterpretHelper::MakeSocketMessage(szSendMessage, nPositionIndex + 1, 1);
	HInterpretHelper::MakeSocketMessage(szSendMessage, tInfo.szNickName, 2);
	HInterpretHelper::MakeSocketMessage(szSendMessage, tInfo.nPoint, 3);
	HInterpretHelper::MakeSocketMessage(szSendMessage, tInfo.nRank, 4);
	HInterpretHelper::MakeSocketMessage(szSendMessage, 0, 5);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);
}

void CGameLogicInterpret::SendGameUserInfoEnd(const CClientBase* pMyClient) const
{
	char* szSendMessage = HInterpretHelper::MakeFailedMessage(m_szMessage[Game::GAME_MSG_USER_INFO]);

    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CGameLogicInterpret::SendUserInfoRight(const CClientBase* pMyClient, const int& nPositionIndex, const int& nRightNumber) const
{
    char* szMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

    HInterpretHelper::MakeSocketMessage(szMessage, m_szMessage[Game::GAME_MSG_RIGHT], 0);
    HInterpretHelper::MakeSocketMessage(szMessage, nPositionIndex + 1, 1);
    HInterpretHelper::MakeSocketMessage(szMessage, nRightNumber, 2);
    HInterpretHelper::MakeSocketMessage(szMessage, pMyClient->GetClientInfo().szNickName, 3);
    HInterpretHelper::MakeSocketMessageEnd(szMessage);

    pMyClient->SendNote(szMessage);

    Safe_Delete(szMessage);
}

void CGameLogicInterpret::SendUserInfoRightEnd(const CClientBase* pMyClient) const
{
    char* szMessage = HInterpretHelper::MakeFailedMessage(m_szMessage[Game::GAME_MSG_RIGHT]);

    HInterpretHelper::MakeSocketMessageEnd(szMessage);

    pMyClient->SendNote(szMessage);

    Safe_Delete(szMessage);
}

void CGameLogicInterpret::SendGameRoomInfo(const CClientBase* pMyClient, const int& nRoomIndex,
    const char* szRoomName, const int& nMaxClientInRoom, const int& nCurrRoomNumber) const
{
	char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	const Client::CLIENT_INFO tInfo = pMyClient->GetClientInfo();

	printf("%d\n", pMyClient->getIndex());

    HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_ROOM_INFO], 0);
	HInterpretHelper::MakeSocketMessage(szSendMessage, nRoomIndex, 1);
	HInterpretHelper::MakeSocketMessage(szSendMessage, szRoomName, 2);
	HInterpretHelper::MakeSocketMessage(szSendMessage, nMaxClientInRoom, 3);
	HInterpretHelper::MakeSocketMessage(szSendMessage, nCurrRoomNumber, 4);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CGameLogicInterpret::SendTimeStart(const CClientBase* pMyClient) const
{
	char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_TIME_START], 0);

    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CGameLogicInterpret::SendTimeOut(const CClientBase* pMyClient) const
{
	char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_TIME_OUT], 0);

    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CGameLogicInterpret::SendSuccessMessage(const CClientBase* pMyClient, const CClientBase* pRightClient) const
{
	char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);
    Client::CLIENT_INFO tRightInfo = pRightClient->GetClientInfo();

	HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_SUCCESS], 0);
    HInterpretHelper::MakeSocketMessage(szSendMessage, tRightInfo.szNickName, 1);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CGameLogicInterpret::EndGame(const CClientBase* pMyClient) const
{
	char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_END], 0);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CGameLogicInterpret::SendPickTopic(const CClientBase* pMyClient, const char* szProblem) const
{
    char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

    HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_PICK_TOPIC], 0);
    HInterpretHelper::MakeSocketMessage(szSendMessage, szProblem, 1);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

    pMyClient->SendNote(szSendMessage);

    Safe_Delete(szSendMessage);
}

void CGameLogicInterpret::SendCategory(const CClientBase* pMyClient, const int& nCateogry) const
{
    char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

    HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Game::GAME_MSG_CATEGORY], 0);
    HInterpretHelper::MakeSocketMessage(szSendMessage, nCateogry, 1);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

    pMyClient->SendNote(szSendMessage);

    Safe_Delete(szSendMessage);
}