#pragma once

#include "DataBase.h"
#include "ClientBase.h"
#include "InterpretHelper.h"
#include "ClientPositionBase.h"

class CRoomInterpret
{
private:
	char m_szMessage[Client_Position::ROOM_MSG_COUNT][SOCKET_MSG_ONE_ARRAY_SIZE];

private:
    void EndPointMessage(const CClientBase* hMyClient, const Client_Position::Room_Message& eMessage) const;
    void MoveToLobby(CClientBase* pMyClient, CClientPositionBase* pLobby);

public:
    void SendRoomUserInfo(const Client_Position::ROOM_CLIENT& pMyClient, const Client_Position::ROOM_CLIENT&, const int& nPositionIndex) const;
    void SendRoomUserInfoEnd(const CClientBase* hMyClient) const;

public :
    void SendRoomInfo(const CClientBase* pMyClient, const Client_Position::ROOM_DATA& tRoomData) const;

public :
    void SendOutRoom(CClientBase* pMyClient, CClientPositionBase* pLobby);
    void SendPrevRoom(CClientBase* pMyClient, CClientPositionBase* pLobby);

public :
    void SuccessStartGame(const CClientBase* pMyClient) const;
    void FailedStartGame(const CClientBase* pMyClient) const;

public:
    const Client_Position::Room_Message InterpretMessage(const char* szMessage) const;

public:
    char* ChatMessage(const CClientBase* hMyClient, const char* szMessage) const;

public :
    void SendPickTopic(const CClientBase* pMyClient, char* szMessage) const;
    void SendPickTopic(const CClientBase* pMyClient, const int& nPickTopic) const;
    const int  GetPickTopicNumber(const char* szMessage) const;

public :
    void ClearRoomMessage(const CClientBase* pMyClient) const;

public:
	CRoomInterpret();
	~CRoomInterpret();
};

