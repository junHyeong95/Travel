
#ifndef TRAVELDB_H_
#define TRAVELDB_H_

#include<iostream>
#include <cstring>
#include<cstdlib>
#include <unistd.h>
#include "/usr/include/mysql/mysql.h"
using namespace std;

class Database
{
public:
    MYSQL Conn;
    MYSQL* ConnPtr = NULL;
    MYSQL_RES* Result;
    MYSQL_ROW Row;
    int stat;
    Database()
    {
        mysql_init(&Conn);
        mysql_options(&Conn, MYSQL_SET_CHARSET_NAME, "utf8");
        ConnPtr = mysql_real_connect(&Conn, "10.10.20.50", "admin", "1234", "TravelAgency", 3306, (char*)NULL, 0);

        if(ConnPtr == NULL)
        {
            fprintf(stderr, "mysql connection error : %s\n", mysql_error(&Conn));
            return;
        }
        else
        {
            cout<<"디비접속 완료..."<<endl;
        }
    }
};

class Travel : public Database
{
private:
    string choice;
    char select[100];
    char select1[100];
    char select2[100];
    int num1, num2, num3;
    int out;
public:
    void Members(char id[100])
    {
        system("clear");
        char query[1024];
        sprintf(query,"select * from Member where ID= '%s' and Status = 0", id);
        stat = mysql_query(ConnPtr, query);
        if(stat != 0)
        {
            fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
            return;
        }
        else
        {
            Result = mysql_store_result(ConnPtr);
            if((Row=mysql_fetch_row(Result))!=NULL)
            {
                if(!strcmp(Row[0],id))
                {
                    cout<<"|멤버등급 : "<<Row[3]<<"| 이름 : "<<Row[2]<<"|"<<endl;
                    cout<<"|     ID : "<<Row[0]<<"|   PW : "<<Row[1]<<"|"<<endl;
                    while(1)
                    {
                        cout<<"회원정보 수정을 입력해주세요.\n수정하실게 없다면 '돌아가기'를 입력해주세요\n>>";
                        getline(cin, choice);
                        if(choice.compare("수정")==0||choice.compare("돌아가기")==0)
                        {
                            if(choice == "수정")
                            {
                                cout<<"멤버등급 : "<<Row[3]<<" 이름 : "<<Row[2]<<endl;
                                cout<<"수정하고자 하는 부분을 입력해주세요.\n>>";
                                getline(cin, choice);
                                if(choice.compare("멤버등급")==0||choice.compare("이름")==0)
                                {
                                    if(choice=="멤버등급")
                                    {
                                        cin.ignore(10, '\n');
                                        char query[1024];
                                        cout<<"멤버등급을 입력해주세요. 1.VIP 2.nomal\n>>";
                                        cin.getline(select, 100, '\n');
                                        sprintf(query,"UPDATE Member SET Membership = '%s' where ID ='%s'",select, Row[0]);
                                        stat = mysql_query(ConnPtr, query);
                                        if(stat != 0)
                                        {
                                            fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                            return;
                                        }
                                        return;
                                    }
                                    else if(choice=="이름")
                                    {
                                        cin.ignore(10, '\n');
                                        char query[1024];
                                        cout<<"수정할 이름을 입력해주세요.\n>>";
                                        cin.getline(select, 100, '\n');
                                        sprintf(query,"UPDATE Member SET name = '%s' where ID ='%s'",select, Row[0]);
                                        stat = mysql_query(ConnPtr, query);
                                        if(stat != 0)
                                        {
                                            fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                            return;
                                        }
                                        return;
                                    }
                                }
                                else
                                {
                                    throw choice;
                                }
                            }
                            else if(choice=="돌아가기")
                            {
                                mysql_free_result(Result);
                                break;
                            }
                        }
                        else
                        {
                            throw choice;
                        }
                    }
                }
            }
            else
            {
                cout<<"없는 아이디 입니다."<<endl;
                sleep(3);
            }
        }
    }
    void deleteMemberList()
    {
        system("clear");
        while(1)
        {
            char query[1024];
            sprintf(query,"select * from Member where Status = 1");
            stat = mysql_query(ConnPtr, query);
            
            if(stat != 0)
            {
                fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                return;
            }

            Result = mysql_store_result(ConnPtr);
            cout<<"   탈퇴 회원 정보   "<<endl;
            while((Row=mysql_fetch_row(Result))!=NULL)
            {
                cout<<"아이디      : "<<Row[0]<<endl;
                cout<<"회원 이름   : "<<Row[2]<<endl<<endl;
            }
            cout<<"나가실 경우 '나가기'를 입력해주세요..\n>>";
            cin>>choice;
            if(choice.compare("나가기")==0)
            {
                if(choice=="나가기")
                {
                    mysql_free_result(Result);
                    break;
                }
            }
            else
            {
                throw choice;
            }
        }
    }
    void TravelList(char list[100])
    {
        system("clear");
        while(1)
        {
            char query[1024];
            sprintf(query,"select * from Tlist where name= '%s'", list);
            stat = mysql_query(ConnPtr, query);

            if(stat != 0)
            {
                fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                return;
            }

            Result = mysql_store_result(ConnPtr);
            while((Row=mysql_fetch_row(Result))!=NULL)
            {
                cout<<"여행지 : "<<Row[0]<<" | "<<Row[1]<<endl;
                cout<<"주소 : "<<Row[2]<<endl;
                cout<<"전화번호 : "<<Row[3]<<endl;
                cout<<"예약 : "<<Row[4]<<endl;

            }
            cout<<"나가실 경우 '나가기'를 입력해주세요..\n>>";
            cin>>choice;
            if(choice.compare("나가기")==0)
            {
                if(choice=="나가기")
                {
                    mysql_free_result(Result);
                    break;
                }
            }
            else
            {
                throw choice;
            }
        }
    }
    void TravelReservation(char list[100])
    {
        system("clear");
        char query[1024];
        sprintf(query,"select * from Tlist where name= '%s'", list);
        stat = mysql_query(ConnPtr, query);

        if(stat != 0)
        {
            fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
            return;
        }
        else
        {
            Result = mysql_store_result(ConnPtr);
            if((Row=mysql_fetch_row(Result))!=NULL)
            {
                cout<<"여행지 : "<<Row[0]<<" | "<<Row[1]<<endl;
                cout<<"주소 : "<<Row[2]<<endl;
                cout<<"전화번호 : "<<Row[3]<<endl;
                cout<<"예약 : "<<Row[4]<<endl;

                cout<<"예약하시거나 예약내역을 변경 하시겠습니까?\n 네 or 아니오 혹은 변경\n>>";
                cin>>choice;
                if(choice.compare("네")==0||choice.compare("아니오")==0||choice.compare("변경")==0)
                {
                    if(choice=="네")
                    {
                        cin.ignore(10, '\n');
                        char query[1024];
                        cout<<"예약자 아이디를 입력해주세요.\n>>";
                        cin.getline(select, 100, '\n');
                        cout<<"인원수를 입력해주세요.\n>>";
                        cin.getline(select1, 100, '\n');

                        sprintf(query,"select * from Member where ID= '%s' and Status = 0", select);
                        stat = mysql_query(ConnPtr, query);
                        if(stat != 0)
                        {
                           fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                           return;
                        }
                        else
                        {
                            Result = mysql_store_result(ConnPtr);
                            if((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                if(!strcmp(Row[0],select))
                                {
                                    sprintf(query,"UPDATE Tlist SET reservation = '%s', people = '%s' where name ='%s'",select, select1, list);
                                    stat = mysql_query(ConnPtr, query);
                                    if(stat != 0)
                                    {
                                        fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                        return;
                                    }
                                }
                            }
                            else
                            {
                                cout<<"없는 아이디 입니다."<<endl;
                                sleep(3);
                            }
                        }
                        return;
                    }
                    else if(choice=="변경")
                    {
                        cin.ignore(10, '\n');
                        char query[1024];
                        cout<<"예약자 아이디를 입력해주세요.\n>>";
                        cin.getline(select, 100, '\n');
                        cout<<"인원수를 입력해주세요.\n>>";
                        cin>>num1;
                        cin.ignore(10, '\n');
                        cout<<"여행지 이름을 입력해주세요.\n>>";
                        cin.getline(select2, 100, '\n');

                        sprintf(query,"select * from Member where ID= '%s' and Status = 0", select);
                        stat = mysql_query(ConnPtr, query);
                        if(stat != 0)
                        {
                           fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                           return;
                        }
                        else
                        {
                            Result = mysql_store_result(ConnPtr);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                if(!strcmp(Row[0],select))
                                {
                                    sprintf(query,"UPDATE Tlist SET reservation = '%s', people = %d where name ='%s'",select, num1, select2);
                                    stat = mysql_query(ConnPtr, query);
                                    if(stat != 0)
                                    {
                                        fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                        return;
                                    }

                                    sprintf(query,"UPDATE Tlist SET reservation = '%s', people = %d where name ='%s'", "없음", 0, list);
                                    stat = mysql_query(ConnPtr, query);
                                    if(stat != 0)
                                    {
                                        fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                        return;
                                    }
                                }
                            }
                        }
                        return;
                    }
                    else if(choice=="아니오")
                    {
                        mysql_free_result(Result);
                        return;
                    }
                }
                else
                {
                    throw choice;
                }
            }
            else
            {
                cout<<"없는 여행지 입니다."<<endl;
                sleep(2);
            }
        }
    }
    void GoodHotel() //호텔 3곳 추천
    {
        system("clear");
        srand((unsigned int) time(NULL));
        num1 = rand() % 319+1;
        num2 = rand() % 319+1;
        num3 = rand() % 319+1;
        for(int i=0; i<1; i++)
        {
            while(1)
            {
                char query[1024];
                sprintf(query,"select * from Alist where num= '%d'", num1);
                stat = mysql_query(ConnPtr, query);

                Result = mysql_store_result(ConnPtr);
                while((Row=mysql_fetch_row(Result))!=NULL)
                {
                    cout<<"숙박업소 : "<<Row[1]<<endl;
                    cout<<"주소 : "<<Row[2]<<endl;
                    cout<<"전화번호 : "<<Row[3]<<endl;
                    cout<<"남은객실 : "<<Row[4]<<endl<<endl;
                }

                sprintf(query,"select * from Alist where num= '%d'", num2);
                stat = mysql_query(ConnPtr, query);

                Result = mysql_store_result(ConnPtr);
                while((Row=mysql_fetch_row(Result))!=NULL)
                {
                    cout<<"숙박업소 : "<<Row[1]<<endl;
                    cout<<"주소 : "<<Row[2]<<endl;
                    cout<<"전화번호 : "<<Row[3]<<endl;
                    cout<<"남은객실 : "<<Row[4]<<endl<<endl;;
                }

                sprintf(query,"select * from Alist where num= '%d'", num3);
                stat = mysql_query(ConnPtr, query);

                Result = mysql_store_result(ConnPtr);
                while((Row=mysql_fetch_row(Result))!=NULL)
                {
                    cout<<"숙박업소 : "<<Row[1]<<endl;
                    cout<<"주소 : "<<Row[2]<<endl;
                    cout<<"전화번호 : "<<Row[3]<<endl;
                    cout<<"남은객실 : "<<Row[4]<<endl<<endl;;
                }
                cout<<"나가실 경우 '나가기'를 입력해주세요..\n>>";
                cin>>choice;
                if(choice=="나가기")
                {
                    break;
                }
            }
        }
        mysql_free_result(Result);
    }
    void Hotels(char hotel[100], char id[100])
    {
        char query[1024];
        sprintf(query,"select * from Member where ID= '%s' and Status = 0", id);
        stat = mysql_query(ConnPtr, query);
        if(stat != 0)
        {
           fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
           return;
        }
        else
        {
            Result = mysql_store_result(ConnPtr);
            if((Row=mysql_fetch_row(Result))!=NULL)
            {
                if(!strcmp(Row[0],id))
                {
                    system("clear");
                    sprintf(query,"select * from Alist where name= '%s'", hotel);
                    stat = mysql_query(ConnPtr, query);

                    if(stat != 0)
                    {
                        fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                        return;
                    }

                    Result = mysql_store_result(ConnPtr);
                    while((Row=mysql_fetch_row(Result))!=NULL)
                    {
                        while(1)
                        {
                            cout<<"숙박업소 : "<<Row[1]<<endl;
                            cout<<"주소 : "<<Row[2]<<endl;
                            cout<<"전화번호 : "<<Row[3]<<endl;
                            cout<<"남은객실 : "<<Row[4]<<endl;
                            cout<<"예약 : "<<Row[5]<<endl;

                            cout<<"예약하시거나 예약내역을 변경 하시겠습니까?\n 예약 or 변경 혹은 안함을 입력해주세요..\n>>";
                            cin>>choice;
                            if(choice.compare("예약")==0 || choice.compare("변경")==0  || choice.compare("안함")==0)
                            {
                                if(choice=="예약")
                                {
                                    char query[1024];

                                    cout<<"인원수를 입력해주세요.\n>>";
                                    cin>>num1;

                                    sprintf(query,"UPDATE Alist SET reservation = '%s', people = %d where name ='%s'",id, num1, hotel);
                                    stat = mysql_query(ConnPtr, query);
                                    if(stat != 0)
                                    {
                                        fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                        sleep(10);
                                        return;
                                    }
                                    return;
                                }
                                else if(choice=="변경")
                                {
                                    cin.ignore(10, '\n');
                                    char query[1024];
                                    cout<<"변경을 원하는 호텔을 입력해주세요."<<endl<<">>";
                                    cin.getline(select, 100, '\n');
                                    cout<<"인원수를 입력해주세요."<<endl<<">>";
                                    cin>>num1;
                                    sprintf(query,"UPDATE Alist SET reservation = '%s', people = %d where name ='%s'",id, num1, select);
                                    stat = mysql_query(ConnPtr, query);
                                    if(stat != 0)
                                    {
                                        fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                        return;
                                    }

                                    sprintf(query,"UPDATE Alist SET reservation = '%s', people = %d where name ='%s'", "없음", 0, hotel);
                                    stat = mysql_query(ConnPtr, query);
                                    if(stat != 0)
                                    {
                                        fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                        return;
                                    }
                                    return;
                                }
                                else if(choice=="안함")
                                {
                                    mysql_free_result(Result);
                                    break;
                                }
                            }
                            else
                            {
                                throw choice;
                            }
                        }
                    }
                }
            }
            else
            {
                cout<<"없는 아이디 입니다."<<endl;
                sleep(3);
            }
        }
    }
    void Guide(char id[100])
    {
        char query[1024];
        sprintf(query,"select * from Member where ID= '%s' and Status = 0", id);
        stat = mysql_query(ConnPtr, query);

        if(stat != 0)
        {
           fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
           return;
        }
        else
        {
            Result = mysql_store_result(ConnPtr);
            if((Row=mysql_fetch_row(Result))!=NULL)
            {
                if(!strcmp(Row[0], id))
                {
                    system("clear");
                    while (1)
                    {
                        cout<<id<<" 회원님"<<endl;
                        cout<<"가이드 여부 조회는 '조회'을 입력해주세요 아니라면 '요청'을 입력해주세요."<<endl;
                        cout<<"나가실려면 '나가기'를 입력해주세요.."<<endl;
                        cout<<"예약취소는 '취소'를 입력해주세요..\n>>";
                        getline(cin, choice);

                        if(choice.compare("조회")==0 || choice.compare("요청")==0  || choice.compare("나가기")==0 || choice.compare("취소")==0 )
                        {
                            if(choice=="조회")
                            {
                                char query[1024];
                                sprintf(query,"select * from Guide where ID= '%s'", id);
                                stat = mysql_query(ConnPtr, query);
                                if(stat != 0)
                                {
                                    cout<<"가이드 요청 없음..."<<endl;
                                    sleep(10);
                                    continue;;
                                }
                                else
                                {
                                    Result = mysql_store_result(ConnPtr);
                                    while((Row=mysql_fetch_row(Result))!=NULL)
                                    {
                                        cout<<"가이드 요청 아이디 : "<<Row[0]<<endl;
                                        cout<<"가이드 요청 지역   : "<<Row[1]<<endl;
                                    }
                                }

                                cout<<"예약취소는 '취소'를 입력해주세요..\n";
                                cout<<"아니라면 '나가기'를 입력해주세요..\n>>";
                                getline(cin, choice);
                                if(choice.compare("나가기")==0 || choice.compare("취소")==0 )
                                {
                                    if(choice=="취소")
                                    {
                                        char query[1024];
                                        sprintf(query,"delete from Guide where ID = '%s' ", id);
                                        stat = mysql_query(ConnPtr, query);

                                        if(stat != 0)
                                        {
                                            fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                            sleep(10);
                                            return;
                                        }
                                    }
                                    else if(choice=="나가기")
                                    {
                                        mysql_free_result(Result);
                                        break;
                                    }
                                }
                                else
                                {
                                    throw choice;
                                }
                            }
                            else if(choice=="요청")
                            {
                                char query[1024];
                                cout<<"가이드 요청 지역\n>>";
                                cin.getline(select1, 100, '\n');
                                sprintf(query,"insert into Guide values('%s','%s')", id, select1);
                                stat = mysql_query(ConnPtr, query);

                                if(stat != 0)
                                {
                                    fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                                    return;
                                }
                                return;
                            }
                            else if(choice=="나가기")
                            {
                                mysql_free_result(Result);
                                break;
                            }
                        }
                        else
                        {
                            throw choice;
                        }
                    }
                }
            }
            else
            {
                cout<<"없는 아이디 입니다."<<endl;
                sleep(3);
            }
        }
    }
    void Memberlist()
    {
        system("clear");
        while(1)
        {
            char query[1024];
            sprintf(query,"select * from Member where Status = 0");
            stat = mysql_query(ConnPtr, query);

            if(stat != 0)
            {
                fprintf(stderr, "Mysql query error : %s\n", mysql_error(&Conn));
                return;
            }

            Result = mysql_store_result(ConnPtr);
            cout<<"   회원 정보   "<<endl;
            while((Row=mysql_fetch_row(Result))!=NULL)
            {
                cout<<"아이디      : "<<Row[0]<<endl;
                cout<<"회원 이름   : "<<Row[2]<<endl<<endl;
            }
            cout<<"나가실 경우 '나가기'를 입력해주세요..\n>>";
            cin>>choice;
            if(choice.compare("나가기")==0)
            {
                if(choice=="나가기")
                {
                    mysql_free_result(Result);
                    break;
                }
            }
            else
            {
                throw choice;
            }
        }
    }
};
#endif