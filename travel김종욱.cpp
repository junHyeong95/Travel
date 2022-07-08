#ifndef DATABASEH
#define DATABASEH
#include<iostream>
#include<string.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include "/usr/include/mysql/mysql.h"
using namespace std;

class Database
{
protected:
    MYSQL *connection = NULL, conn;
    MYSQL_RES *Result;
    MYSQL_ROW Row;


public:
    char query[1024];
    Database()
    {
        mysql_init(&conn);
        mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "utf8"); //
        connection = mysql_real_connect(&conn, "10.10.20.50", "VIP", "1234", "TravelAgency", 3306, (char*)NULL, 0); 
        if(connection==NULL)
        {
            fprintf(stderr, "Failed to connect to databases: Error: %s\n",
            mysql_error(&conn));
            return;
        }
        else
        {
            puts("Database connect!");
        }
    }
};

class VIP : public Database
{
    private:
        char PW[20]={};
    public:
        char name[20]={};
        char ID[20]={};
        void sandquery(char *a)
        {
            if(mysql_query(connection,a)!=0)
            {
                cout<<"error"<<endl;
                exit(1);
            }
        }
        void signup()
        {
            int stat;
            while(1)
            {
                int stat=0;
                cout<<"회원가입을 위해 사용하실 아이디를 입력해주세요.:";
                cin>>ID;
                sprintf(query, "select count(*) from Member where ID = '%s'", ID);
                mysql_query(connection, query);
                Result = mysql_store_result(connection);
                Row = mysql_fetch_row(Result);
                if(*Row[0] != '0')
                {
                    cout<<"사용중인 아이디입니다. 다시입력해주세요."<<endl;
                }
                else
                    break;
            }
            cout<<"회원님이 사용하실 비밀번호를 입력하세요.:";
            cin>>PW;
            cout<<"회원님의 성함을 입력하세요.:";
            cin>>name;
            sprintf(query, "insert into Member values('%s', '%s', '%s', 'VIP', '0')", ID, PW, name);
            sandquery(query);

            cout<<"회원가입이 완료되었습니다."<<endl;
        }

        void loginpage()
        {

            char a[20]={};
            char b[20]={};
            int stat;
            while(1)
            {
                cout<<"회원가입하려면 \"x\"를 입력해주세요.\n아이디를 입력해주세요.:";
                cin>>a;
                if(*a=='x')
                {
                    signup();
                }
                else
                {
                    cout<<"비밀번호를 입력해주세요.:";
                    cin>>b;
                }
                sprintf(query, "select count(*) from Member where ID = '%s' and PW = '%s' and Status = '0' and Membership = 'VIP'", a, b);
                strcpy(ID, a);
                stat=mysql_query(connection, query);
                Result= mysql_store_result(connection);
                Row = mysql_fetch_row(Result);
                if(*Row[0] !='0')
                {
                    cout<<a<<"님 접속되었습니다."<<endl;
                    break;
                }
                else
                    cout<<"다시입력해주세요."<<endl;
            }

        }

