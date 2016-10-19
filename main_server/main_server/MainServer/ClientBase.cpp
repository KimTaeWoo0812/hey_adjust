#include "ClientBase.h"
#include "MessageBox.h"

std::mutex CClientBase::m_Mutex;


CClientBase::CClientBase()
: m_pMessageBox(nullptr)
{
	m_bExit = TRUE;
}


CClientBase::~CClientBase()
{
}

int CClientBase::Initialize(const SOCKET& pClient)
{
	m_hClientSocket = pClient;

	m_pMessageBox = new CMessageBox<char*>();
	m_pMessageBox->Initialize();

	m_bExit = FALSE;

	m_hReceiverThread = std::thread(ReceiveMessage, m_hClientSocket, m_pMessageBox, m_pQuitBox, m_nIndex, &m_bExit, &m_dwReceiveTime);
	m_hWatchThread = std::thread(WatchToMe, m_pQuitBox, m_nIndex, &m_dwReceiveTime, &m_bExit);

	return 0;
}

int CClientBase::Progress()
{

	return 0;
}

int CClientBase::Render()
{
	return 0;
}

int CClientBase::Release()
{
	m_bExit = TRUE;
	closesocket(m_hClientSocket);
	TerminateThread(m_Mutex.native_handle(), NULL);
	TerminateThread(m_hReceiverThread.native_handle(), NULL);
	m_hReceiverThread.join();
	m_hWatchThread.join();
	Safe_Delete(m_pMessageBox);
	m_pMessageBox = nullptr;
	return 0;
}

void CClientBase::ReceiveMessage(SOCKET hClientSocket, CMessageBox<char*>* pMessageBox, CMessageBox<int>* pQuitBox, const int& nIndex, bool* bExit, DWORD* dwStartTime)
{
	char szBuffer[SOCKET_MSG_BUF];
	char* szQueue;
	int  nBuffer;

	while (true)
	{
		(*dwStartTime) = GetTickCount();
		ZeroMemory(szBuffer, SOCKET_MSG_BUF);
		nBuffer = recv(hClientSocket, szBuffer, SOCKET_MSG_BUF, 0);

		if (!(*bExit))
		{
			if (nBuffer <= 0)
			{
				for (int i = 0; i < SOCKET_MSG_BUF; ++i)
				{
					if (szBuffer[i] == '\n')
					{
						szBuffer[i] = NULL;
						break;
					}
				}
			}

			if (szBuffer[0] == 'T')
				continue;

			if (strcmp(szBuffer, "Quit") == 0 || nBuffer < 2)
			{
				(*bExit) = TRUE;
				pQuitBox->PushMessage(nIndex);
				break;
			}

            if (szBuffer[0] == NULL) continue;

            std::lock_guard<std::mutex> lock(m_Mutex);

			szQueue = new char[SOCKET_MSG_BUF];
			memcpy(szQueue, szBuffer, SOCKET_MSG_BUF);
			pMessageBox->PushMessage(szQueue);
		}
	}
}

void CClientBase::WatchToMe(CMessageBox<int>* pQuitBox, const int& nIndex, DWORD* dwReceiveTime, bool* bExit)
{
	while (!(*bExit))
	{
		DWORD dwCurrTime = GetTickCount() - (*dwReceiveTime);

		if (dwCurrTime >= 100000 && dwCurrTime <= 120000)
		{
			(*bExit) = TRUE;
			pQuitBox->PushMessage(nIndex);
		}
	}
}

void CClientBase::SendNote(const char* pMessage) const
{
	if (!m_bExit)
		send(m_hClientSocket, pMessage, SOCKET_MSG_BUF, 0);
}

bool CClientBase::isExit() const
{
	return m_bExit;
}

void CClientBase::SetIndex(const int& nIndex)
{
	m_nIndex = nIndex;
}

void CClientBase::SetQuitBox(CMessageBox<int>* pQuitBox)
{
	m_pQuitBox = pQuitBox;
}

const int CClientBase::getIndex() const
{
	return m_nIndex;
}

void CClientBase::AddPoint(const int& nPoint)
{
    m_tClientInfo.nPoint += nPoint;
}

const bool CClientBase::hasMessage() const
{
	return m_pMessageBox->hasMessage();
}

const char* CClientBase::getMessage() const
{
	return m_pMessageBox->getMessage();
}

void CClientBase::SetClientInfo(const Client::CLIENT_INFO& tClientInfo)
{
	m_tClientInfo = tClientInfo;
}

const Client::CLIENT_INFO CClientBase::GetClientInfo() const
{
	return m_tClientInfo;
}
/*
m_pImp->SetQuitBox(m_pQuitBox);
m_pImp->SetIndex(m_nIndex);
m_pImp->Initialize(m_hClient);*/