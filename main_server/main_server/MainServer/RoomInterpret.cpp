#include "RoomInterpret.h"


CRoomInterpret::CRoomInterpret()
{
	for (int i = 0; i < Client_Position::ROOM_MSG_COUNT; ++i)
		ZeroMemory(m_szMessage[i], SOCKET_MSG_ONE_ARRAY_SIZE);

	strcpy(m_szMessage[Client_Position::ROOM_MSG_PREV], "AM_ROOM_PREV");
	strcpy(m_szMessage[Client_Position::ROOM_MSG_OUT], "AM_ROOM_OUT");
	strcpy(m_szMessage[Client_Position::ROOM_MSG_CHAT], "AM_ROOM_CHAT");
	strcpy(m_szMessage[Client_Position::ROOM_MSG_REFRESH], "AM_ROOM_REFRESH");
	strcpy(m_szMessage[Client_Position::ROOM_MSG_START ], "AM_ROOM_START");
	strcpy(m_szMessage[Client_Position::ROOM_MSG_USER_INFO], "AM_ROOM_USER_INFO");
	strcpy(m_szMessage[Client_Position::ROOM_MSG_READY], "AM_ROOM_READY");
	strcpy(m_szMessage[Client_Position::ROOM_MSG_ROOM_INFO], "AM_ROOM_ROOM_INFO");
    strcpy(m_szMessage[Client_Position::ROOM_MSG_CATEGORY], "AM_ROOM_CATEGORY");
}

CRoomInterpret::~CRoomInterpret()
{

}

const Client_Position::Room_Message CRoomInterpret::InterpretMessage(const char* szMessage) const
{
	char szOrderMessage[SOCKET_MSG_ONE_ARRAY_SIZE];

	ZeroMemory(szOrderMessage, SOCKET_MSG_ONE_ARRAY_SIZE);

	HInterpretHelper::ReadSocketMessage(szOrderMessage, szMessage, 0);

	for (int i = 0; i < Client_Position::ROOM_MSG_COUNT; ++i)
	{
		if (strcmp(szOrderMessage, m_szMessage[i]) == 0)
			return Client_Position::Room_Message(i);
	}

	return Client_Position::ROOM_MSG_COUNT;
}

void CRoomInterpret::SendRoomUserInfo(const Client_Position::ROOM_CLIENT& tMyClient, const Client_Position::ROOM_CLIENT& tYourClient, const int& nPositionIndex) const
{
	char szRoomMessage[SOCKET_MSG_BUF];
	ZeroMemory(szRoomMessage, SOCKET_MSG_BUF);

	Client::CLIENT_INFO tInfo = tYourClient.pClient->GetClientInfo();

	HInterpretHelper::MakeSocketMessage(szRoomMessage, m_szMessage[Client_Position::ROOM_MSG_USER_INFO], 0);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, nPositionIndex + 1, 1);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, tInfo.szNickName, 2);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, tInfo.nPoint, 3);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, tInfo.nRank, 4);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, (int)tYourClient.bReady, 5);
    HInterpretHelper::MakeSocketMessageEnd(szRoomMessage);

	tMyClient.pClient->SendNote(szRoomMessage);
}

void CRoomInterpret::SendRoomUserInfoEnd(const CClientBase* pMyClient) const
{
	EndPointMessage(pMyClient, Client_Position::ROOM_MSG_USER_INFO);
}

void CRoomInterpret::SendRoomInfo(const CClientBase* pMyClient, const Client_Position::ROOM_DATA& tRoomData) const
{
	char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	Client::CLIENT_INFO tInfo = pMyClient->GetClientInfo();

	printf("%d\n", pMyClient->getIndex());

	HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Client_Position::ROOM_MSG_ROOM_INFO], 0);
	HInterpretHelper::MakeSocketMessage(szSendMessage, tRoomData.nRoomNumber, 1);
	HInterpretHelper::MakeSocketMessage(szSendMessage, tRoomData.szRoomName, 2);
	HInterpretHelper::MakeSocketMessage(szSendMessage, tRoomData.nMaxClientInRoom, 3);
	HInterpretHelper::MakeSocketMessage(szSendMessage, tRoomData.nCurrentClientNumber, 4);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}


