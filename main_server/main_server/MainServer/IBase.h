#pragma once

#include "stdafx.h"

class IBase
{
public :
	virtual int Initialize()						PURE;
	virtual int Progress()							PURE;
	virtual int Render()							PURE;
	virtual int Release()							PURE;

public:
	IBase(void);
	virtual ~IBase(void);
};