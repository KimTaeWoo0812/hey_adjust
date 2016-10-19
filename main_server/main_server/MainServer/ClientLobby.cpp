#include "ClientLobby.h"

CClientLobby::CClientLobby()
{
}


CClientLobby::~CClientLobby()
{
}

int CClientLobby::Initialize()
{
	for (int i = 0; i < 10; ++i)
	{
		m_pRoom[i].SetLobby(this);
		m_pRoom[i].Initialize();
	}

	return 0;
}

int CClientLobby::Progress()
{
	CheckList();

	Interpreter();

	for (int i = 0; i < 10; ++i)
	{
		if (m_pRoom[i].isCreate())
		{
			if (m_pRoom[i].getUserCount() == 0)
				m_pRoom[i].DeleteRoom();

			else
				m_pRoom[i].Progress();
		}
	}

	return 0;
}

int CClientLobby::Render()
{
	return 0;
}

int CClientLobby::Release()
{
	m_List.clear();

    for (int i = 0; i < ROOM_COUNT; ++i)
        m_pRoom[i].Release();

	return 0;
}

void CClientLobby::SendRoomInfo(const CClientBase* pMyClient)
{
	for (int i = 0; i < ROOM_COUNT; ++i)
	{
        if (m_pRoom[i].isCreate())
            m_Interpret.SendRoomInfo(pMyClient, m_pRoom[i].GetRoomData());
	}
	m_Interpret.SendRoomInfoEnd(pMyClient);
}

void CClientLobby::SendClearLobby(const CClientBase* pMyClient) const
{
	m_Interpret.SendClearLobby(pMyClient);
}

void CClientLobby::SendUserInfo(const CClientBase* pMyClient)
{
    /*
	for (auto iter = m_List.begin();
		iter != m_List.end(); ++iter)
	{
		if ((*iter) == pMyClient)
			continue;

		m_Interpret.SendLobyUserInfo(pMyClient, (*iter));
	}*/

    for (auto iter : m_List)
    {
        if (iter == pMyClient)
            continue;

        m_Interpret.SendLobyUserInfo(pMyClient, iter);
    }

	m_Interpret.SendLobyUserInfoEnd(pMyClient);
}

void CClientLobby::SendLobbyUser(const CClientBase* pMyClient, const char* szMessage)
{
	char* szChatMessage = m_Interpret.ChatMessage(pMyClient, szMessage);

    /*
	for (auto iter = m_List.begin();
		iter != m_List.end(); ++iter)
	{
        HInterpretHelper::MakeSocketMessageEnd(szChatMessage);
		(*iter)->SendNote(szChatMessage);
	}*/

    for (auto iter : m_List)
    {
        HInterpretHelper::MakeSocketMessageEnd(szChatMessage);
        iter->SendNote(szChatMessage);
    }

	Safe_Delete(szChatMessage);
}

bool CClientLobby::AddClient(CClientBase* pClient)
{
	CClientPositionBase::AddClient(pClient);

    CheckRoom();

	AllRefresh();

	return true;
}

void CClientLobby::Refresh(const CClientBase* pMyClient)
{
	SendClearLobby(pMyClient);
	SendRoomInfo(pMyClient);
	SendUserInfo(pMyClient);
}

bool CClientLobby::CheckList()
{
	if(CClientPositionBase::CheckList())
		AllRefresh();

	return true;
}

void CClientLobby::Interpreter()
{
    auto iter = m_List.begin();
    auto delIter = iter;

	while (iter != m_List.end())
	{
		if ((*iter)->hasMessage())
		{
			const char* message = (*iter)->getMessage();
			printf("%s\n", message);

			switch (m_Interpret.InterpretMessage(message))
			{
			case Client_Position::LOBBY_MSG_REFRESH:
				Refresh((*iter));
				break;

			case Client_Position::LOBBY_MSG_CHAT :
				SendLobbyUser((*iter), message);
				break;

			case Client_Position::LOBBY_MSG_CREATE_ROOM :
				iter = CreateRoom((*iter), message);
				break;

			case Client_Position::LOBBY_MSG_ENTER_ROOM:
				iter = EnterRoom((*iter), message);
				break;
			}
			Safe_Delete(message);
		}
		if (iter != m_List.end())
			++iter;
	}
}

