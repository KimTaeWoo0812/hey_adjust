#include "GameServer.h"
#include "ClientRoom.h"
#include "ClientLobby.h"
#include "DataBase.h"
#include "Random.h"

CGameServer::CGameServer()
{
}


CGameServer::~CGameServer()
{
}

int CGameServer::Initialize()
{
	m_GameLogic.Initialize();
	m_bExit = false;

    InitRandom();

	return 0;
}

void CGameServer::InitRandom()
{
    m_Random.Initialize();
    m_Random.SetNumber(1, 19);
}


int CGameServer::Progress()
{
	if (m_bExit)
	{//Thread가 종료되었다면....
		EndGame();
	}
	return 0;
}

int CGameServer::Render()
{
	return 0;
}

int CGameServer::Release()
{
	if (m_GameLogic.GetExit() == false)
	{
		m_GameLogic.ExitLogic();
		m_hLogicThread.join();
	}
    /*
	while (!m_tProblemQueue.empty())
	{
		Safe_Delete(m_tProblemQueue.front());
		m_tProblemQueue.pop();
	}*/

	m_GameLogic.Release();

	return 0;
}

bool CGameServer::AddClient(CClientBase* pClientBase, const int& nIndex)
{
	m_GameLogic.AddClient(pClientBase, nIndex);

	return true;
}

void CGameServer::SetRoom(CClientRoom* pRoom)
{
	m_pRoom = pRoom;
}

void CGameServer::SetProblemCount(const int& nProblemCount)
{
	m_nProblemCount = nProblemCount;
}

void CGameServer::SetLobby(CClientLobby* pLobby)
{
	m_pLobby = pLobby;
	m_GameLogic.SetLobby(pLobby);
}

void CGameServer::StartGame()
{
    m_GameLogic.SetRoomData(m_pRoom->GetRoomData());
	MakeProblem();
    //m_GameLogic.CallCategory();

    m_GameLogic.SetProblemQueue(&m_tProblemQueue);
	m_hLogicThread = std::thread(StartLogic, &m_GameLogic, &m_bExit);
}

void CGameServer::MakeProblem()
{
	bool isVisit[20];
	ZeroMemory(isVisit, sizeof(isVisit));
	int nIndex;
    
    while (true)
    {
        nIndex = m_Random.GetRandomNumber();

		if (isVisit[nIndex] == false)
		{
            char* szTopicKey = m_GameTable.GetTopic(m_pRoom->GetRoomData().nPickTopic);
            m_tProblemQueue.push(GetSingle<CDataBase>()->GetAnswer(szTopicKey, nIndex));

            Safe_Delete(szTopicKey);

			isVisit[nIndex] = true;
		}

        if (m_tProblemQueue.size() == 5) break;
	}
}

void CGameServer::StartLogic(CGameLogic* pLogic, bool* bExit)
{
	while (pLogic->Progress());
	(*bExit) = true;
}

void CGameServer::EndGame()
{
	int nClientCount = m_GameLogic.GetClientCount();

	if (m_GameLogic.GetAbnormalExit() == false)
	{//정상종료라면
        m_GameLogic.CallEndGame();

		for (int i = 0; i < nClientCount; ++i)
		{
			CClientBase* pClient = m_GameLogic.GetClient(i);

            if (pClient != nullptr)
				m_pRoom->BackToClient(pClient, i);
        }
        m_pRoom->BackToRoomData(m_GameLogic.GetRoomData());
	}


	else
	{//정상 종료가 아니라면!
		m_GameLogic.CallOutOfGame();

        for (int i = 0; i < nClientCount; ++i)
        {
            CClientBase* pClient = m_GameLogic.GetClient(i);

            if (pClient != nullptr)
                m_pLobby->AddClient(pClient);
        }
        m_pRoom->Initialize();
	}

	m_bExit = false;

	m_GameLogic.CallSaveClient();
	m_pRoom->EndGame();
    m_hLogicThread.join();
	Release();
}