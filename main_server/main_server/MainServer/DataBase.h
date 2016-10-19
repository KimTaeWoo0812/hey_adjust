#pragma once

#include "stdafx.h"
#include <mysql.h>
#include <my_global.h>

#define HOST "localhost"
#define USER "root"
#define PASSWORD "root"
#define DB_NAME "game"
#define DB_PORT 3306

class CDataBase
{
    DECLARE_SINGLETON(CDataBase)

private:
    MYSQL_RES *sql_result;
    MYSQL_ROW sql_row;
    MYSQL *connection, conn;
    std::mutex  m_hMutex;

private:
    int  CreateRandomNumber(int nStdNumber);

public:
    void  Initialize();
    char* getPassword(char* szID);
    char* getNickName(char* szID);
    char* getQuestionAnswer(int szRandomNum, char *questionName);
    int	  setUserSignUp(char* szID, char* szPW, char* szNick);
    int   getPoint(char* szID);
    int   getRank(char* szID);
    char* GetAnswer(char* szQuestionName, int nIndex);
    void  exitMysql();
    void  UpdateRankAndPoint(char* szID, int nPoint);

public:
    CDataBase();
    ~CDataBase();
};