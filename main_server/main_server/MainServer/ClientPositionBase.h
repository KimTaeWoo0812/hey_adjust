#pragma once

#include "IClientPosition.h"
#include "ClientBase.h"

class CClientPositionBase :
	public IClientPosition
{
protected :
	std::list<CClientBase*>					m_List;

protected:
	bool CheckList()						override;

public:
	bool AddClient(CClientBase* pClient)	override;

public:
	CClientPositionBase();
	~CClientPositionBase();
};

