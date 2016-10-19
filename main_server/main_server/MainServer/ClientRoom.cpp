#include "ClientRoom.h"
#include "GameServer.h"
#include "ClientLobby.h"

CClientRoom::CClientRoom()
{
}


CClientRoom::~CClientRoom()
{
}

int CClientRoom::Initialize()
{
	InitRoomClient();
    InitRoomData();

	return 0;
}

void CClientRoom::InitRoomClient()
{
	for (int i = 0; i < CLINET_MAX_IN_ROOM; ++i)
		m_tRoomClient[i] = Client_Position::ROOM_CLIENT();
}

void CClientRoom::InitRoomData()
{
    m_tRoomData = Client_Position::ROOM_DATA();
}

int CClientRoom::Progress()
{
	if (!m_tRoomData.bStart)
	{
		CheckList();

		Interpreter();
	}

	else m_GameServer.Progress();

	return 0;
}

int CClientRoom::Render()
{
	return 0;
}

int CClientRoom::Release()
{
    m_GameServer.Release();
	return 0;
}

bool CClientRoom::AddClient(CClientBase* pClient)
{
	if (!isEnterRoom()) 
		return false;

	++m_tRoomData.nCurrentClientNumber;
	m_tRoomClient[getRoomEmptyIndex()].pClient = pClient;
	Refresh();

	return true;
}

void CClientRoom::BackToClient(CClientBase* pClient, const int& nIndex)
{
	m_tRoomClient[nIndex].pClient = pClient;
	Refresh();
}

bool CClientRoom::CheckList()
{
	bool isChange = false;

	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient != nullptr)
		{
			if (m_tRoomClient[i].pClient->isExit())
			{
				--m_tRoomData.nCurrentClientNumber;
				m_tRoomClient[i] = Client_Position::ROOM_CLIENT();
				isChange = true;
			}
		}
	}

	if (isChange) Refresh();

	return true;
}

void CClientRoom::Interpreter()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
		if (m_tRoomClient[i].pClient != nullptr)
		{
			if (m_tRoomClient[i].pClient->hasMessage())
			{
				const char* message = m_tRoomClient[i].pClient->getMessage();
				printf("%s\n", message);

				switch (m_Interpret.InterpretMessage(message))
				{
				case Client_Position::ROOM_MSG_CHAT:
					SendRoomUser(m_tRoomClient[i].pClient, message);
					break;

				case Client_Position::ROOM_MSG_PREV:
					PrevRoom(i);
					break;

				case Client_Position::ROOM_MSG_READY :
					Ready(i);
					break;

				case Client_Position::ROOM_MSG_START :
					Start();
					break;

                case Client_Position::ROOM_MSG_CATEGORY:
					PickTopic(message);
					break;
				}
				Safe_Delete(message);
			}
		}
	}
}

void CClientRoom::SetLobby(CClientLobby* pLobby)
{
	m_pLobbyPosition = pLobby;
}

void CClientRoom::CreateRoom(const int& nRoomNumber, const int& nMaxUserInRoom, char* szRoomName)
{
	ZeroMemory(m_tRoomData.szRoomName, SOCKET_MSG_ONE_ARRAY_SIZE);
	m_tRoomData.nCurrentClientNumber = 0;
	m_tRoomData.nRoomNumber = nRoomNumber;
	m_tRoomData.nMaxClientInRoom = nMaxUserInRoom;
	strcpy(m_tRoomData.szRoomName, szRoomName);
}

void CClientRoom::DeleteRoom()
{
	ZeroMemory(m_tRoomData.szRoomName, SOCKET_MSG_ONE_ARRAY_SIZE);
	InitRoomClient();
	m_tRoomData.nRoomNumber = -1;
	m_tRoomData.nMaxClientInRoom = -1;
}

int CClientRoom::getUserCount()
{
	return m_tRoomData.nCurrentClientNumber;
}

bool CClientRoom::isCreate()
{
	return m_tRoomData.nRoomNumber > 0;
}

void CClientRoom::SetName(char* szName)
{
	ZeroMemory(m_tRoomData.szRoomName, SOCKET_MSG_ONE);
	memcpy(m_tRoomData.szRoomName, szName, strlen(szName));
}

int CClientRoom::getRoomNumber()
{
	return m_tRoomData.nRoomNumber;
}

char* CClientRoom::getRoomName()
{
	return m_tRoomData.szRoomName;
}

bool CClientRoom::isEnterRoom()
{
	if (m_tRoomData.nCurrentClientNumber + 1 > m_tRoomData.nMaxClientInRoom)
		return false;

    if (m_tRoomData.bStart)
        return false;

	return true;
}

void CClientRoom::SendUserInfo(const Client_Position::ROOM_CLIENT& tRoomClient) const
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient == nullptr)
			continue;

		m_Interpret.SendRoomUserInfo(tRoomClient, m_tRoomClient[i], i);
	}

	m_Interpret.SendRoomUserInfoEnd(tRoomClient.pClient);
}

void CClientRoom::SendRoomInfo(const CClientBase* pMyClient) const
{
	m_Interpret.SendRoomInfo(pMyClient, m_tRoomData);
}

