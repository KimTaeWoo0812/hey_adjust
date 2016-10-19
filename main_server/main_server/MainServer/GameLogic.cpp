#include "GameLogic.h"
#include "ClientBase.h"
#include "DataBase.h"
#include "ClientLobby.h"
#include "DataBase.h"
#include "HStringIncoding.h"

CGameLogic::CGameLogic()
{
}


CGameLogic::~CGameLogic()
{
}

int CGameLogic::Initialize()
{
	InitClientArray();
	m_bAbnormalExit = m_bExit = false;

	return 0;
}

int CGameLogic::Progress()
{
	CheckArray();

    CheckClientCount();

    Interpreter();

	return !m_bExit;
}

int CGameLogic::Render()
{
	return 0;
}

int CGameLogic::Release()
{
	return 0;
}

bool CGameLogic::AddClient(CClientBase* pClientBase, const int& nIndex)
{
	m_pClientArray[nIndex] = Game::GAME_CLIENT_INFO(pClientBase);

	return true;
}

void CGameLogic::InitClientArray()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
        m_pClientArray[i] = Game::GAME_CLIENT_INFO();
}

void CGameLogic::CheckArray()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_pClientArray[i].pClient != nullptr)
        {
            if (m_pClientArray[i].pClient->isExit())
            {
                --m_tRoomData.nCurrentClientNumber;
                SaveClient(m_pClientArray[i]);
                if (i == 0)
                {
                    m_bExit = true;
                    m_bAbnormalExit = true;
                }
                m_pClientArray[i] = Game::GAME_CLIENT_INFO();
            }
            if (m_bExit) break;
        }
	}
}

void CGameLogic::CheckClientCount()
{
    if (m_tRoomData.nCurrentClientNumber == 0)
		m_bExit = true;
}

void CGameLogic::Interpreter()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_pClientArray[i].pClient != nullptr)
		{
			if (m_pClientArray[i].pClient->hasMessage())
			{
				const char* message = m_pClientArray[i].pClient->getMessage();
				printf("%s\n", message);

				switch (m_Interpret.InterpretMessage(message))
				{
				case Game::GAME_MSG_CHAT :
					SendGameUser(m_pClientArray[i].pClient, message);
					break;

				case Game::GAME_MSG_PREV :
					PrevLobby(i);
					break;

				case Game::GAME_MSG_TIME_OUT :
					ProgressTimeOut();
					break;

                case Game::GAME_MSG_SUCCESS :
                    SendSuccessUser(i);
                    break;
				}

				Safe_Delete(message);
			}
		}
	}
}

void CGameLogic::SendGameUser(const Game::GAME_CLIENT_INFO& pMyClientInfo, const char* szMessage) const
{
	char* szChatMessage = m_Interpret.ChatMessage(pMyClientInfo.pClient, szMessage);

	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_pClientArray[i].pClient != nullptr)
        {
            HInterpretHelper::MakeSocketMessageEnd(szChatMessage);
            m_pClientArray[i].pClient->SendNote(szChatMessage);
        }
	}
	Safe_Delete(szChatMessage);
}

CClientBase* CGameLogic::GetClient(const int& nIndex) const
{
	CClientBase* pClient = m_pClientArray[nIndex].pClient;

	return pClient;
}

const int CGameLogic::GetClientCount() const
{
    return m_tRoomData.nCurrentClientNumber;
}

const bool CGameLogic::GetAbnormalExit() const
{
	return m_bAbnormalExit;
}

void CGameLogic::CallOutOfGame()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_pClientArray[i].pClient != nullptr)
			m_Interpret.SendOutOfGame(m_pClientArray[i].pClient);
	}
}

const bool CGameLogic::GetExit() const
{
	return m_bExit;
}

void CGameLogic::ExitLogic()
{
	m_bExit = TRUE;
}

void CGameLogic::Refresh()
{
    for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
    {
        if (m_pClientArray[i].pClient != nullptr)
        {
            SendRoomInfo(m_pClientArray[i].pClient);
            SendUserInfo(m_pClientArray[i].pClient);
            SendRightInfo(m_pClientArray[i].pClient);
        }
    }
}

void CGameLogic::SendUserInfo(const CClientBase* pMyClient) const
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_pClientArray[i].pClient != nullptr)
            m_Interpret.SendGameUserInfo(pMyClient, m_pClientArray[i].pClient, i);
	}

	m_Interpret.SendGameUserInfoEnd(pMyClient);
}