void CRoomInterpret::EndPointMessage(const CClientBase* pMyClient, const Client_Position::Room_Message& eMessage) const
{
	char szRoomMessage[SOCKET_MSG_BUF];
	ZeroMemory(szRoomMessage, SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szRoomMessage, m_szMessage[eMessage], 0);
	szRoomMessage[SOCKET_MSG_ONE_ARRAY_SIZE] = '0';
	szRoomMessage[SOCKET_MSG_ONE_ARRAY_SIZE + 1] = '#';
    HInterpretHelper::MakeSocketMessageEnd(szRoomMessage);

	pMyClient->SendNote(szRoomMessage);
}

char* CRoomInterpret::ChatMessage(const CClientBase* pMyClient, const char* szMessage) const
{
	//1. Message
	//2. NickName
	//3. Chat Message
	char* szChat = HInterpretHelper::ReadEndLine(szMessage, 1);

	char* szChatMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szChatMessage, m_szMessage[Client_Position::ROOM_MSG_CHAT], 0);
	HInterpretHelper::MakeSocketMessage(szChatMessage, pMyClient->GetClientInfo().szNickName, 1);
	HInterpretHelper::MakeSocketMessageWhile(szChatMessage, szChat, 2);

	Safe_Delete(szChat);

	return szChatMessage;
}

void CRoomInterpret::SendOutRoom(CClientBase* pMyClient, CClientPositionBase* pLobby)
{
	char* szMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szMessage, m_szMessage[Client_Position::ROOM_MSG_OUT], 0);
    HInterpretHelper::MakeSocketMessageEnd(szMessage);

	pMyClient->SendNote(szMessage);
	MoveToLobby(pMyClient, pLobby);

	Safe_Delete(szMessage);
}

void CRoomInterpret::SendPrevRoom(CClientBase* pMyClient, CClientPositionBase* pLobby) 
{
	char* szMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szMessage, m_szMessage[Client_Position::ROOM_MSG_PREV], 0);
    HInterpretHelper::MakeSocketMessageEnd(szMessage);

	pMyClient->SendNote(szMessage);
	MoveToLobby(pMyClient, pLobby);

	Safe_Delete(szMessage);
}

void CRoomInterpret::MoveToLobby(CClientBase* pMyClient, CClientPositionBase* pLobby)
{
	pLobby->AddClient(pMyClient);
}

void CRoomInterpret::SuccessStartGame(const CClientBase* pMyClient) const
{
	char* szSendMessage = HInterpretHelper::MakeSuccessMessage(m_szMessage[Client_Position::ROOM_MSG_START]);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CRoomInterpret::FailedStartGame(const CClientBase* pMyClient) const
{
	char* szSendMessage = HInterpretHelper::MakeFailedMessage(m_szMessage[Client_Position::ROOM_MSG_START]);
    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}

void CRoomInterpret::SendPickTopic(const CClientBase* pMyClient, char* szMessage) const
{
    HInterpretHelper::MakeSocketMessageEnd(szMessage);

	pMyClient->SendNote(szMessage);
}

void CRoomInterpret::SendPickTopic(const CClientBase* pMyClient, const int& nPickTopic) const
{
    char* szSendMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

    HInterpretHelper::MakeSocketMessage(szSendMessage, m_szMessage[Client_Position::ROOM_MSG_CATEGORY], 0);
    HInterpretHelper::MakeSocketMessage(szSendMessage, nPickTopic, 1);

    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

    pMyClient->SendNote(szSendMessage);

    Safe_Delete(szSendMessage);
}

const int CRoomInterpret::GetPickTopicNumber(const char* szMessage) const
{
	char* szTopicNumber = HInterpretHelper::MakeCharArray(SOCKET_MSG_ONE_ARRAY_SIZE);

	HInterpretHelper::ReadSocketMessage(szTopicNumber, szMessage, 1);

	szTopicNumber[strlen(szTopicNumber)] = NULL;

	int nNumber = atoi(szTopicNumber);

	Safe_Delete(szTopicNumber);

	return nNumber;
}

void CRoomInterpret::ClearRoomMessage(const CClientBase* pMyClient) const
{
    char* szClearMessage = HInterpretHelper::ClearMessage();

    printf("AM_ROOM_CLEAR\n");

    HInterpretHelper::MakeSocketMessageEnd(szClearMessage);

    pMyClient->SendNote(szClearMessage);

    Safe_Delete(szClearMessage);
}