void CClientRoom::SendClearUser(CClientBase* pMyClient)
{
    m_Interpret.ClearRoomMessage(pMyClient);
}

void CClientRoom::SendRoomUser(const CClientBase* pMyClient, const char* szMessage) const
{
	char* szChatMessage = m_Interpret.ChatMessage(pMyClient, szMessage);

	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient != nullptr)
        {
            HInterpretHelper::MakeSocketMessageEnd(szChatMessage);
            m_tRoomClient[i].pClient->SendNote(szChatMessage);
        }
	}

	Safe_Delete(szChatMessage);
}

int CClientRoom::getRoomEmptyIndex()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient == nullptr)
			return i;
	}

	return -1;
}

void CClientRoom::OutRoom(const int& nIndex)
{
    if (m_tRoomClient[nIndex].pClient != nullptr)
	{
		--m_tRoomData.nCurrentClientNumber;
		m_Interpret.SendOutRoom(m_tRoomClient[nIndex].pClient, m_pLobbyPosition);
		m_tRoomClient[nIndex] = Client_Position::ROOM_CLIENT();
	}

	m_pLobbyPosition->EndBack();
}

void CClientRoom::AllOutOfRoom()
{
	for (int i = 0; i < CLINET_MAX_IN_ROOM; ++i)
		OutRoom(i);

	DeleteRoom();
}

void CClientRoom::PrevRoom(const int& nIndex)
{
	if (nIndex == -1) return;

	if (nIndex == 0)
	{
		AllOutOfRoom();
		return;
	}

	--m_tRoomData.nCurrentClientNumber;
	m_Interpret.SendPrevRoom(m_tRoomClient[nIndex].pClient, m_pLobbyPosition);
	m_tRoomClient[nIndex] = Client_Position::ROOM_CLIENT();

	((CClientLobby*)m_pLobbyPosition)->EndBack();

	Refresh();
}

const int CClientRoom::getClientIndex(const CClientBase* pMyClient)
{
	for (int i = 0; i < CLINET_MAX_IN_ROOM; ++i)
	{
		if (m_tRoomClient[i].pClient == pMyClient)
			return i;
	}

	return -1;
}

void CClientRoom::Refresh()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient != nullptr)
		{
            SendClearUser(m_tRoomClient[i].pClient);
			SendRoomInfo(m_tRoomClient[i].pClient);
			SendUserInfo(m_tRoomClient[i]);
            SendPickTopic(m_tRoomClient[i].pClient);
		}
	}
}

void CClientRoom::Ready(const int& nIndex)
{
	m_tRoomClient[nIndex].bReady = !m_tRoomClient[nIndex].bReady;

	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient != nullptr)
        {
            SendClearUser(m_tRoomClient[i].pClient);
            SendUserInfo(m_tRoomClient[i]);
        }
	}
}

void CClientRoom::Start()
{
	int nReadyNumber = 1;

	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient != nullptr)
		{
			if (m_tRoomClient[i].bReady == true)
				++nReadyNumber;
		}
	}

	if (m_tRoomData.nCurrentClientNumber == nReadyNumber)
	{
		SuccessStartGame();
		GoGame();
	}
	else FailedStartGame();
}

void CClientRoom::SuccessStartGame()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient != nullptr)
			m_Interpret.SuccessStartGame(m_tRoomClient[i].pClient);
	}
}

void CClientRoom::FailedStartGame()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient != nullptr)
			m_Interpret.FailedStartGame(m_tRoomClient[i].pClient);
	}
}

void CClientRoom::GoGame()
{
    m_GameServer.SetRoom(this);
    m_GameServer.SetLobby(m_pLobbyPosition);
    m_GameServer.SetProblemCount(5);
	m_GameServer.Initialize();

	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_tRoomClient[i].pClient != nullptr)
		{
			m_GameServer.AddClient(m_tRoomClient[i].pClient, i);
			m_tRoomClient[i] = Client_Position::ROOM_CLIENT();
		}
	}
	m_tRoomData.bStart = true;
	m_GameServer.StartGame();
}

void CClientRoom::EndGame()
{
	m_tRoomData.bStart = false;
}

bool CClientRoom::isStartGame()
{
	return m_tRoomData.bStart;
}

Client_Position::ROOM_DATA CClientRoom::GetRoomData()
{
	return m_tRoomData;
}

void CClientRoom::PickTopic(const char* szMessage)
{
    m_tRoomData.nPickTopic = m_Interpret.GetPickTopicNumber(szMessage);
    SendAllPickTopic();
}

void CClientRoom::SendAllPickTopic()
{
    for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
    {
        if (m_tRoomClient[i].pClient != nullptr)
            m_Interpret.SendPickTopic(m_tRoomClient[i].pClient, m_tRoomData.nPickTopic);
    }
}

void CClientRoom::SendPickTopic(CClientBase* pMyClient)
{
    m_Interpret.SendPickTopic(pMyClient, m_tRoomData.nPickTopic);
}

void CClientRoom::BackToRoomData(Client_Position::ROOM_DATA tData)
{
    m_tRoomData = tData;
}