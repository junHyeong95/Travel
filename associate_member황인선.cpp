// 관광지 내역 조회, 회원 가입 수정 탈퇴
#include "database.h"
#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>

using namespace std;

// 회워가입 : 준회원(nomak)/ 이름,id,pw
class NormalMember : public Database // << 객체를 만들어서 쓸 것
{
protected:
    char Name[20];
    char Query[1024];
    int test;    // 실험용
    string pass; // 아무키 누를 시 다음으로 넘어감


public:
    char Id[20];
    char Pw[20];
    char Id_copy[20];
    char Pw_copy[20];
    //로그인, 회원가입, id찾기, 비밀번호 찾기
    void SignUp();
    char *Login();
    void FindId();
    void FindPw();
};

void NormalMember::SignUp()
{ //                          ////// status   회원 가입할 시 0    탈퇴 시 1
    cout << "사용하실 이름을 입력하세요: ";
    cin >> Name;
    cout << "사용하실 ID를 입력하세요: ";
    cin >> Id;

    sprintf(Query, "select count(*) from Member where ID='%s'", Id);

    cout << "=======ID 중복 검사를 실행합니다.=======\n";
    sleep(1);
    strcpy(Pw, Id);                     // cin으로 값이 들어있지만, else문의 쿼리에서 Id는 쓰레기 값이 있어 strcpy()
    test = mysql_query(ConnPtr, Query); // 쿼리함수가 에러이면 무조건 0 이외의 값이 반환
    if (test != 0)                      // select 문에서 값을 찾아도 못찾아도 반환값은 0 이다. "쿼리"라는 함수가 오류이면 if 문  수행
    {
        fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(ConnPtr));
    }
    else
    {
        // Result = mysql_store_result(ConnPtr);
        sprintf(Query, "select count(*) from Member where ID = '%s'", Pw);

        mysql_query(ConnPtr, Query);
        Result = mysql_store_result(ConnPtr); // 쿼리 수행한 결과 값 모두 담음
        Row = mysql_fetch_row(Result);        // 배열 형태로 바꿈
        if (*Row[0] != '0')
        {
            cout << "사용중인 아이디입니다. 다시입력해주세요.\n"
                 << endl;
            cout << "확인 하신 후 아무키나 누르세요." << endl;
            cin >> pass;
            system("clear");
            mysql_free_result(Result); // 쿼리 담은 것을 모두 메모리 해체
        }
        else
        {
            mysql_free_result(Result); // 쿼리 담은 것을 모두 메모리 해체
            cout << "사용가능한 id입니다. \n\n";
            cout << "사용하실 PW를 입력하세요: ";
            cin >> Pw;

            system("clear");
            sprintf(Query, "INSERT INTO Member(ID,PW,Name,Membership,Status) VALUES('%s','%s','%s','normal','0')", Id, Pw, Name);
            if (mysql_query(&Conn, Query) != 0)
            {
                fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
            }
            else
            {
                sprintf(Query, "select ID,PW,Name,Membership from Member where ID='%s'", Id);
                if (mysql_query(&Conn, Query) != 0)
                {
                    fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
                }
                else
                {
                    Result = mysql_store_result(&Conn);             // 쿼리 수행 결과 값 담음
                    while ((Row = mysql_fetch_row(Result)) != NULL) // 한 행을 구함   
                    {
                        cout << "회원가입하신 정보입니다.....\n";
                        cout << "NAME: " << Row[2] << endl; // 배열 상태로 출력
                        cout << " ID : " << Row[0] << endl;
                        cout << " PW : " << Row[1] << endl;
                    }
                    cout << "회원가입 완료 (아무키나 누르시면 됩니다.)" << endl;
                    cin >> pass;
                    system("clear");
                    mysql_free_result(Result);
                }
            }
        }
    }
}

