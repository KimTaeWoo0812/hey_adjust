#pragma once

#include "IBase.h"

class CClientRoom;

class IGameServer :
	public IBase
{
public :
	virtual bool AddClient(CClientBase* pClientBase, const int& nIndex)	PURE;

public:
	IGameServer();
	virtual ~IGameServer();
};

