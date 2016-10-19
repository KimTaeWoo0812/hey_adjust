#pragma once
#include "IBase.h"
#include "ClientBase.h"
#include "MessageBox.h"

class CClientLogin;

class CClientAdministrator :
	public IBase
{
private :
	CClientBase				m_pClientBase[CLIENT_MAX_COUNT];
	CClientLogin*			m_pLogin;
	CMessageBox<int>*		m_pQuitBox;
	int						m_nClientIndex;

private:
	void Interpretation();
	void ClientRelease();

private :
	void InitValue();

private :
	void InitClass();
	void ReleaseClass();

public :
	void CreateClient(const SOCKET& pClient);


public:
	const int GetAccessClientCount();
	const int GetLoginClientCount();
	const int GetLobbyClientCount();
	const int GetRoomClientCount();
	const int GetRoomCount();

public:
	virtual int Initialize()					override;
	virtual int Progress()						override;
	virtual int Render()						override;
	virtual int Release()						override;

public:
	CClientAdministrator();
	virtual ~CClientAdministrator();
};