char *NormalMember::Login()
{
    cout << "<로그인 페이지>"
         << "\nID: ";
    cin >> Id;
    cout << "PW: ";
    cin >> Pw;
    system("clear");
    strcpy(Id_copy, Id); // 조건문들이 수행 되면서 본연의 입력값이 사라지기에 카피
    strcpy(Pw_copy, Pw);
    sprintf(Query, "select PW,Status from Member where ID='%s'", Id); // id로 pw,Status 받음
    test = mysql_query(&Conn, Query);
    if (test != 0)
    {
        fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
    }
    else
    {
        Result = mysql_store_result(&Conn);             // 퀄리 수행 결과 값 담음
        Row = mysql_fetch_row(Result);                  // 배열 형태로 바꿈
        if (!strcmp(Row[0], Pw_copy) && *Row[1] != '1') // pw와 회원 상태라면  ( status가 1 이면 회원 탈퇴상태 > 로그인 불가 )
        {
            cout << "로그인되었습니다.\n";
            return Id;
        }
        else
        {
            return NULL;
        }
    }
}

void NormalMember::FindId()
{
    cout << "<아이디 찾기 페이지>\n";
    cout << "등록된 이름 입력: ";
    cin >> Name;

    sprintf(Query, "select ID,PW,Status from Member where Name='%s'", Name); //// 동일한 이름 포함하여 쿼리문 실행
    if (mysql_query(&Conn, Query) != 0)
    {
        fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
    }
    else
    {
        cout << "등록된 PW 입력: ";
        cin >> Pw;
        Result = mysql_store_result(&Conn);             // 결과 값 담음
        while ((Row = mysql_fetch_row(Result)) != NULL) // 한행 씩 출력 Row[0],[1]
        {
            if (!strcmp(Row[1], Pw) && *Row[2] == '0') // // 회원일 때만 사용했던 ID 제공
            {
                cout << "사용자의 ID는 [ " << Row[0] << " ] 입니다." << endl;
                cout << "\n확인하신 후 아무키나 누르세요...";
                cin >> pass;
                system("clear");
                mysql_free_result(Result); // 결과 값 담은 것을 메모리 해체
                return;
            }
            else if (!strcmp(Row[1], Pw) && *Row[2] == '1') // 탈퇴한 회원일 때
            {
                cout << "\n\t탈퇴된 상태으므로 아이디 조회를 금지합니다.\n";
                cout << "\n확인하신 후 아무키나 누르세요...";
                cin >> pass;
                system("clear");
                mysql_free_result(Result);
                return;
            }
        }
        cout << "\n\t가입하신 이름 또는 pw를 잘못입력했습니다.\n"; // Row값에 아무 것도 없을 시 출력문
        cout << "\n확인하신 후 아무키나 누르세요...";
        cin >> pass;
        system("clear");
        mysql_free_result(Result);
    }
}

void NormalMember::FindPw()
{
    cout << "<비밀번호 찾기 페이지>" << endl;
    cout << "등록된 이름 입력: " << endl;
    cin >> Name;
    sprintf(Query, "select ID,PW,Status from Member where Name='%s'", Name); // 동일한 이름 포함하여 쿼리문 실행
    if (mysql_query(&Conn, Query) != 0)
    {
        fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
    }
    else
    {
        cout << "본인의 ID 입력: "; // 동일한 이름 중 ID 통해 pw가 명확해짐
        cin >> Id;
        Result = mysql_store_result(&Conn);             // 결과 값을 모두 담음
        while ((Row = mysql_fetch_row(Result)) != NULL) // 배열 형태로 바꿈
        {
            if (!strcmp(Row[0], Id) && *Row[2] == '0') // id가 존재하며, 회원일 때
            {
                cout << "사용자의 PW는 [ " << Row[1] << " ] 입니다." << endl;
                cout << "\n확인하신 후 아무키나 누르세요...";
                cin >> pass;
                system("clear");
                mysql_free_result(Result); // 결과 값 메모리 해체
                return;
            }
            else if (!strcmp(Row[0], Id) && *Row[2] == '1') // id가 존재하며, 회원 탈퇴일 때
            {
                cout << "탈퇴된 상태으므로 비밀번호 조회를 금지합니다.\n";
                cout << "\n확인하신 후 아무키나 누르세요...";
                cin >> pass;
                system("clear");
                mysql_free_result(Result);
                return;
            }
        }
        cout << "가입하신 이름 또는 ID를 잘못입력했습니다.\n";
        cout << "\n확인하신 후 아무키나 누르세요...";
        cin >> pass;
        system("clear");
        mysql_free_result(Result);
    }
}

