#pragma once

#include "IGameServer.h"
#include "GameLogic.h"
#include "GameTable.h"
#include "Random.h"

class CClientRoom;
class CClientLobby;

class CGameServer : public IGameServer
{
private :
	CClientRoom*		                m_pRoom;
	CClientLobby*		                m_pLobby;
	CGameLogic			                m_GameLogic;
	std::thread			                m_hLogicThread;
	bool				                m_bExit;
	std::queue<char*>	                m_tProblemQueue;
	CGameTable			                m_GameTable;
	int					                m_nProblemCount;
    CRandom                             m_Random;

private :
	static void StartLogic(CGameLogic* pLogic, bool* bExit);

private :
	void EndGame();
	void MakeProblem();
    void InitRandom();

public:
	virtual bool AddClient(CClientBase* pClientBase, const int& nIndex)		override;
	void StartGame();
	void SetRoom(CClientRoom* pRoom);
	void SetLobby(CClientLobby* pLobby);
	void SetProblemCount(const int& nProblemCount);

public :
	virtual int Initialize()		override;
	virtual int Progress()			override;
	virtual int Render()			override;
	virtual int Release()			override;

public:
	CGameServer();
	virtual ~CGameServer();
};