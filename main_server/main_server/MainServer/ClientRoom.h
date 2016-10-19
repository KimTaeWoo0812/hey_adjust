#pragma once

#include "ClientPositionBase.h"
#include "RoomInterpret.h"
#include "ClientBase.h"
#include "GameServer.h"

class CClientLobby;

class CClientRoom :
	public CClientPositionBase
{
private :
	Client_Position::ROOM_CLIENT	m_tRoomClient[CLINET_MAX_IN_ROOM];
	Client_Position::ROOM_DATA		m_tRoomData;
	CRoomInterpret					m_Interpret;
	CClientLobby*					m_pLobbyPosition;
	CGameServer						m_GameServer;

private :
    void PickTopic(const char* szMessage);

private :
	void SuccessStartGame();
	void FailedStartGame();
	void InitRoomClient();
    void InitRoomData();
    void SendRoomInfo(const CClientBase* pMyClient) const;
    void SendUserInfo(const Client_Position::ROOM_CLIENT& tRoomClient) const;
	int  getRoomEmptyIndex();
    const int  getClientIndex(const CClientBase* pMyClient);
	void AllOutOfRoom();
    void OutRoom(const int& nIndex);
    void PrevRoom(const int& nIndex);
	void Refresh();
    void Ready(const int& nIndex);
	void Start();
	void GoGame();
    void SendAllPickTopic();
    void SendPickTopic(CClientBase* pMyClient);

protected:
	bool CheckList()						override;
	void Interpreter();
	void AllMessage(char* pMessage);
    void SendRoomUser(const CClientBase* pMyClient, const char* szMessage) const;
    void SendClearUser(CClientBase* pMyClient);

public :
	void	CreateRoom(const int& nRoomNumber, const int& nMaxUserInRoom, char* szRoomName);
	void	DeleteRoom();
	int		getUserCount();
	bool	isCreate();
	int		getRoomNumber();
	char*	getRoomName();
	bool	isEnterRoom();
	void	EndGame();
	bool    isStartGame();
	Client_Position::ROOM_DATA	GetRoomData();

public:
	bool AddClient(CClientBase* pClient)	override;
	void BackToClient(CClientBase* pClient, const int& nIndex);
    void BackToRoomData(Client_Position::ROOM_DATA tData);
	void SetLobby(CClientLobby* pLobby);
	void SetName(char* pRoomName);

public:
	virtual int Initialize()				override;
	virtual int Progress()					override;
	virtual int Render()					override;
	virtual int Release()					override;

public:
	CClientRoom();
	virtual ~CClientRoom();
};

