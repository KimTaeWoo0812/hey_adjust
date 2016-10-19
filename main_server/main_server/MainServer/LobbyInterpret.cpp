#include "LobbyInterpret.h"


CLobbyInterpret::CLobbyInterpret()
{
	for (int i = 0; i < Client_Position::LOBBY_MSG_COUNT; ++i)
		ZeroMemory(m_szMessage[i], SOCKET_MSG_ONE_ARRAY_SIZE);

	strcpy(m_szMessage[Client_Position::LOBBY_MSG_ROOM_INFO], "AM_ROOM_INFO");
	strcpy(m_szMessage[Client_Position::LOBBY_MSG_USER_INFO], "AM_USER_INFO");
	strcpy(m_szMessage[Client_Position::LOBBY_MSG_FRIEND_INFO], "AM_FRIEND_INFO");
	strcpy(m_szMessage[Client_Position::LOBBY_MSG_REFRESH], "AM_REFRESH");
	strcpy(m_szMessage[Client_Position::LOBBY_MSG_CHAT], "AM_LOBBY_CHAT");
	strcpy(m_szMessage[Client_Position::LOBBY_MSG_CREATE_ROOM], "AM_CREATE_ROOM");
	strcpy(m_szMessage[Client_Position::LOBBY_MSG_ENTER_ROOM], "AM_ENTER_ROOM");
}


CLobbyInterpret::~CLobbyInterpret()
{
}

void CLobbyInterpret::EndPointMessage(const CClientBase* pMyClient, const Client_Position::Lobby_Message& eMessage) const
{
	char szRoomMessage[SOCKET_MSG_BUF];
	ZeroMemory(szRoomMessage, SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szRoomMessage, m_szMessage[eMessage], 0);
	szRoomMessage[SOCKET_MSG_ONE_ARRAY_SIZE] = '0';
	szRoomMessage[SOCKET_MSG_ONE_ARRAY_SIZE + 1] = '#';
	szRoomMessage[SOCKET_MSG_BUF - 1] = '\n';

    HInterpretHelper::MakeSocketMessageEnd(szRoomMessage);

	pMyClient->SendNote(szRoomMessage);
}

void CLobbyInterpret::SendRoomInfo(const CClientBase* pMyClient, const Client_Position::ROOM_DATA& tRoomData) const
{
	//1. Message
	//2. RoomNumber
	//3. RoomName
	char szRoomMessage[SOCKET_MSG_BUF];
	ZeroMemory(szRoomMessage, SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szRoomMessage, m_szMessage[Client_Position::LOBBY_MSG_ROOM_INFO], 0);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, tRoomData.nRoomNumber, 1);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, tRoomData.szRoomName, 2);
    HInterpretHelper::MakeSocketMessage(szRoomMessage, tRoomData.nCurrentClientNumber, 3);
    HInterpretHelper::MakeSocketMessage(szRoomMessage, tRoomData.nMaxClientInRoom, 4);
    HInterpretHelper::MakeSocketMessage(szRoomMessage, tRoomData.bStart, 5);
    HInterpretHelper::MakeSocketMessageEnd(szRoomMessage);

	pMyClient->SendNote(szRoomMessage);
}

void CLobbyInterpret::SendRoomInfoEnd(const CClientBase* pMyClient) const
{
	EndPointMessage(pMyClient, Client_Position::LOBBY_MSG_ROOM_INFO);
}

void CLobbyInterpret::SendLobyUserInfo(const CClientBase* pMyClient, const CClientBase* pYourClient) const
{
	//1. Message
	//2. User.NickName
	//3. User.Point
	//4. User.Rank

	char szRoomMessage[SOCKET_MSG_BUF];
	ZeroMemory(szRoomMessage, SOCKET_MSG_BUF);

	Client::CLIENT_INFO tInfo = pYourClient->GetClientInfo();

	HInterpretHelper::MakeSocketMessage(szRoomMessage, m_szMessage[Client_Position::LOBBY_MSG_USER_INFO], 0);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, tInfo.szNickName, 1);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, tInfo.nPoint, 2);
	HInterpretHelper::MakeSocketMessage(szRoomMessage, tInfo.nRank, 3);
    HInterpretHelper::MakeSocketMessageEnd(szRoomMessage);

	pMyClient->SendNote(szRoomMessage);
}

void CLobbyInterpret::SendLobyUserInfoEnd(const CClientBase* pMyClient) const
{
	EndPointMessage(pMyClient, Client_Position::LOBBY_MSG_USER_INFO);
}

