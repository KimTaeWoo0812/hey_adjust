#include "ClientAdministrator.h"
#include "ClientBase.h"
#include "MessageBox.h"
#include "ClientLogin.h"

CClientAdministrator::CClientAdministrator()
{
}


CClientAdministrator::~CClientAdministrator()
{
}

int CClientAdministrator::Initialize()
{
	InitValue();
	InitClass();
	return 0;
}

int CClientAdministrator::Progress()
{
	if (m_pQuitBox->hasMessage())
		ClientRelease();

	m_pLogin->Progress();

	return 0;
}

int CClientAdministrator::Render()
{
	return 0;
}

int CClientAdministrator::Release()
{
	ReleaseClass();
	return 0;
}

void CClientAdministrator::CreateClient(const SOCKET& pClient)
{
	for (int i = 0; i < CLIENT_MAX_COUNT; ++i)
	{
		if (m_pClientBase[i].isExit())
		{
			m_pClientBase[i].SetQuitBox(m_pQuitBox);
			m_pClientBase[i].SetIndex(i);
			m_pClientBase[i].Initialize(pClient);

			m_pLogin->AddClient(&m_pClientBase[i]);

			++m_nClientIndex;

			if (m_nClientIndex >= CLIENT_MAX_COUNT)
				m_nClientIndex = CLIENT_MAX_COUNT;

			break;
		}
	}
}

void CClientAdministrator::InitClass()
{
	m_pQuitBox = new CMessageBox<int>();
	m_pQuitBox->Progress();

	m_pLogin = new CClientLogin();
	m_pLogin->Initialize();
}

void CClientAdministrator::ReleaseClass()
{
	m_pLogin->Release();
	Safe_Delete(m_pLogin);

	for (int i = 0; i < m_nClientIndex; ++i)
	{
		if (!m_pClientBase[i].isExit())
			m_pClientBase[i].Release();
	}

	Safe_Delete(m_pQuitBox);
}

void CClientAdministrator::InitValue()
{
	m_nClientIndex = 0;
}

void CClientAdministrator::ClientRelease()
{
	while (m_pQuitBox->hasMessage())
	{
		int nIndex = m_pQuitBox->getMessage();
		printf("Release Client Index : %d\n", nIndex);
			//m_pClientBase[m_pQuitBox->getMessage()].Release();
		m_pClientBase[nIndex].Release();
	}
}

const int CClientAdministrator::GetAccessClientCount()
{
	int nClientCount = 0;
	for (int i = 0; i < m_nClientIndex; ++i)
	{
		if (!m_pClientBase[i].isExit())
			++nClientCount;
	}

	return nClientCount;
}

const int CClientAdministrator::GetLoginClientCount()
{
	return m_pLogin->GetLoginClientCount();
}

const int CClientAdministrator::GetLobbyClientCount()
{
	return m_pLogin->GetLobbyClientCount();
}

const int CClientAdministrator::GetRoomClientCount()
{
	return m_pLogin->GetRoomClientCount();
}

const int CClientAdministrator::GetRoomCount()
{
	return m_pLogin->GetRoomCount();
}