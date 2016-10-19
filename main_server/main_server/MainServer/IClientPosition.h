#pragma once

#include "IBase.h"

class CClientBase;

class IClientPosition :
	public IBase
{
protected :
	virtual bool CheckList()						PURE;

public :
	virtual bool AddClient(CClientBase* pClient)	PURE;

public:
	IClientPosition();
	virtual ~IClientPosition();
};

