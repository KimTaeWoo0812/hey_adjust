#pragma once

#include "IGameServer.h"
#include "GameLogicInterpret.h"

class CClientLobby;

class CGameLogic : public IGameServer
{
private:
	Game::GAME_CLIENT_INFO			m_pClientArray[CLINET_MAX_IN_ROOM];
	CGameLogicInterpret				m_Interpret;
	bool							m_bAbnormalExit;
	bool							m_bExit;
	Client_Position::ROOM_DATA		m_tRoomData;
	CClientLobby*					m_pLobby;
	std::queue<char*>*				m_pProblemQueue;

private:
	void InitClientArray();
	void Interpreter();
	void CheckArray();
	void CheckClientCount();

private :
    void SaveClient(const Game::GAME_CLIENT_INFO& pClient);

private :
	void PrevLobby(const int& nIndex);

private :
    void ChangeProblem();

private :
	void ProgressTimeOut();

private :
	void Refresh();
    void SendUserInfo(const CClientBase* pMyClient)  const;
    void SendRoomInfo(const CClientBase* pMyClient)  const;
    void SendRightInfo(const CClientBase* pMyClient) const;
    void MoveToLobby(CClientBase* pMyClient) ;
    void SendGameUser(const Game::GAME_CLIENT_INFO& pMyClientInfo, const char* szMessage) const;
    void SendSuccessUser(const int& nIndex);

public :
    void SetRoomData(const Client_Position::ROOM_DATA& tRoomData);
    const Client_Position::ROOM_DATA GetRoomData() const;
    void SetLobby(CClientLobby* pLobby);
    void SetProblemQueue(std::queue<char*>* pProblemQueue);

public:
    void CallOutOfGame();
    void CallSaveClient();
    void CallEndGame();
    void CallCategory();

public:
	virtual bool AddClient(CClientBase* pClientBase, const int& nIndex)		override;

public :
	CClientBase* GetClient(const int& nIndex)         const;
	const int	 GetClientCount()                     const;
	const bool	 GetAbnormalExit()                    const;
	const bool	 GetExit()                            const;
	void		 ExitLogic();

public :
	virtual int Initialize()		override;
	virtual int Progress()			override;
	virtual int Render()			override;
	virtual int Release()			override;

public:
	CGameLogic();
	~CGameLogic();
};

