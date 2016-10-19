#pragma once

#include "stdafx.h"

class CGameTable
{
private :
	char		m_szAnimalTalbe[Game::GAME_TOPIC_COUNT][SOCKET_MSG_ONE_ARRAY_SIZE];

public :
	char* GetTopic(int nChoiceNumber);

public:
	CGameTable();
	~CGameTable();
};