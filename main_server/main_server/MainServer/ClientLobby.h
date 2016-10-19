#pragma once

#include "ClientPositionBase.h"
#include "ClientRoom.h"
#include "LobbyInterpret.h"

class CClientLobby :
	public CClientPositionBase
{
private :
	CClientRoom			m_pRoom[ROOM_COUNT];
	CLobbyInterpret		m_Interpret;
	int					m_nRoomNumber;

private :
	LIST_ITER(CClientBase*) CreateRoom(CClientBase* pMyClient, const char* szMessage);
	LIST_ITER(CClientBase*) EnterRoom(CClientBase* pMyClient, const char* szMessage);
    void SendRoomInfo(const CClientBase* pMyClient);
    void SendUserInfo(const CClientBase* pMyClient);
    void SendLobbyUser(const CClientBase* pMyClient, const char* szMessage);
    void SendClearLobby(const CClientBase* pMyClient) const;

private :
	const int   			Factorial_CreateRoom(const CClientBase* pMyClient, const char* szMessage);
	LIST_ITER(CClientBase*) Factorial_EnterRoom(CClientBase* pMyClient, const int& nIndex);
	const int  GetRoomIndex();
	void AllRefresh();

protected:
	bool CheckList()						override;
	void CheckRoom();
	void Interpreter();
    void Refresh(const CClientBase* pMyClient);

public:
    bool AddClient(CClientBase* pClient)	override;
    const int GetLobbyClientCount();
    const int GetRoomClientCount();
    const int GetRoomCount();

public:
    void EndBack();

public:
	virtual int Initialize()				override;
	virtual int Progress()					override;
	virtual int Render()					override;
	virtual int Release()					override;

public:
	CClientLobby();
	~CClientLobby();
};
