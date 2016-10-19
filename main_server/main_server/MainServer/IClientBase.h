#pragma once
#include "IBase.h"
class IClientBase :
	public IBase
{
private :
	virtual int Initialize() final;

public :
	virtual int Initialize(const SOCKET& pClient)		PURE;
	virtual int Progress()						        PURE;
	virtual int Render()						        PURE;
	virtual int Release()						        PURE;

public:
	IClientBase();
	virtual ~IClientBase();
};

