#pragma once

#include "stdafx.h"
#include "DataBase.h"
#include "ClientBase.h"
#include "InterpretHelper.h"
#include "ClientPositionBase.h"

class CGameLogicInterpret
{
private:
	char m_szMessage[Game::GAME_MSG_COUNT][SOCKET_MSG_ONE_ARRAY_SIZE];

public:
	const Game::Game_Message InterpretMessage(const char* szMessage);
	
public :
	void SendOutOfGame(const CClientBase* pMyClient) const;

public :
	void SendGameUserInfo(const CClientBase* pMyClient, const CClientBase* pYourClinet, const int& nPositionIndex) const;
	void SendGameUserInfoEnd(const CClientBase* pMyClient) const;

public :
    void SendUserInfoRight(const CClientBase* pMyClient, const int& nPositionIndex, const int& nRightNumber) const;
    void SendUserInfoRightEnd(const CClientBase* pMyClient) const;

public:
	void SendGameRoomInfo(const CClientBase* pMyClient, const int& nRoomIndex,
		const char* szRoomName, const int& nMaxClientInRoom, const int& nCurrRoomNumber) const;

public :
	void SendTimeStart(const CClientBase* pMyClient) const;
	void SendTimeOut(const CClientBase* pMyClient)  const;

public :
	void EndGame(const CClientBase* pMyClient)  const;

public:
    char* ChatMessage(const CClientBase* pMyClient, const char* szMessage) const;
    void SendSuccessMessage(const CClientBase* pMyClient, const CClientBase* pRightClient) const;

public :
    void SendPickTopic(const CClientBase* pMyClient, const char* szProblem) const;
    void SendCategory(const CClientBase* pMyClient, const int& nCategory) const;

public:
	CGameLogicInterpret();
	~CGameLogicInterpret();
};

