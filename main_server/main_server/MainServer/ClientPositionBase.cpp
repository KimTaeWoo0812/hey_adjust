#include "ClientPositionBase.h"


CClientPositionBase::CClientPositionBase()
{
}


CClientPositionBase::~CClientPositionBase()
{
}

bool CClientPositionBase::AddClient(CClientBase* pClient)
{
	m_List.push_back(pClient);

	return true;
}

bool CClientPositionBase::CheckList()
{
	LIST_ITER(CClientBase*) iter = m_List.begin();
	LIST_ITER(CClientBase*) delIter;
	bool isDelete = false;
	bool isChange = false;

	while (iter != m_List.end())
	{
		if ((*iter)->isExit())
		{
			delIter = iter;
			isDelete = TRUE;
			isChange = TRUE;
		}

		++iter;

		if (isDelete)
		{
			isDelete = FALSE;
			m_List.erase(delIter);
		}
	}

	return isChange;
}