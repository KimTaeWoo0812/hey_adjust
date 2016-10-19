#pragma once
#include "IClientBase.h"
#include "MessageBox.h"

class CClientBaseImp;

class CClientBase :
	public IClientBase
{
protected :
	DWORD					m_dwReceiveTime;

protected:
	SOCKET					m_hClientSocket;
	static std::mutex		m_Mutex;
	bool					m_bExit;
	int						m_nIndex;
	Client::CLIENT_INFO		m_tClientInfo;

protected:
	std::thread				m_hReceiverThread;
	std::thread				m_hWatchThread;
	CMessageBox<char*>*		m_pMessageBox;
	CMessageBox<int>*		m_pQuitBox;

public:
	static void ReceiveMessage(SOCKET hClientSocket, CMessageBox<char*>* pMessageBox, CMessageBox<int>* pQuitBox, const int& nIndex, bool* bExit, DWORD* dwStartTime);
	static void WatchToMe(CMessageBox<int>* pQuitBox, const int& nIndex, DWORD* dwReceiveTime, bool* bExit);

public:
	const bool	hasMessage()                        const;
	const char*	getMessage()                        const;
    const int   getIndex()                          const;
    const Client::CLIENT_INFO GetClientInfo()       const;
    void SendNote(const char* pMessage)             const;
    bool	isExit()                                const;

public:
    void AddPoint(const int& nPoint);
	void SetQuitBox(CMessageBox<int>* pQuitBox);
	void SetIndex(const int& nIndex);
	void SetClientInfo(const Client::CLIENT_INFO& tClientInfo);

public:
	virtual int Initialize(const SOCKET& pClient)		override;
	virtual int Progress()				        		override;
	virtual int Render()						        override;
	virtual int Release()						        override;

public:
	CClientBase();
	virtual ~CClientBase();
};