void CGameLogic::SendRoomInfo(const CClientBase* pMyClient) const
{
    m_Interpret.SendGameRoomInfo(pMyClient, m_tRoomData.nRoomNumber, m_tRoomData.szRoomName, m_tRoomData.nMaxClientInRoom, m_tRoomData.nCurrentClientNumber);
}

void CGameLogic::SendRightInfo(const CClientBase* pMyClient) const
{
    for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
    {
        if (m_pClientArray[i].pClient != nullptr)
            m_Interpret.SendUserInfoRight(pMyClient, i, m_pClientArray[i].nRightCount);
    }

    m_Interpret.SendUserInfoRightEnd(pMyClient);
}

void CGameLogic::SendSuccessUser(const int& nIndex)
{
    ++m_pClientArray[nIndex].nRightCount;

    for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
    {
        if (m_pClientArray[i].pClient != nullptr)
            m_Interpret.SendSuccessMessage(m_pClientArray[i].pClient, m_pClientArray[nIndex].pClient);
    }

    ChangeProblem();

    for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
    {
        if (m_pClientArray[i].pClient != nullptr)
            m_Interpret.SendUserInfoRight(m_pClientArray[i].pClient, nIndex, m_pClientArray[nIndex].nRightCount);

    }

    //Refresh();
}

void CGameLogic::SetRoomData(const Client_Position::ROOM_DATA& tRoomData)
{
	m_tRoomData = tRoomData;
}

void CGameLogic::ProgressTimeOut()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_pClientArray[i].pClient != nullptr)
			m_Interpret.SendTimeOut(m_pClientArray[i].pClient);
	}
    m_bExit = true;
	Refresh();
}

void CGameLogic::SetLobby(CClientLobby* pLobby)
{
	m_pLobby = pLobby;
}

void CGameLogic::PrevLobby(const int& nIndex) 
{
    if (nIndex == 0)
    {
        m_bExit = true;
        m_bAbnormalExit = true;
    }

    else
    {
        --m_tRoomData.nCurrentClientNumber;
        m_pLobby->AddClient(m_pClientArray[nIndex].pClient);
    }

    SaveClient(m_pClientArray[nIndex]);
    MoveToLobby(m_pClientArray[nIndex].pClient);
	m_pClientArray[nIndex] = Game::GAME_CLIENT_INFO();
}

void CGameLogic::MoveToLobby(CClientBase* pMyClient)
{
    m_pLobby->AddClient(pMyClient);
    Refresh();
}

void CGameLogic::CallSaveClient()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_pClientArray[i].pClient != nullptr)
			SaveClient(m_pClientArray[i]);
	}
}

void CGameLogic::SaveClient(const Game::GAME_CLIENT_INFO& tClient)
{
    tClient.pClient->AddPoint(tClient.nRightCount);

    Client::CLIENT_INFO tInfo = tClient.pClient->GetClientInfo();

	GetSingle<CDataBase>()->UpdateRankAndPoint(tInfo.szNickName, tInfo.nPoint);
}

void CGameLogic::CallEndGame()
{
	for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
	{
        if (m_pClientArray[i].pClient != nullptr)
			m_Interpret.EndGame(m_pClientArray[i].pClient);
	}
}

void CGameLogic::CallCategory()
{
    for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
    {
        if (m_pClientArray[i].pClient != nullptr)
            m_Interpret.SendCategory(m_pClientArray[i].pClient, m_tRoomData.nPickTopic);
    }
}

void CGameLogic::SetProblemQueue(std::queue<char*>* pProblemQueue)
{
    m_pProblemQueue = pProblemQueue;
    ChangeProblem();
}

void CGameLogic::ChangeProblem()
{
    if (m_pProblemQueue->size() == 0)
    {
        m_bExit = true;
        return;
    }

    if (m_pProblemQueue->empty())
    {
        m_bExit = true;
        return;
    }

    char* szProblem;
    szProblem = m_pProblemQueue->front();
    m_pProblemQueue->pop();

    char* szUTF8 = HStringIncoding::ANSIToUniCodeToUTF8(szProblem);

    for (int i = 0; i < m_tRoomData.nMaxClientInRoom; ++i)
    {
        if (m_pClientArray[i].pClient != nullptr)
            m_Interpret.SendPickTopic(m_pClientArray[i].pClient, szUTF8);
    }

    Safe_Delete(szUTF8);
    //Safe_Delete(szProblem);
}

const Client_Position::ROOM_DATA CGameLogic::GetRoomData() const
{
    return m_tRoomData;
}