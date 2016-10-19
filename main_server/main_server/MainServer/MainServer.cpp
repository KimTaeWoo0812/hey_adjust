#include "MainServer.h"
#include "ClientAdministrator.h"
#include "DataBase.h"
#include "Random.h"

CMainServer::CMainServer(void)
{
}


CMainServer::~CMainServer(void)
{
}

int CMainServer::Initialize()
{
	InitSocket();
	InitClass();
	InitValue();

	return 0;
}

int CMainServer::Progress()
{
	if (!m_bCreateThread)
	{
		m_hThread = std::thread(InputClient, m_pClient, m_hServSock, &m_hMutex);
		m_bCreateThread = !m_bCreateThread;
	}
	
    std::lock_guard<std::mutex> lock(m_hMutex);
		m_pClient->Progress();
	
	return !(*m_pExit);
}

int CMainServer::Render()
{
	return 0;
}

int CMainServer::Release()
{
	TerminateThread(m_hMutex.native_handle(), NULL);
	TerminateThread(m_hThread.native_handle(), NULL);
	m_hThread.join();
	closesocket(m_hServSock);
	m_pClient->Release();
	Safe_Delete(m_pClient);
	GetSingle<CDataBase>()->DestroyInstance();
	return 0;
}

int CMainServer::InitSocket()
{
	WSADATA wsaData;
	SOCKADDR_IN servAddr;
	int portNum = 7788;
	// 보낼 데이터를 초기화한다. 

	if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
		printf("WSAStartup() error!");

	m_hServSock=socket(PF_INET, SOCK_STREAM, 0);

	if(m_hServSock==INVALID_SOCKET)
		printf("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port = htons(portNum);

	if(bind(m_hServSock, (SOCKADDR*) &servAddr, sizeof(servAddr))==SOCKET_ERROR)
		printf("bind() error");

	if(listen(m_hServSock, 5)==SOCKET_ERROR)
		printf("listen() error");

	return 0;
}

void CMainServer::InitClass()
{
	m_pClient = new CClientAdministrator();
	m_pClient->Initialize();

	GetSingle<CDataBase>()->Initialize();
}

void CMainServer::InputClient(CClientAdministrator* pClient, SOCKET hServerSock, std::mutex* mutex)
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
	int nClntAddr = sizeof(clntAddr);

	while (true)
	{
		hClntSock = accept(hServerSock, (SOCKADDR*)&clntAddr, &nClntAddr);

		printf("Client Access\n");

        std::lock_guard<std::mutex> lock((*mutex));
			pClient->CreateClient(hClntSock);
	}
}

void CMainServer::InitValue()
{
	m_bCreateThread = FALSE;
}

void CMainServer::SetExit(bool* pExit)
{
	m_pExit = pExit;
}

const int CMainServer::GetAccessClientCount()
{
	return m_pClient->GetAccessClientCount();
}

const int CMainServer::GetLoginClientCount()
{
	return m_pClient->GetLoginClientCount();
}

const int CMainServer::GetLobbyClientCount()
{
	return m_pClient->GetLobbyClientCount();
}

const int CMainServer::GetRoomClientCount()
{
	return m_pClient->GetRoomClientCount();
}

const int CMainServer::GetRoomCount()
{
	return m_pClient->GetRoomCount();
}