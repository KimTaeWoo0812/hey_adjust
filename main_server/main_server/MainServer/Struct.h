#ifndef __STRUCT_H__
#define __STRUCT_H__

class CClientBase;

namespace Interpret
{
	typedef struct InterpretData
	{
		CClientBase*		pMyClient;
		CClientBase*		pYourClient;
		char*				pMessage;
	}INTERPRET_DATA;
}


namespace Client
{
	typedef struct tagClientInfo
	{
		char		szNickName[SOCKET_MSG_ONE];
		int			nPoint;
		int			nRank;


		void Initialize()
		{
			ZeroMemory(szNickName, sizeof(szNickName));
		}

		tagClientInfo() {}
		tagClientInfo(char* szNickName, const int& nPoint, const int& nRank)
			: nPoint(nPoint), nRank(nRank)
		{
			Initialize();
			memcpy(this->szNickName, szNickName, strlen(szNickName));
		}

	}CLIENT_INFO;
}

namespace Client_Position
{
	typedef struct tagRoomData
	{
		char		szRoomName[SOCKET_MSG_ONE_ARRAY_SIZE];
		int			nRoomNumber;
		int			nMaxClientInRoom;
		int			nCurrentClientNumber;
		int			nPickTopic;
		bool		bStart;

		tagRoomData()
		{
			ZeroMemory(szRoomName, SOCKET_MSG_ONE_ARRAY_SIZE);
			nRoomNumber = -1;
			nMaxClientInRoom = -1;
			nPickTopic = -1;
			nCurrentClientNumber = 0;
			bStart = false;
		}

	}ROOM_DATA;

	typedef struct tagRoomClient
	{
		CClientBase* pClient;
		bool		 bReady;

		tagRoomClient()
			: pClient(nullptr), bReady(false){}

		tagRoomClient(CClientBase* pClient)
			: pClient(pClient), bReady(false) {}

        tagRoomClient(CClientBase* pClient, bool bReady)
            : pClient(pClient), bReady(bReady) {}

	}ROOM_CLIENT;
}

namespace Game
{
	typedef struct tagGameInfo
	{
		CClientBase*	pClient;
		int				nRightCount;

		tagGameInfo()
            : pClient(nullptr), nRightCount(0) {}
		tagGameInfo(CClientBase* pClient)
			: pClient(pClient), nRightCount(0) {}
	}GAME_CLIENT_INFO;
}
#endif