#include <iostream>
#include <WinSock2.h>
#include "DataBase.h"

CDataBase::CDataBase()
{
}


CDataBase::~CDataBase()
{
    exitMysql();
}

char* CDataBase::getPassword(char* szID)
{
    std::lock_guard<std::mutex> lock(m_hMutex);

    char buffer[200] = { 0, };

    strcpy(buffer, "select * from users WHERE user_id = '");
    strcat(buffer, szID);
    strcat(buffer, "'");

    mysql_query(connection, buffer);
    if (!connection)
    {
        std::cout << mysql_error(&conn) << std::endl;
        return "-1";
    }
    sql_result = mysql_store_result(connection);

    while (sql_row = mysql_fetch_row(sql_result))
    {
        char* szPassword = new char[SOCKET_MSG_BUF];
        ZeroMemory(szPassword, SOCKET_MSG_BUF);

        strcpy(szPassword, sql_row[1]);

        return szPassword;
    }

    return nullptr;
}

char* CDataBase::getNickName(char* szID)
{
    std::lock_guard<std::mutex> lock(m_hMutex);

    char buffer[200] = { 0, };

    strcpy(buffer, "select * from users WHERE user_id = '");
    strcat(buffer, szID);
    strcat(buffer, "'");

    mysql_query(connection, buffer);
    sql_result = mysql_store_result(connection);
    char* szNickName = new char[SOCKET_MSG_ONE_ARRAY_SIZE];
    ZeroMemory(szNickName, sizeof(szNickName));
    while (sql_row = mysql_fetch_row(sql_result))
    {
        strcpy(szNickName, sql_row[2]);
    }
    return szNickName;
}

int CDataBase::setUserSignUp(char* szID, char* szPW, char* szNick)
{
    std::lock_guard<std::mutex> lock(m_hMutex);

    char buffer[1000] = { 0, };

    strcpy(buffer, "INSERT INTO users VALUES ('");
    strcat(buffer, szID);
    strcat(buffer, "', '");
    strcat(buffer, szPW);
    strcat(buffer, "', '");
    strcat(buffer, szNick);
    strcat(buffer, "', ");
    strcat(buffer, "0, 0)");

    mysql_query(connection, buffer);
  
    if (!connection)
    {
        std::cout << mysql_error(&conn) << std::endl;
        return 0;
    }

    return 1;
}

int CDataBase::getPoint(char* szID)
{
    std::lock_guard<std::mutex> lock(m_hMutex);

    char buffer[200] = { 0, };
    int userPoint;
    strcpy(buffer, "select * from users WHERE user_id = '");
    strcat(buffer, szID);
    strcat(buffer, "'");

    mysql_query(connection, buffer);

    sql_result = mysql_store_result(connection);

    while (sql_row = mysql_fetch_row(sql_result))
    {
        userPoint = atoi(sql_row[3]);
    }

    return userPoint;
}

int CDataBase::getRank(char* szID)
{
    std::lock_guard<std::mutex> lock(m_hMutex);

    char buffer[200] = { 0, };
    int userRank;
    strcpy(buffer, "select * from users WHERE user_id = '");
    strcat(buffer, szID);
    strcat(buffer, "'");

    mysql_query(connection, buffer);
    sql_result = mysql_store_result(connection);

    while (sql_row = mysql_fetch_row(sql_result))
    {
        userRank = atoi(sql_row[2]);
    }

    return userRank;
}

char* CDataBase::GetAnswer(char *questionName, int szRandomNum)
{
    std::lock_guard<std::mutex> lock(m_hMutex);

    char buffer[200] = { 0, };
 
    strcpy(buffer, "select * from ");
    int i = 0;
    while (questionName[i] != '_')
    {
        i++;
    }
    questionName[i] = NULL;
    strcat(buffer, questionName);
    strcat(buffer, " WHERE ");
    strcat(buffer, questionName);
    strcat(buffer, "_key");
    strcat(buffer, " = ");

    char tmp[3] = { 0, };

    strcat(buffer, itoa(szRandomNum, tmp, 10));
   
    mysql_query(connection, buffer);

    if (!connection)
    {
        std::cout << mysql_error(&conn) << std::endl;
        return "-1";
    }
    sql_result = mysql_store_result(connection);
    while (sql_row = mysql_fetch_row(sql_result))
    {
        return sql_row[1];
    }

    return nullptr;
}

void CDataBase::Initialize()
{
    std::lock_guard<std::mutex> lock(m_hMutex);

    mysql_init(&conn);

    connection = mysql_real_connect(&conn, HOST, USER, PASSWORD, DB_NAME, DB_PORT, (char*)NULL, 0);

    if (!connection)
    {
        std::cout << mysql_error(&conn) << std::endl;
        return;
    }
    else
    {
        std::cout << "DB 연결 성공" << std::endl;
        ////한글 설정
        mysql_query(connection, "set session character_set_connection = euckr;");
        mysql_query(connection, "set session character_set_results = euckr;");
        mysql_query(connection, "set session character_set_client = euckr;");
        mysql_query(connection, "USE game");
    }
}

void  CDataBase::exitMysql()
{
    mysql_close(connection);
    std::cout << "DB 연결 종료" << std::endl;
}

void CDataBase::UpdateRankAndPoint(char* szID, int nPoint)
{
    std::lock_guard<std::mutex> lock(m_hMutex);

    char buffer[200] = { 0, };
    strcpy(buffer, "update users set user_point = ");
    char tmp[3] = { 0, };
    strcat(buffer, itoa(nPoint, tmp, 10));
    strcat(buffer, " where user_id = '");
    strcat(buffer, szID);
    strcat(buffer, "'");

    mysql_query(connection, "USE game");
    mysql_query(connection, buffer);
}