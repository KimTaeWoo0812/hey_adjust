#pragma once

#include "ibase.h"

class CClientAdministrator;

class CMainServer :
	public IBase
{
private :
	bool*					m_pExit;

private :
	SOCKET					m_hServSock;
	CClientAdministrator*	m_pClient;
	std::thread				m_hThread;
	std::mutex				m_hMutex;
	bool					m_bCreateThread;

private :
	static void InputClient(CClientAdministrator* pClient, SOCKET hServerSock, std::mutex* mutex);

private :
	int InitSocket();
	void InitClass();
	void InitValue();

public :
	const int GetAccessClientCount();
	const int GetLoginClientCount();
	const int GetLobbyClientCount();
	const int GetRoomClientCount();
	const int GetRoomCount();

public :
	void SetExit(bool* pExit);

public :
	virtual int Initialize()					override;
	virtual int Progress()						override;
	virtual int Render()						override;
	virtual int Release()						override;

public:
	CMainServer(void);
	virtual ~CMainServer(void);
};