void CClientLobby::CheckRoom()
{
    bool bChange = false;

	for (int i = 0; i < ROOM_COUNT; ++i)
	{
		if (m_pRoom[i].isCreate())
		{
			if (m_pRoom[i].getUserCount() <= 0)
			{
                if (m_pRoom[i].isStartGame() == false)
                {
                    m_pRoom[i].DeleteRoom();
                    bChange = true;
                }
			}
		}
	}

    if (bChange) AllRefresh();
}

const int CClientLobby::GetLobbyClientCount()
{
	return m_List.size();
}

const int CClientLobby::GetRoomClientCount() 
{
	int nRoomClientCount = 0;

	for (int i = 0; i < ROOM_COUNT; ++i)
	{
		if (m_pRoom[i].isCreate())
			nRoomClientCount += m_pRoom[i].getUserCount();
	}

	return nRoomClientCount;
}

const int CClientLobby::GetRoomCount()
{
	int nRoomCount = 0;

	for (int i = 0; i < ROOM_COUNT; ++i)
	{
		if (m_pRoom[i].isCreate())
			++nRoomCount;
	}

	return nRoomCount;
}

const int CClientLobby::GetRoomIndex()
{
	for (int i = 0; i < ROOM_COUNT; ++i)
	{
		if (!m_pRoom[i].isCreate())
			return i;
	}

	return -1;
}

LIST_ITER(CClientBase*) CClientLobby::CreateRoom(CClientBase* pMyClient, const char* szMessage)
{
	auto ReturnIter = m_List.begin();
	int nRoomIndex;

	nRoomIndex = Factorial_CreateRoom(pMyClient, szMessage);

	if (nRoomIndex != -1)
		ReturnIter = Factorial_EnterRoom(pMyClient, nRoomIndex);

	else
	{
		m_Interpret.FailedCreateRoom(pMyClient);
		ReturnIter = m_List.end();
	}

	AllRefresh();

	return ReturnIter;
}

void CClientLobby::EndBack()
{
	CheckRoom();
	AllRefresh();	
}

const int CClientLobby::Factorial_CreateRoom(const CClientBase* pMyClient, const char* szMessage)
{
	int nRoomIndex = GetRoomIndex();

	if (nRoomIndex == -1) return -1;

	m_Interpret.CreateRoom(&m_pRoom[nRoomIndex], szMessage, nRoomIndex + 1);
	m_Interpret.SuccessCreateRoom(pMyClient, nRoomIndex + 1);

	return nRoomIndex;
}

LIST_ITER(CClientBase*) CClientLobby::EnterRoom(CClientBase* pMyClient, const char* szMessage)
{
	int nIndex = m_Interpret.InterpretRoomNumber(szMessage) - 1;
	auto ReturnIter = m_List.begin();

	if (nIndex < 0)
	{
		m_Interpret.FailedEnterRoom(pMyClient);
		return m_List.end();
	}

	if (m_pRoom[nIndex].isEnterRoom())
	{
		m_Interpret.SuccessEnterRoom(pMyClient);
		ReturnIter = Factorial_EnterRoom(pMyClient, nIndex);
		AllRefresh();
	}

	else
	{
		m_Interpret.FailedEnterRoom(pMyClient);
		ReturnIter = m_List.end();
	}

	return ReturnIter;
}

LIST_ITER(CClientBase*) CClientLobby::Factorial_EnterRoom(CClientBase* pMyClient, const int& nIndex)
{
	auto ReturnIter = m_List.begin();

	for (auto iter = m_List.begin();
		iter != m_List.end(); ++iter)
	{
		if ((*iter) == pMyClient)
		{
			ReturnIter = m_List.erase(iter);
			break;
		}
	}

	m_pRoom[nIndex].AddClient(pMyClient);

	return ReturnIter;
}

void CClientLobby::AllRefresh()
{
    for (auto iter : m_List)
        Refresh(iter);

    /*
	auto iter = m_List.begin();

	while (iter != m_List.end())
	{
		Refresh((*iter));
		++iter;
	}*/
}