const Client_Position::Lobby_Message CLobbyInterpret::InterpretMessage(const char* szMessage) const
{
	char szOrderMessage[SOCKET_MSG_ONE_ARRAY_SIZE];

	ZeroMemory(szOrderMessage, SOCKET_MSG_ONE_ARRAY_SIZE);

	HInterpretHelper::ReadSocketMessage(szOrderMessage, szMessage, 0);

	for (int i = 0; i < Client_Position::LOBBY_MSG_COUNT; ++i)
	{
		if (strcmp(szOrderMessage, m_szMessage[i]) == 0)
			return Client_Position::Lobby_Message(i);
	}

	return Client_Position::LOBBY_MSG_COUNT;
}

char* CLobbyInterpret::ChatMessage(const CClientBase* hMyClient, const char* szMessage) const
{
	//1. Message
	//2. NickName
	//3. Chat Message
	char* szChat = HInterpretHelper::ReadEndLine(szMessage, 1);

	char* szChatMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szChatMessage, m_szMessage[Client_Position::LOBBY_MSG_CHAT], 0);
	HInterpretHelper::MakeSocketMessage(szChatMessage, hMyClient->GetClientInfo().szNickName, 1);
	HInterpretHelper::MakeSocketMessageWhile(szChatMessage, szChat, 2);

	Safe_Delete(szChat);

	return szChatMessage;
}

void CLobbyInterpret::CreateRoom(CClientRoom* pRoomInfo, const char* szMessage, const int& nRoomIndex) const
{
	char* szRoomName = HInterpretHelper::MakeCharArray(SOCKET_MSG_ONE_ARRAY_SIZE);

	HInterpretHelper::ReadSocketMessage(szRoomName, szMessage, 1);

    int nIndex = 0; 

    SOCKET_CREATE_MSG_LOOP(2)
    {
        if (szMessage[i] == '#')
            nIndex = i;
    }

    int nMaxClientInRoom = szMessage[nIndex - 1] - '0';

	pRoomInfo->CreateRoom(nRoomIndex, nMaxClientInRoom, szRoomName);

	Safe_Delete(szRoomName);
}

void CLobbyInterpret::SuccessCreateRoom(const CClientBase* pMyClient, const int& nRoomIndex) const
{
	char* szMessage = HInterpretHelper::MakeCharArray(SOCKET_MSG_BUF);

	HInterpretHelper::MakeSocketMessage(szMessage, m_szMessage[Client_Position::LOBBY_MSG_CREATE_ROOM], 0);
	HInterpretHelper::MakeSocketMessage(szMessage, nRoomIndex, 1);
    HInterpretHelper::MakeSocketMessageEnd(szMessage);

	pMyClient->SendNote(szMessage);

	Safe_Delete(szMessage);
}	

void CLobbyInterpret::FailedCreateRoom(const CClientBase* pMyClient) const
{
	char* szMessage = HInterpretHelper::MakeFailedMessage(m_szMessage[Client_Position::LOBBY_MSG_CREATE_ROOM]);

    HInterpretHelper::MakeSocketMessageEnd(szMessage);

	pMyClient->SendNote(szMessage);

	Safe_Delete(szMessage);
}

const int CLobbyInterpret::InterpretRoomNumber(const char* message) const
{
	char* szNumber = HInterpretHelper::MakeCharArray(SOCKET_MSG_ONE_ARRAY_SIZE);

	HInterpretHelper::ReadSocketMessage(szNumber, message, 1);

	int nNumber = szNumber[0] - '0';

	Safe_Delete(szNumber);

	return nNumber;
}


void CLobbyInterpret::SuccessEnterRoom(const CClientBase* pMyClient) const
{
	char* szMessage = HInterpretHelper::MakeSuccessMessage(m_szMessage[Client_Position::LOBBY_MSG_ENTER_ROOM]);

    HInterpretHelper::MakeSocketMessageEnd(szMessage);

	pMyClient->SendNote(szMessage);

	Safe_Delete(szMessage);
}

void CLobbyInterpret::FailedEnterRoom(const CClientBase* pMyClient) const
{
	char* szMessage = HInterpretHelper::MakeFailedMessage(m_szMessage[Client_Position::LOBBY_MSG_ENTER_ROOM]);

    HInterpretHelper::MakeSocketMessageEnd(szMessage);

	pMyClient->SendNote(szMessage);

	Safe_Delete(szMessage);
}

void CLobbyInterpret::SendClearLobby(const CClientBase* pMyClient) const
{
	char* szSendMessage = HInterpretHelper::ClearMessage();

    HInterpretHelper::MakeSocketMessageEnd(szSendMessage);

	pMyClient->SendNote(szSendMessage);

	Safe_Delete(szSendMessage);
}