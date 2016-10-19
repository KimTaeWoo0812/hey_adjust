#pragma once

#include "IBase.h"
#include "DataBase.h"
#include "ClientBase.h"
#include "InterpretHelper.h"

//데이터 소켓 방식
//0 ~ 15 16 ~ 30 .....
//#이 끝으로 인식하게 된다.

class CLoginInterpret
{
private :
	char m_szMessage[Client_Position::LOGIN_MSG_COUNT][SOCKET_MSG_ONE_ARRAY_SIZE];
	char m_szID[SOCKET_MSG_ONE_ARRAY_SIZE];
	Client::CLIENT_INFO	m_tClientInfo;

public :
	bool isCreate(CClientBase* pMyClient, const char* szMessage);
	void SuccessCreate(CClientBase* pMyClient);
	void FailedCreate(CClientBase* pMyClient);

public:
	bool isLogin(const char* pMessage);
	char* CreateFirstMessage();
	char* CreateTwiceMessage();
	void SuccessMessage(CClientBase* pMyClient);
	void FailedMessage(CClientBase* pMyClient);

public :
	CLoginInterpret();

};