#include "ServerInterface.h"


CServerInterface::CServerInterface()
{
}


CServerInterface::~CServerInterface()
{
}

int CServerInterface::Initialize()
{
	m_bExit = FALSE;
	m_MainServer.Initialize();
	m_MainServer.SetExit(&m_bExit);
	m_hMainServerThread = std::thread(RunMainServer, &m_MainServer);

	return TRUE;
}

int CServerInterface::Progress()
{
	fflush(stdin);

	char szMessage[SOCKET_MSG_BUF];

	ZeroMemory(szMessage, sizeof(szMessage));
	scanf("%s", szMessage);

	if (strcmp(szMessage, "help") == 0)
	{
		printf("Command is....\n");
		printf("Quit : Server quit\n");
		printf("Clear : Clear to command\n");
		printf("TotalClient : Accesss to game user count\n");
		printf("LoginClient : During login client count\n");
		printf("LobbyClinet : During lobby client count\n");
		printf("RoomClient : During room client count\n");
		printf("RoomCount : how many server has room\n");
	}

	else if (strcmp(szMessage, "Quit") == 0)
		return false;

	else if (strcmp(szMessage, "Clear") == 0)
		system("cls");

	else if (strcmp(szMessage, "TotalClient") == 0)
		printf("%d\n",m_MainServer.GetAccessClientCount());

	else if (strcmp(szMessage, "LoginClient") == 0)
		printf("%d\n",m_MainServer.GetLoginClientCount());

	else if (strcmp(szMessage, "LobbyClient") == 0)
		printf("%d\n",m_MainServer.GetLobbyClientCount());

	else if (strcmp(szMessage, "RoomClient") == 0)
		printf("%d\n",m_MainServer.GetRoomClientCount());

	else if (strcmp(szMessage, "RoomCount") == 0)
		printf("%d\n",m_MainServer.GetRoomCount());

	else
	{
		printf("%s, I don't know this command\n", szMessage);
		printf("Plz.. Check your Sentence\n");
	}

	return true;
}

int CServerInterface::Render()
{
	return TRUE;
}

int CServerInterface::Release()
{
	m_bExit = TRUE;
	m_hMainServerThread.join();
	m_MainServer.Release();

	return TRUE;
}


void CServerInterface::RunMainServer(CMainServer* pMainServer)
{
	while (pMainServer->Progress());
}