        void choice()
        {
            char input[]={};
            char ch;
            char wate[]={};
            while(1)
            {
                sleep(1);
                cout<<"1. 관광지 내역 조회\n2. 숙박\n3. 예약 관리\n4. 가이드\n5. 회원 탈퇴\n선택:";
                cin>>ch;
                if(ch=='1')
                {
                    while(1)
                    {
                        cout<<"1. 지역 이름으로 조회\n2. 이름으로 조회\n3. 이전으로 돌아가기"<<endl;
                        cin>>ch;
                        if(ch=='1')
                        {
                            cout<<"관광지 지역을 입력해주세요.:";
                            cin>>input;
                            sprintf(query, "select * from Tlist where area = '%s'", input);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                cout<<"지역:"<<Row[0]<<"\n여행지:"<<Row[1]<<endl;
                                cout<<"주소:"<<Row[2]<<endl;
                                cout<<"전화번호:"<<Row[3]<<endl;
                            }
                            cout<<"관광지 이름을 입력해주세요.:";
                            cin>>input;
                            sprintf(query, "select * from Tlist where name = '%s'", input);
                            strcpy(name, input);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while(1)
                            {
                                cout<<"예약 하시겠습니까?(Y/N):";
                                cin>>ch;
                                if(ch=='Y'||ch=='y')
                                {
                                    cout<<"몇명을 예약하시겠습니까?:";
                                    cin>>wate;
                                    sprintf(query, "update  Tlist set reservation  = '%s', people = '%s' where name = '%s'", ID, wate, name);
                                    cout<<query<<endl;
                                    sandquery(query);
                                    cout<<"예약되었습니다."<<endl;
                                    break;
                                }
                                else if(ch=='n'||ch=='N')
                                    break;
                                else
                                    cout<<"잘못입력해셨습니다."<<endl;
                            }
                            break;
                        }
                        else if(ch=='2')
                        {
                            cout<<"관광지 이름을 입력해주세요.:";
                            cin>>input;
                            sprintf(query, "select * from Tlist where name = '%s'", input);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                cout<<"지역:"<<Row[0]<<"\n여행지:"<<Row[1]<<endl;
                                cout<<"주소:"<<Row[2]<<endl;
                                cout<<"전화번호:"<<Row[3]<<endl;
                            }
                            strcpy(name, input);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while(1)
                            {
                                cout<<"예약 하시겠습니까?(Y/N):";
                                cin>>ch;
                                if(ch=='Y'||ch=='y')
                                {
                                    cout<<"몇명을 예약하시겠습니까?:";
                                    cin>>wate;
                                    sprintf(query, "update  Tlist set reservation  = '%s', people = '%s' where name = '%s'", ID, wate, name);
                                    cout<<query<<endl;
                                    sandquery(query);
                                    cout<<"예약되었습니다."<<endl;
                                    break;
                                }
                                else if(ch=='n'||ch=='N')
                                    break;
                                else
                                    cout<<"잘못입력해셨습니다."<<endl;
                            }
                            break;
                        }
                        else if(ch=='3')
                        {
                            cout<<"이전으로 돌아가기"<<endl;
                            break;
                        }
                    }
                }
                else if(ch=='2')
                {
                    while(1)
                    {
                        cout<<"1. 지역 이름으로 조회\n2. 이름으로 조회\n3. 이전으로 돌아가기"<<endl;
                        cin>>ch;
                        if(ch=='1')
                        {
                            cout<<"숙박업소 주소를 입력해주세요.:";
                            cin>>input;
                            sprintf(query, "select * from Alist where address = '%s'", input);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                cout<<"번호:"<<Row[0]<<"\n여행지:"<<Row[1]<<endl;
                                cout<<"주소:"<<Row[2]<<endl;
                                cout<<"전화번호:"<<Row[3]<<endl;
                            }
                            cout<<"숙소 이름을 입력해주세요.:";
                            cin>>input;
                            sprintf(query, "select * from Tlist where name = '%s'", input);
                            strcpy(name, input);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while(1)
                            {
                                cout<<"예약 하시겠습니까?(Y/N):";
                                cin>>ch;
                                if(ch=='Y'||ch=='y')
                                {
                                    cout<<"몇명을 예약하시겠습니까?:";
                                    cin>>wate;
                                    sprintf(query, "update  Alist set reservation  = '%s', people = '%s' where name = '%s'", ID, wate, name);
                                    cout<<query<<endl;
                                    sandquery(query);
                                    cout<<"예약되었습니다."<<endl;
                                    break;
                                }
                                else if(ch=='n'||ch=='N')
                                    break;
                                else
                                    cout<<"잘못입력해셨습니다."<<endl;
                            }
                            break;
                        }
                        else if(ch=='2')
                        {
                            cout<<"숙박업소 이름을 입력해주세요.:";
                            cin>>input;
                            sprintf(query, "select * from Alist where name = '%s'", input);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                cout<<"N0."<<Row[0]<<"\n여행지:"<<Row[1]<<endl;
                                cout<<"주소:"<<Row[2]<<endl;
                                cout<<"전화번호:"<<Row[3]<<endl;
                            }
                            strcpy(name, input);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while(1)
                            {
                                cout<<"예약 하시겠습니까?(Y/N):";
                                cin>>ch;
                                if(ch=='Y'||ch=='y')
                                {
                                    cout<<"몇명을 예약하시겠습니까?:";
                                    cin>>wate;
                                    sprintf(query, "update  Alist set reservation  = '%s', people = '%s' where name = '%s'", ID, wate, name);
                                    cout<<query<<endl;
                                    sandquery(query);
                                    cout<<"예약되었습니다."<<endl;
                                    break;
                                }
                                else if(ch=='n'||ch=='N')
                                    break;
                                else
                                    cout<<"잘못입력해셨습니다."<<endl;
                            }
                            cout<<"나가려면 아무키나 누르세요."<<endl;
                            cin>>wate;
                            break;
                        }
                        else if(ch=='3')
                        {
                            cout<<"이전으로 돌아가기"<<endl;
                            break;
                        }
                        else
                            cout<<"잘못 입력 하셨습니다. 다시 입력해주세요."<<endl;
                    }
                }
                else if(ch=='3')
                {
                    while(1)
                    {
                        cout<<"1. 관광지 예약 관리\n2. 숙박업소 예약 관리\nx. 이전으로 돌아가기\n입력:";
                        cin>>ch;
                        if(ch=='1')
                        {
                            sprintf(query, "select * from Tlist where reservation = '%s'", ID);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                cout<<"지역:"<<Row[0]<<"\n관광지:"<<Row[1]<<endl;
                                cout<<"주소:"<<Row[2]<<endl;
                                cout<<"전화번호:"<<Row[3]<<endl;
                            }
                            cout<<"1. 예약 취소\n2. 예약 변경\n3. 이전으로 돌아가기\n선택:";
                            cin>>ch;
                            if(ch=='1')
                            {
                                cout<<"취소할 관광지의 이름을 입력해주세요.:";
                                cin>>name;
                                sprintf(query, "update  Tlist set reservation = ' ', people = '0' where name = '%s'", name);
                                cout<<query<<endl;
                                sandquery(query);
                                cout<<"취소되었습니다."<<endl;
                                break;
                            }
                            else if(ch=='2')
                            {
                                cout<<"변경할 관광지의 이름을 입력해주세요.:";
                                cin>>name;
                                cout<<"변경할 인원수를 입력해주세요.:";
                                cin>>input;
                                sprintf(query, "update  Tlist set people = '%s' where name = '%s'", input, name);
                                cout<<query<<endl;
                                sandquery(query);
                                cout<<"변경되었습니다."<<endl;
                                break;
                            }
                            else if(ch=='3')
                            {
                                cout<<"이전으로 돌아갑니다."<<endl;
                                break;   
                            }
                            else
                                cout<<"다시입력해주세요."<<endl;
                        }
                        else if(ch=='2')
                        {
                           sprintf(query, "select * from Alist where reservation = '%s'", ID);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                cout<<"지역:"<<Row[0]<<"\n숙박시설:"<<Row[1]<<endl;
                                cout<<"주소:"<<Row[2]<<endl;
                                cout<<"전화번호:"<<Row[3]<<endl;
                            }
                            cout<<"1. 예약 취소\n2. 예약 변경\n3. 이전으로 돌아가기 \n선택:";
                            cin>>ch;
                            if(ch=='1')
                            {
                                cout<<"취소할 숙박시설 이름을 입력해주세요.:";
                                cin>>name;
                                sprintf(query, "update  Alist set reservation = ' ', people = '0' where name = '%s'", name);
                                cout<<query<<endl;
                                sandquery(query);
                                cout<<"취소되었습니다."<<endl;
                                break;
                            }
                            else if(ch=='2')
                            {
                                cout<<"변경할 숙박시설의 이름을 입력해주세요.:";
                                cin>>name;
                                cout<<"변경할 인원수를 입력해주세요.:";
                                cin>>input;
                                sprintf(query, "update  Alist set people = '%s' where name = '%s'", input, name);
                                cout<<query<<endl;
                                sandquery(query);
                                cout<<"변경되었습니다."<<endl;
                                break;
                            }
                            else if(ch=='3')
                            {
                                cout<<"이전으로 돌아갑니다."<<endl;
                                break;
                            }
                            else
                                cout<<"다시입력해주세요."<<endl;
                        }
                        else if(ch=='x')
                        {
                            cout<<"다시 메뉴로 돌아갑니다."<<endl;
                            break;
                        }
                        else
                            cout<<"잘못 입력 하셨습니다. 다시 입력해주세요."<<endl;
                    }
                }
                else if(ch=='4')
                {
                    while(1)
                    {
                        cout<<"1. 가이드 요청하기\n2. 가이드 구역 수정\n3. 가이드 요청 취소\n3. 이전으로 돌아가기"<<endl;
                        cin>>ch;
                        if(ch=='1')
                        {
                            cout<<"가이드를 받으실 구역을 설정해주세요.:";
                            cin>>input;
                            cout<<ID<<endl;
                            sprintf(query, "insert into Guide(ID, area) values('%s', '%s')", ID, input);
                            sandquery(query);
                            cout<<"예약되었습니다."<<endl;
                            break;
                        }
                         else if(ch=='2')
                        {
                            sprintf(query, "select * from Guide where ID='%s'", ID);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                cout<<"ID:"<<Row[0]<<"\n지역:"<<Row[1]<<endl;  
                            }
                            while(1)
                            {
                                cout<<"변경될 구역을 입력해주세요.:";
                                cin>>name;
                                cout<<"변경할 지역을 입력해주세요.:";
                                cin>>input;
                                sprintf(query, "update Guide set area = '%s' where area = '%s' and ID ='%s'", input, name, ID);
                                sandquery(query);
                                cout<<"변경 되었습니다."<<endl;
                                break;
                            }
                        }
                        else if(ch=='3')
                        {
                            sprintf(query, "select * from Guide where ID='%s'", ID);
                            sandquery(query);
                            cout<<query<<endl;
                            Result= mysql_store_result(connection);
                            while((Row=mysql_fetch_row(Result))!=NULL)
                            {
                                cout<<"ID:"<<Row[0]<<"\n지역:"<<Row[1]<<endl;  
                            }
                            while(1)
                            {
                                cout<<"최소 하시겠습니까?(Y/N):";
                                cin>>ch;
                                if(ch=='Y'||ch=='y')
                                {
                                    cout<<"취소할 가이드의 지역을 입력해주세요.:";
                                    cin>>input;
                                    sprintf(query, "delete from Guide where area = '%s'", input);
                                    sandquery(query);
                                    cout<<"취소되었습니다."<<endl;
                                    break;
                                }
                                else if(ch=='n'||ch=='N')
                                    break;
                                else
                                    cout<<"잘못입력해셨습니다."<<endl;
                            }
                        }
                        else if(ch=='3')
                        {
                            cout<<"이전으로 돌아가기"<<endl;
                            break;
                        }
                        else
                            cout<<"잘못 입력 하셨습니다. 다시 입력해주세요."<<endl;
                    }
                }
                else if(ch=='5')
                {
                    while(1)
                    {
                        cout<<"정말 탈퇴 하시겠습니까?(Y/N):";
                        cin>>ch;
                        if(ch=='Y'||ch=='y')
                        {
                            sprintf(query, "update  Alist set reservation  = ' ', people = '0' where reservation = '%s'", ID);
                            sandquery(query);
                            sprintf(query, "update  Tlist set reservation  = ' ', people = '0' where reservation = '%s'", ID);
                            sandquery(query);
                            sprintf(query, "delete from Guide where ID = '%s'", ID);
                            sandquery(query);
                            sprintf(query, "update Member set Status = '1' where ID = '%s'", ID);
                            sandquery(query);
                            cout<<"탈퇴하셨습니다."<<endl;
                            exit(1);
                        }
                        else if(ch=='n'||ch=='N')
                            break;
                    }
                }
            }
            
        }
};
#endif
int main()
{
    VIP a;
    a.loginpage();
    a.choice();
    return 0;
}