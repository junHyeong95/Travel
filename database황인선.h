
#ifndef __DATABASE_H_
#define __DATABASE_H_
#include "/usr/include/mysql/mysql.h"
#include <iostream>
#include <cstring>
#include <cstdlib> // sys clear
using namespace std;

class Database // 데이터베이스
{
public:
    MYSQL* ConnPtr = NULL, Conn;
    MYSQL_RES* Result;
    MYSQL_ROW Row;
    int Stat;
    Database() // 생성자
    {
        mysql_init(&Conn);
        mysql_options(&Conn, MYSQL_SET_CHARSET_NAME, "utf8"); // 한글이 깨지는 현상 방지
        ConnPtr = mysql_real_connect(&Conn, "10.10.20.50", "nomal", "1234", "TravelAgency", 3306, NULL, 0);

        if(ConnPtr == NULL)
        {
            fprintf(stderr, "mysql connection error : %s\n", mysql_error(&Conn));
            return;
        }
        else
        {
            puts("Database connect!");
            system("clear");
        }
    }
};
#endif