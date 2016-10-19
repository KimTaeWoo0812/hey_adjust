#pragma once

#include "IBase.h"
#include "DataBase.h"
#include "ClientBase.h"
#include "InterpretHelper.h"
#include "ClientRoom.h"

class CLobbyInterpret
{
private:
	char m_szMessage[Client_Position::LOBBY_MSG_COUNT][SOCKET_MSG_ONE_ARRAY_SIZE];

private :
	void EndPointMessage(const CClientBase* hMyClient, const Client_Position::Lobby_Message& eMessage) const;

public :
    void SendRoomInfo(const CClientBase* hMyClient, const Client_Position::ROOM_DATA& tRoomData) const;
	void SendRoomInfoEnd(const CClientBase* hMyClient) const;

public :
	void SendLobyUserInfo(const CClientBase* hMyClient, const CClientBase* hYourClient) const;
	void SendLobyUserInfoEnd(const CClientBase* hMyClient) const;

public :
    void CreateRoom(CClientRoom* pRoomInfo, const char* szMessageconst, const int& nRoomIndex) const;
	void SuccessCreateRoom(const CClientBase* pClient, const int& nRoomIndex) const;
	void FailedCreateRoom(const CClientBase* pMyClient) const;

public :
	void SendClearLobby(const CClientBase* pMyClient) const;

public :
	const Client_Position::Lobby_Message InterpretMessage(const char* szMessage) const;

public :
	char* ChatMessage(const CClientBase* hMyClient, const char* szMessage) const;

public :
	const int InterpretRoomNumber(const char* message) const;
	void SuccessEnterRoom(const CClientBase* pMyClient) const;
	void FailedEnterRoom(const CClientBase* pMyClient) const;

public:
	CLobbyInterpret();
	~CLobbyInterpret();
};

