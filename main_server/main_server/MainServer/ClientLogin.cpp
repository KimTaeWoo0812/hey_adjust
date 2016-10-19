#include "ClientLogin.h"
#include "ClientBase.h"
#include "ClientLobby.h"
#include "LoginInterpret.h"

CClientLogin::CClientLogin()
{
}


CClientLogin::~CClientLogin()
{
}

int CClientLogin::Initialize()
{
	m_pLobby = new CClientLobby();
	m_pLobby->Initialize();

	return 0;
}

int CClientLogin::Progress()
{
	CheckList();

	Interpreter();

	m_pLobby->Progress();

	return 0;
}

int CClientLogin::Render()
{
	return 0;
}

int CClientLogin::Release()
{
	m_List.clear();
	m_pLobby->Release();
	Safe_Delete(m_pLobby);

	return 0;
}

bool CClientLogin::AddClient(CClientBase* pClient)
{
	CClientPositionBase::AddClient(pClient);

	return true;
}

bool CClientLogin::CheckList()
{
	return CClientPositionBase::CheckList();
}

void CClientLogin::Interpreter()
{
	auto iter = m_List.begin();
	auto delIter = iter;

	while (iter != m_List.end())
	{
		if ((*iter)->hasMessage())
		{
			const char* message = (*iter)->getMessage();
			printf("%s\n", message);

			if (m_Interpret.isCreate((*iter), message));

			else if (m_Interpret.isLogin(message))
			{
				m_Interpret.SuccessMessage((*iter));
				delIter = iter;
				++iter;
				m_pLobby->AddClient((*delIter));
				m_List.erase(delIter);
			}

			else m_Interpret.FailedMessage((*iter));


			Safe_Delete(message);
		}

		else ++iter;
	}
}

int CClientLogin::GetLoginClientCount()
{
	return m_List.size();
}

int CClientLogin::GetLobbyClientCount()
{
	return m_pLobby->GetLobbyClientCount();
}

int CClientLogin::GetRoomClientCount()
{
	return m_pLobby->GetRoomClientCount();
}

int CClientLogin::GetRoomCount()
{
	return m_pLobby->GetRoomCount();
}