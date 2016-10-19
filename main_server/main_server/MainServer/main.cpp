#include "ServerInterface.h"

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CServerInterface mainServer;

	mainServer.Initialize();

	while (mainServer.Progress());

	mainServer.Release();
}