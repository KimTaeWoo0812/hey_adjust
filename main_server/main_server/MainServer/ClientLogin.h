#pragma once

#include "ClientPositionBase.h"
#include "LoginInterpret.h"

class CClientLobby;

class CClientLogin :
	public CClientPositionBase
{
private :
	CClientLobby*							m_pLobby;
	CLoginInterpret							m_Interpret;

protected :
	bool CheckList()						override;
	void Interpreter();

public :
	bool AddClient(CClientBase* pClient)	override;

public :
	int GetLoginClientCount();
	int GetLobbyClientCount();
	int GetRoomClientCount();
	int GetRoomCount();

public:
	virtual int Initialize()				override;
	virtual int Progress()					override;
	virtual int Render()					override;
	virtual int Release()					override;

public:
	CClientLogin();
	virtual ~CClientLogin();
};

