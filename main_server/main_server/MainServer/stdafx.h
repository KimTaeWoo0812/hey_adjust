#ifndef __STD_AFX_H__
#define __STD_AFX_H__

#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <thread>
#include <mutex>
#include <stack>
#include <queue>
#include <list>
#include <vector>
#include <string>
#include <tchar.h>
#include <random>
#include "Include.h"

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable : 4996)

#include <crtdbg.h>

#ifndef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)
#endif

#endif