class NormalWithdrawal : public NormalMember
{
private:
    string choice;
    char oath[20];

public:
    void Withdrawal(char *Id, char *Pw, char *Name, char *writtenOath); //  탈퇴 서약
    void W_Login();                                                     // 탈퇴하기
};

void NormalWithdrawal::W_Login()
{
    cout << "정말로 탈퇴하겠습니까? (y / n)\n입력: ";
    cin >> choice;
    if (choice == "y")
    {
        cout << "==========================================\n";
        cout << "등록된 회원의 이름 입력: ";
        cin >> Name;
        sprintf(Query, "select ID,PW,Status from Member where Name='%s'", Name);
        if (mysql_query(&Conn, Query) != 0)
        {
            fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
        }
        else
        {
            cout << "등록된 아이디 입력: ";
            cin >> Id;
            cout << "등록된 비밀번호 입력: ";
            cin >> Pw;
            cout << "==========================================\n";
            Result = mysql_store_result(&Conn); //결과 값 담음

            while ((Row = mysql_fetch_row(Result)) != NULL) // 배열로 바꿈
            {
                if (!strcmp(Row[0], Id) && !strcmp(Row[1], Pw) && *Row[2] == '0') // id,pw가 있으며, 회원일 때 탈퇴 가능
                {
                    cout << "\"consent\"를 따라 써주세요\n>> ";
                    cin >> oath;
                    Withdrawal(Id, Pw, Name, oath);
                    mysql_free_result(Result);
                    return;
                }
                else if (!strcmp(Row[0], Id) && !strcmp(Row[1], Pw) && *Row[2] == '1') // id, pw가 있으며, 탈퇴 회원일 때
                {
                    cout << "탈퇴 되었있는 회원입니다.\n";
                    mysql_free_result(Result);
                    return;
                }
            }
            cout << "\n잘못입력하셨습니다. id 또는 pw찾기를 이용한 후 다시 이용해 주시기바랍니다.\n";
            cout << "\t확인하신 후 아무키나 누르세요\n";
            cin >> pass;
            system("clear");
        }
    }
    else
    {
        cout << "[ n ] 또는 다른 문장을 입력하여 탈퇴 절차를 마칩니다.\n";
        cout << "아무키나 눌러주세요\n";
        cin >> pass;
        system("clear");
    }
}

void NormalWithdrawal::Withdrawal(char *Id, char *Pw, char *Name, char *writtenOath)
{

    if (strcmp(writtenOath, "consent") == 0)
    {
        sprintf(Query, "select * from Member where ID='%s' and PW='%s' and Name='%s'", Id, Pw, Name);
        if (mysql_query(&Conn, Query) != 0)
        {
            fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
        }
        else
        {
            Result = mysql_store_result(&Conn);
            if ((Row = mysql_fetch_row(Result)) != NULL)
            {
                system("clear");
                cout << "\t[ 회원 탈퇴 중입니다... 기다려주세요 ]  \n";
                sleep(2);
                sprintf(Query, "update Member set Status=1 where ID='%s' and PW='%s' and Name='%s'", Row[0], Row[1], Row[2]);
                if (mysql_query(&Conn, Query) != 0)
                {
                    fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
                }
                else
                {
                    cout << "\t성공적을 탈퇴를 마쳤습니다.\n";
                }
            }
        }
    }
    else
    {
        cout << "\"consent\" 외에 다른 문장을 입력하셨습니다.\n";
        cout << "아무키나 눌러주세요\n";
        cin >> pass;
        system("clear");
    }
}

class TourIsm : public Database
{ // 관광지
protected:
    char Name[40]; // 장소조회
    int test;
    char Query[1024];

public:
    // 장소 이름으로 검색
    void NameLookup();
};

