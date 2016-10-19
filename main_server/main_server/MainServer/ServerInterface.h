#pragma once

#include "stdafx.h"
#include "IBase.h"
#include "MainServer.h"

class CServerInterface : public IBase
{
private :
	std::thread		m_hMainServerThread;
	CMainServer		m_MainServer;
	bool			m_bExit;

private :
	static void RunMainServer(CMainServer* pMainServer);

public:
	virtual int Initialize()		override;
	virtual	int Progress()			override;
	virtual int Render()			override;
	virtual int Release()			override;

public:
	CServerInterface();
	virtual ~CServerInterface();
};

