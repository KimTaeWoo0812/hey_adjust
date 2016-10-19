#include "GameTable.h"
#include "InterpretHelper.h"

CGameTable::CGameTable()
{
	for (int i = 0; i < Game::GAME_TOPIC_COUNT; ++i)
		ZeroMemory(m_szAnimalTalbe[i], sizeof(m_szAnimalTalbe[i]));

	strcpy(m_szAnimalTalbe[Game::GAME_TOPIC_ANIMAL], "animal_key");
	strcpy(m_szAnimalTalbe[Game::GAME_TOPIC_COMIC], "comic_key");
	strcpy(m_szAnimalTalbe[Game::GAME_TOPIC_MOVIE], "movie_key");
	strcpy(m_szAnimalTalbe[Game::GAME_TOPIC_SPORTS], "sports_key");
}

CGameTable::~CGameTable()
{
}

char* CGameTable::GetTopic(int nChoiceNumber)
{
	char* szChoiceTopic = HInterpretHelper::MakeCharArray(SOCKET_MSG_ONE_ARRAY_SIZE);

	strcpy(szChoiceTopic, m_szAnimalTalbe[Game::Game_Topic(nChoiceNumber)]);

	return szChoiceTopic;
}