void TourIsm::NameLookup()
{ // 이름으로 조회
    cout << "\n\n관광지의 장소 입력 : ";
    cin >> Name;

    sprintf(Query, "select *from Tlist where name = '%s'", Name); //    수정할 것
    test = mysql_query(ConnPtr, Query);
    if (test != 0)
    {
        fprintf(stderr, "Failed to connect to databases: Error: %s\n", mysql_error(&Conn));
    }
    else
    {
        Result = mysql_store_result(&Conn);          // 관광지 정보 모두 담음
        if ((Row = mysql_fetch_row(Result)) != NULL) // 관광지 정보가 있으면 수행
        {
            cout << "\t관광지역 : " << Row[0] << "\n\t장소: " << Row[1] << "\n\t주소: " << Row[2] << "\n\t전화번호 : " << Row[3] << endl;
            mysql_free_result(Result);
            return;
        }
        cout << "\n\t입력하신 관광지의 장소는 없습니다.\n";
        mysql_free_result(Result);
    }
}

class Start : public NormalWithdrawal, TourIsm
{
private:
    int choice;
    int tun_login;
    int loop;
    char *turn_Name;

public:
    void Run();
};

void Start::Run()
{
    NormalMember normal;
    NormalWithdrawal not_normal;
    TourIsm tourism;
    while (1)
    {

        cout << "========================";
        cout << "\n1) 회원 가입   \n2) 로그인   \n3) 회원탈퇴  \n4) id/pw 찾기 \n0) 종료\n";
        cout << "========================";
        cout << "\n입력: ";
        cin >> choice;
        system("clear");
        if (choice == 1) // 회원 가입
        {
            normal.SignUp();
        }
        else if (choice == 2) // 로그인
        {
            turn_Name = normal.Login();
            if (turn_Name != NULL) // 로그인 성공
            {
                while (1)
                {
                    cout << "=======================================================\n";
                    cout << "\t\t안녕하세요. " << turn_Name << "님, \n저희 관광 조회 서비스를 이용해주셔서 진심으로 감사합니다.\n\n";
                    cout << "\t\t< 관광지 내역 조회 > \n\n";
                    cout << "\t\t1) 장소로 조회    \n\t\t2) 로그아웃\n\n\n";

                    cout << "\t\t   입력: ";
                    cin >> choice;
                    if (choice == 1)
                    {
                        // system("clear");
                        tourism.NameLookup();
                        cout << "=======================================================\n";
                        cout << "\t다시 조회하실려면 아무키나 눌러주세요";
                        cin >> pass;
                        system("clear");
                    }
                    else if (choice == 2)
                    {
                        system("clear");
                        break;
                    }
                    else
                    {
                        cout << "잘못 누르셨습니다.\n";
                        cout << "=======================================================\n";
                        cout << "\t다시 조회하실려면 아무키나 눌러주세요";
                        cin >> pass;
                        system("clear");
                    }
                }
            }
            else // 로그인 실패
            {
                cout << "로그인 실패로 이용불가합니다.\n";
            }
        }
        else if (choice == 3) // 회원탈퇴
        {
            not_normal.W_Login();
        }
        else if (choice == 4) // id/pw 찾기
        {
            cout << "============";
            cout << "<회원정보 찾기>\n";
            cout << "\n1)  id 찾기   \n2)  pw 찾기\n";
            cout << "============";
            cout << "\n입력: ";
            cin >> choice;
            if (choice == 1)
            {
                normal.FindId();
            }
            else if (choice == 2)
            {
                normal.FindPw();
            }
            else
            {
                cout << "\n\n잘못입력하여 메뉴로 돌아갑니다.\n\n";
            }
        }
        else if (choice == 0) // 종료
        {
            cout << "종료를 선택했습니다.\n";
            cout << "[  아무키나 누르세요.  ]\n";
            cin >> pass;
            system("clear");
            return;
        }
        else // 잘못 입력
        {
            cout << "잘못 눌렀습니다.\n";
            cout << "[  아무키나 누르세요.  ]\n";
            cin >> pass;
            system("clear");
        }
    }
}

int main()
{
    Start start;
    start.Run();
    return 0;
}