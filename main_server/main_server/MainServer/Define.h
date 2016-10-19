#ifndef __DEFINE_H____
#define __DEFINE_H____

#define DECLARE_SINGLETON(type)					\
public:											\
	static type** GetInstance(void)				\
	{											\
		static type*	pInstance = new type;	\
		if(pInstance == nullptr)				\
			pInstance = new type;				\
		return &pInstance;						\
	}											\
	void DestroyInstance(void)					\
	{											\
		type** ppInstance = GetInstance();		\
		if(ppInstance != nullptr)				\
		{										\
			delete *ppInstance;					\
			*ppInstance = nullptr;				\
		}										\
	}


#define GET_SINGLE(type)	(*type::GetInstance())
#define ERR_MSG(Message) MessageBox(g_hWnd, Message, L"System Message", NULL);
#define CLIENT_MAX_COUNT 100

#define SOCKET_MSG_BUF 128
#define SOCKET_MSG_ONE 20
#define SOCKET_MSG_ONE_ARRAY_SIZE (SOCKET_MSG_ONE + 1)

#define SOCKET_CREATE_MSG_LOOP(n) \
for (int i = (SOCKET_MSG_ONE * n) + 1; i <= (SOCKET_MSG_ONE * (n + 1)); ++i)

#define SOCKET_CREATE_MSG_WHILE(n) \
for (int i = (SOCKET_MSG_ONE * n) + 1;; ++i)

#define SOCKET_MATCH_INDEX(n) \
	((SOCKET_MSG_ONE * n) + 1)

#define LIST_ITER(T) std::list<T>::iterator

#define ROOM_COUNT		20

#define CLINET_MAX_IN_ROOM	8
#define UNICODE_GARBAGE 65533

#endif